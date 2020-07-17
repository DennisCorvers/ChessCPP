#pragma once

template <typename... Args>
class Event {
private:
	typedef std::function<void(Args...)> EventHandler;
	std::unordered_map<std::string, EventHandler> m_callbacks;

public:

	void operator()(Args... args) {
		for (auto handler : m_callbacks)
			handler.second(args...);
	}

	void AddCallback(const std::string& name, const EventHandler& func) {
		m_callbacks.emplace(name, func);
	}

	void RemoveCallback(const std::string& name) {
		auto itr = m_callbacks.find(name);
		if (itr == m_callbacks.end())
			return;

		m_callbacks.erase(name);
	}
};

