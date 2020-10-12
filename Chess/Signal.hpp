#pragma once
#include <unordered_map>
#include <functional>

template<typename... Args>
class Signal final {

private:
	unsigned int m_id;
	std::unordered_map<unsigned int, std::function<void(Args...)>> m_handlers;

public:

	Signal() : m_id(0) { }

	Signal(const Signal& other) noexcept = default;
	Signal(Signal&& other) noexcept = default;
	Signal& operator=(const Signal& other) noexcept = default;
	Signal& operator=(Signal&& other) noexcept = default;

	void invoke(const Args&... args) {
		for (auto itr = m_handlers.begin(); itr != m_handlers.end();)
		{
			if (!itr->second)
				itr = m_handlers.erase(itr);
			else
				itr->second(args...);
			++itr;
		}
	}

	void operator() (const Args&... args) {
		invoke(args...);
	}

	template<class T>
	unsigned int connect(void(T::*func)(Args...), T* instance) {
		return connect([=](Args... args) {  (instance->*func)(args...);	});
	}

	unsigned int connect(std::function<void(Args...)> const& handler) {
		m_handlers.emplace(++m_id, handler);
		return m_id;
	}

	void disconnect(unsigned int id) {
		auto itr = m_handlers.find(id);

		if (itr != m_handlers.end())
			m_handlers.erase(itr);
	}

	void disconnectAll() {
		m_handlers.clear();
	}


protected:
};