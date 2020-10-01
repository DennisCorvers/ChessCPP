#include <list>
#include <utility>

///Forward declarations
class Connection;
class HandlerBase;

template<typename... Args>
class Event;

template<typename... Args>
class EventHandlerBase;

template<typename T, typename... Args>
class EventHandler;

template<typename... Args>
class EventHandlerStatic;

//Aliases
using Connector = std::shared_ptr<Connection>;

//Implementation
class HandlerBase {
public:
	virtual void disconnect() = 0;
};

class Connection {
private:
	template<typename... Args>
	friend class Event;

	std::weak_ptr<HandlerBase> m_eventBase;
	Connection(std::weak_ptr<HandlerBase> eventBase) :
		m_eventBase(eventBase)
	{};

	Connection(const Connection&) = delete;
	Connection& operator=(const Connection&) = delete;

	static std::shared_ptr<Connection> create(const std::shared_ptr<HandlerBase>& eventBase) {
		return std::shared_ptr<Connection>(new Connection(std::weak_ptr<HandlerBase>(eventBase)));
	}

public:
	bool disconnect() {
		if (!m_eventBase.expired()) {
			m_eventBase.lock()->disconnect();
			return true;
		}
		return false;
	}

	bool isConnected() const {
		return !m_eventBase.expired();
	}

	~Connection() {
		disconnect();
	}
};

template<typename... Args>
class EventHandlerBase : public HandlerBase {
private:
	Event<Args...>* m_event;
	bool m_isStatic;
	uint32_t m_key;

public:
	EventHandlerBase(uint32_t key, bool isStatic, Event<Args...>* event) :
		m_key(key),
		m_isStatic(isStatic),
		m_event(event) { }

	virtual ~EventHandlerBase() {}

	virtual bool isExpired() const {
		return false;
	}
	inline bool isStaticHandler() const {
		return m_isStatic;
	}

protected:
	virtual bool equals(EventHandlerBase<Args...>* other) const = 0;
	virtual void invoke(Args... args) const = 0;

private:
	void disconnect() {
		m_event->connectorUnsubscribe(m_key);
	}
};

template<typename T, typename... Args>
class EventHandler final : public EventHandlerBase<Args...> {
public:
	std::weak_ptr<Connection> m_connector;
	T* m_listener;
	void(T::*m_func)(Args...);

	EventHandler(uint32_t key, void(T::*func)(Args...), T* instance, Event<Args...>* event) :
		EventHandlerBase<Args...>(key, false, event),
		m_listener(instance),
		m_func(func)
	{ }

	~EventHandler() { }

	void setConnector(const std::shared_ptr<Connection>& connector) {
		m_connector = std::weak_ptr<Connection>(connector);
	}

	bool equals(EventHandlerBase<Args...>* other) const override {
		if (this->isStaticHandler() != other->isStaticHandler())
			return false;

		EventHandler<T, Args...>* castedHandler = static_cast<EventHandler<T, Args...>*>(other);
		return
			this->m_listener == castedHandler->m_listener &&
			this->m_func == castedHandler->m_func;
	}

	void invoke(Args... args) const override {
		if (!isExpired())
			(m_listener->*m_func)(args...);
	}

	bool isExpired() const override {
		return m_connector.expired();
	}
};

template<typename... Args>
class EventHandlerStatic final : public EventHandlerBase<Args...> {
public:
	void(*m_func)(Args...);

	EventHandlerStatic(uint32_t key, void(*func)(Args...), Event<Args...>* event) :
		EventHandlerBase<Args...>(key, true, event),
		m_func(func)
	{ }

	~EventHandlerStatic() { }

	bool equals(EventHandlerBase<Args...>* other) const override {
		if (this->isStaticHandler() != other->isStaticHandler())
			return false;

		EventHandlerStatic<Args...>* castedHandler = static_cast<EventHandlerStatic<Args...>*>(other);

		return this->m_func == castedHandler->m_func;
	}

	void invoke(Args... args) const override {
		(*m_func)(args...);
	}
};

template<typename... Args>
class Event {
	template<typename... Args>
	friend class EventHandlerBase;

private:
	using EventHandlerPtr = std::shared_ptr<EventHandlerBase<Args...>>;
	std::unordered_map<uint32_t, EventHandlerPtr> m_eventHandlers;
	uint32_t m_connectorKey = 0;

public:
	Event() {};
	Event(const Event&) = delete;
	Event& operator=(const Event&) = delete;

	void operator()(Args... args) {
		for (auto it = m_eventHandlers.begin(); it != m_eventHandlers.end();)
		{
			if ((**it).isExpired()) {
				it = m_eventHandlers.erase(it);
			}
			else {
				(**it).invoke(args...);
				++it;
			}
		}
	}

	////
	///@brief Subscribe to the Event using a member function.
	////
	template<typename T>
	std::shared_ptr<Connection> connect(void(T::*func)(Args...), T* instance) {
		auto handler = std::make_shared<EventHandler<T, Args...>>(m_connectorKey, func, instance, this);
		auto connector = Connection::create(handler);
		handler->setConnector(connector);

		m_eventHandlers.emplace(m_connectorKey++, std::move(handler));
		return connector;
	}

	////
	///@brief Subscribe to the Event using a static member function.
	///		Does not automatically get disconnected as there is no member to become null.
	////
	void connect(void(*func)(Args...)) {
		auto handler = std::make_shared<EventHandlerStatic<Args...>>(m_connectorKey, func, this);
		auto connector = Connection::create(handler);
		handler->setConnector(connector);

		m_eventHandlers.emplace(m_connectorKey++, std::move(handler));
		return connector;
	}

	////
	///@brief Subscribe to the Event using a static function.
	///		Does not automatically get disconnected.
	////
	void staticConnect(void(*func)(Args...)) {
		m_eventHandlers.push_back(std::make_shared<EventHandlerStatic<Args...>>(func));
	}

	////
	///@brief Unsubscribe from the Event using a static function.
	////
	void staticDisconnect(void(*func)(Args...)) {
		innerUnsubscribe(EventHandlerStatic<Args...>(func));
	}

	////
	///@brief Removes all EventHandlers from this Event.
	////
	void clearHandlers() noexcept {
		m_eventHandlers.clear();
	}

	////
	///@brief Removes all expired EventHandlers from this Event.
	////
	void cleanExpiredHandlers() noexcept {
		for (auto it = m_eventHandlers.begin(); it != m_eventHandlers.end(); ++it) {
			if (it->second->isExpired())
				it = m_eventHandlers.erase(it);
			else
				++it;
		}
	}


private:
	bool connectorUnsubscribe(uint32_t handlerID) {
		auto handler = m_eventHandlers.find(handlerID);

		if (handler == m_eventHandlers.end())
			return false;

		m_eventHandlers.erase(handler);
	}

	void innerUnsubscribe(const EventHandlerBase<Args...>& handlerReplica) {
		for (auto it = m_eventHandlers.begin(); it != m_eventHandlers.end();)
		{
			if (!(**it).equals(handlerReplica))
				++it;
			else
				it = m_eventHandlers.erase(it);
		}
	}
};