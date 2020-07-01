#include "pch.h"
#include "StateManager.h"

StateManager::StateManager(SharedContext& context) :
	m_sharedContext(&context)
{
	//TODO Register all states here?
}

StateManager::~StateManager()
{
	for (auto itr = m_states.begin(); itr != m_states.end(); ++itr) {
		itr->second->onDestroy();
	}
}

void StateManager::update(float deltaTime)
{
	for (auto itr = m_states.rbegin(); itr != m_states.rend(); ++itr) {
		if (!itr->second->update(deltaTime))
			return;
	}
}

void StateManager::render()
{
	for (auto itr = m_states.rbegin(); itr != m_states.rend(); ++itr) {
		if (!itr->second->isTransparent()) {
			//TODO Set view to window?
			itr->second->render();
		}
	}
}

void StateManager::handleEvent(const sf::Event & event)
{
	for (auto itr = m_states.rbegin(); itr != m_states.rend(); ++itr) {
		if (!itr->second->handleEvent(event))
			return;
	}
}

void StateManager::lateUpdate()
{
	while (m_toRemove.begin() != m_toRemove.end()) {
		removeState(*m_toRemove.begin());
		m_toRemove.erase(m_toRemove.begin());
	}

}

SharedContext * StateManager::getContext() {
	return m_sharedContext;
}

void StateManager::createState(States stateID)
{
	auto factoryFunction = m_factories.find(stateID);
	assert(factoryFunction != m_factories.end());

	auto newState = factoryFunction->second();
	//TODO set default view for state?

	m_states.emplace_back(stateID, std::move(newState));
	newState->onCreate();
}

void StateManager::removeState(const States stateID)
{
	for (auto it = m_states.begin(); it != m_states.end(); ++it) {
		if (it->first != stateID)
			continue;

		it->second->onDestroy();
		m_states.erase(it);
		return;
	}

}
