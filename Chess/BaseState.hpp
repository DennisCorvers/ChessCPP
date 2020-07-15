#pragma once
#include "States.h"
#include "assetNames.h"

class StateManager;
class BaseState
{
public:
	BaseState(StateManager& stateManager) :
		m_stateManager(&stateManager),
		m_isTransparent(false),
		m_isActive(true)
	{}

	virtual ~BaseState() {}

	virtual void onCreate() = 0;
	virtual void onDestroy() = 0;

	virtual void activate() = 0;
	virtual void deactivate() = 0;

	virtual void render() = 0;
	virtual bool update(float deltaTime) = 0;
	virtual bool handleEvent(const sf::Event& event) = 0;

	void setTransparent(bool isTransparent) {
		m_isTransparent = isTransparent;
	}
	bool isTransparent() const {
		return m_isTransparent;
	}

	void setActive(bool isActive) {
		m_isActive = isActive;
	}
	bool isActive() const {
		return m_isActive;
	}

	const sf::View& getView() const {
		return m_view;
	}
	const StateManager& getStateManager() const {
		return *m_stateManager;
	}

protected:
	StateManager* m_stateManager;
	bool m_isTransparent;
	bool m_isActive;
	sf::View m_view;
};

