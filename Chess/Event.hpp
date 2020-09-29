#include <list>
#include <utility>

template<typename... Args>
class Event {
private:
	template<typename... Args>
	struct EventHandlerBase {

	protected:
		EventHandlerBase() {}

	public:
		virtual ~EventHandlerBase() {}

		virtual bool compare(EventHandlerBase* other) const = 0;
		virtual void invoke(Args... args) const = 0;
	};

	template<typename T, typename... Args>
	struct EventHandler : EventHandlerBase<Args...> {
		EventHandler(void(T::*func)(Args...), T* instance) :
			m_listener(instance), m_func(func) { }
		~EventHandler() { std::cout << "Deleted Handler" << std::endl; }

		bool compare(EventHandlerBase<Args...>* other) const override {
			EventHandler<T, Args...>* castedHandler = static_cast<EventHandler<T, Args...>*>(other);

			return
				this->m_listener == castedHandler->m_listener &&
				this->m_func == castedHandler->m_func;
		}

		void invoke(Args... args) const override {
			if (!m_listener) //Assume static handler
				m_dfunc(args...);
			else
				(m_listener->*m_func)(args...);
		}

	private:
		T* m_listener;
		void(T::*m_func)(Args...);
		void(*m_dfunc)(Args...);
	};

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
		//Uses int instead of T since it's null anyway.
		m_eventHandlers.push_back(std::make_unique<EventHandler<int, Args...>>(func, 0));
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
		innerUnsubscribe(&EventHandler<int, Args...>(func, 0));
		return *this;
	}

private:
	void innerUnsubscribe(EventHandlerBase<Args...>* handlerReplica) {
		for (auto it = m_eventHandlers.begin(); it != m_eventHandlers.end();)
		{
			if ((**it).compare(handlerReplica))
				++it;
			else
				it = m_eventHandlers.erase(it);
		}
	}
};