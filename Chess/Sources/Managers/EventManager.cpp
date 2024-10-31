#include "pch.h"
#include "EventManager.h"

EventManager::EventManager()
{
}

EventManager::~EventManager()
{
}

bool EventManager::removeCallback(States state, const std::string & name)
{
	auto it = m_callbackContainers.find(state);
	if (it == m_callbackContainers.end())
		return false;

	auto callbackIt = it->second.find(name);
	if (callbackIt == it->second.end())
		return false;

	it->second.erase(name);
	return true;
}

bool EventManager::removeCallbacks(States state)
{
	auto it = m_callbackContainers.find(state);
	if (it == m_callbackContainers.end())
		return false;

	m_callbackContainers.erase(it);
	return true;
}

void EventManager::removeAllCallbacks() {
	m_callbackContainers.clear();
}

void EventManager::handleEvent(States state, const std::string & name, const EventArgs & eventInfo) const
{
	auto container = m_callbackContainers.find(state);
	if (container == m_callbackContainers.end())
		return;

	auto callback = container->second.find(name);
	if (callback == container->second.end())
		return;

	callback->second(eventInfo);
}

void EventManager::handleEvent(const std::string& name, const EventArgs & eventInfo) const
{
	for (auto containers : m_callbackContainers) {
		auto callback = containers.second.find(name);
		if (callback == containers.second.end())
			continue;

		callback->second(eventInfo);
	}
}
