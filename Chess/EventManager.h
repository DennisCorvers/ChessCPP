#pragma once
#include "States.h"

class EventArgs { };

class EventManager
{
public:
	EventManager();
	~EventManager();

	// Needs to be defined in the header!
	template<class T>
	bool addCallback(States state, const std::string& name,
		void(T::*func)(const EventArgs&), T* instance)
	{
		auto container = m_callbackContainers.emplace(state, CallbackContainer()).first;
		auto tempFunc = std::bind(func, instance, std::placeholders::_1);
		return container->second.emplace(name, tempFunc).second;
	}

	bool removeCallback(States state, const std::string& name);
	bool removeCallbacks(States state);
	void removeAllCallbacks();

	void handleEvent(States state, const std::string& name, const EventArgs& eventInfo) const;
	void handleEvent(const std::string& name, const EventArgs& eventInfo) const;

	static sf::Vector2i GetMousePos(const sf::RenderWindow* const window = nullptr) {
		return (window ? sf::Mouse::getPosition(*window) : sf::Mouse::getPosition());
	}

	static sf::Vector2f GetPixelPosition(const sf::RenderWindow& window, const sf::View& view) {
		return window.mapPixelToCoords(sf::Mouse::getPosition(window), view);
	}

private:
	using Callback = std::function<void(const EventArgs&)>;
	using CallbackContainer = std::unordered_map<std::string, Callback>;

	std::unordered_map<States, CallbackContainer> m_callbackContainers;
};

