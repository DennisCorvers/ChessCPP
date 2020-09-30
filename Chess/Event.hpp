#include <list>
#include <utility>

template<typename... Args>
class EventHandlerBase {

protected:
	bool m_isStatic;
	EventHandlerBase() {}

public:
	virtual ~EventHandlerBase() {}

	virtual bool equals(EventHandlerBase<Args...>* other) const = 0;
	virtual void invoke(Args... args) const = 0;

	inline bool isStaticHandler() const{
		return m_isStatic;
	}
};


template<typename T, typename... Args>
class EventHandler final : public EventHandlerBase<Args...> {

private:
	T* m_listener;
	void(T::*m_func)(Args...);

public:
	EventHandler(void(T::*func)(Args...), T* instance) :
		m_listener(instance),
		m_func(func)
	{
		this->m_isStatic = false;
	}

	~EventHandler() { }

	bool equals(EventHandlerBase<Args...>* other) const override {
		if (this->isStaticHandler() != other->isStaticHandler())
			return false;

		EventHandler<T, Args...>* castedHandler = static_cast<EventHandler<T, Args...>*>(other);
		return
			this->m_listener == castedHandler->m_listener &&
			this->m_func == castedHandler->m_func;
	}

	void invoke(Args... args) const override {
		(m_listener->*m_func)(args...);
	}
};


template<typename... Args>
class EventHandlerStatic final : public EventHandlerBase<Args...> {

private:
	void(*m_func)(Args...);

public:
	EventHandlerStatic(void(*func)(Args...)) :
		m_func(func)
	{
		this->m_isStatic = true;
	}

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
private:
	using EventHandlerPtr = std::unique_ptr<EventHandlerBase<Args...>>;
	std::vector<EventHandlerPtr> m_eventHandlers;

public:
	Event() {};
	Event(const Event&) = delete;
	Event& operator=(const Event&) = delete;

	void operator()(Args... args) {
		for (auto const &handler : m_eventHandlers)
			handler->invoke(args...);
	}

	////
	///@brief Subscribe to the Event using a member function.
	////
	template<typename T>
	Event& subscribe(void(T::*func)(Args...), T* instance) {
		m_eventHandlers.push_back(std::make_unique<EventHandler<T, Args...>>(func, instance));
		return *this;
	}

	////
	///@brief Subscribe to the Event using a static function.
	////
	Event& subscribe(void(*func)(Args...)) {
		m_eventHandlers.push_back(std::make_unique<EventHandlerStatic<Args...>>(func));
		return *this;
	}

	////
	///@brief Unsubscribe from the Event using a member function.
	////
	template<typename T>
	Event& unsubscribe(void(T::*func)(Args...), T* instance) {
		innerUnsubscribe(&EventHandler<T, Args...>(func, instance));
		return *this;
	}

	////
	///@brief Unsubscribe from the Event using a static function.
	////
	Event& unsubscribe(void(*func)(Args...)) {
		innerUnsubscribe(&EventHandlerStatic<Args...>(func));
		return *this;
	}

	void clearHandlers() noexcept {
		m_eventHandlers.clear();
	}

private:
	void innerUnsubscribe(EventHandlerBase<Args...>* handlerReplica) {
		for (auto it = m_eventHandlers.begin(); it != m_eventHandlers.end();)
		{
			if (!(**it).equals(handlerReplica))
				++it;
			else
				it = m_eventHandlers.erase(it);
		}
	}
};