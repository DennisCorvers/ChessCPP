#include "pch.h"
#include "StateManager.h"
#include "BaseState.h"


StateManager::StateManager(SharedContext& context) :
	m_sharedContext(&context)
{

}

StateManager::~StateManager()
{
	for (auto itr = m_states.begin(); itr != m_states.end(); ++itr) {
		itr->second->onDestroy();
	}
}

void StateManager::update(float deltaTime) const
{
	for (auto itr = m_states.rbegin(); itr != m_states.rend(); ++itr) {
		if (!itr->second->update(deltaTime))
			return;
	}
}

void StateManager::render() const
{
	for (auto itr = m_states.begin(); itr != m_states.end(); ++itr) {
		if (!itr->second->isTransparent()) {
			m_sharedContext->window->setView(itr->second->getView());
			itr->second->render();
		}
	}
}

void StateManager::handleEvent(const sf::Event & event) const
{
	for (auto itr = m_states.rbegin(); itr != m_states.rend(); ++itr) {
		if (!itr->second->handleEvent(event))
			return;
	}
}

void StateManager::lateUpdate()
{
	while (m_toRemove.begin() != m_toRemove.end()) {
		removeInternal(*m_toRemove.begin());
		m_toRemove.erase(m_toRemove.begin());
	}

}

bool StateManager::hasState(const States stateID) const
{
	for (auto itr = m_states.begin(); itr != m_states.end(); ++itr)
	{
		if (itr->first == stateID) {
			auto removed = std::find(m_toRemove.begin(), m_toRemove.end(), stateID);
			if (removed == m_toRemove.end())
				return true;
			else
				return false;
		}
	}
	return false;

}

bool StateManager::isEmpty() const {
	return m_states.size() < 1;
}

void StateManager::createState(States stateID)
{
	auto factoryFunction = m_factories.find(stateID);
	assert(factoryFunction != m_factories.end());

	auto newState = factoryFunction->second();
	auto statePtr = newState.get();

	m_sharedContext->window->setView(newState->getView());
	m_states.emplace_back(stateID, std::move(newState));
	statePtr->onCreate();
}

void StateManager::removeInternal(const States stateID)
{
	for (auto it = m_states.begin(); it != m_states.end(); ++it) {
		if (it->first != stateID)
			continue;

		it->second->onDestroy();
		m_states.erase(it);
		return;
	}
}

void StateManager::switchState(const States stateID)
{
	for (auto itr = m_states.begin(); itr != m_states.end(); ++itr)
	{
		if (itr->first == stateID) {
			m_states.back().second->deactivate();

			States stateid = itr->first;
			auto& state = itr->second;

			m_states.erase(itr);
			m_states.emplace_back(stateID, std::move(state));

			state->activate();
			m_sharedContext->window->setView(state->getView());
			return;
		}
	}

	if (!m_states.empty())
		m_states.back().second->deactivate();

	createState(stateID);
	m_states.back().second->activate();
	m_sharedContext->window->setView(m_states.back().second->getView());
}

void StateManager::removeState(const States stateID) {
	m_toRemove.emplace_back(stateID);
}

void StateManager::clearStates()
{
	for (auto& item : m_states)
		removeState(item.first);
}


