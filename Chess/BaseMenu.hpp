#pragma once
#include "BaseState.hpp"
#include "StateManager.h"

class BaseMenu : public BaseState
{
public:
	BaseMenu(StateManager& stateManager) :
		BaseState(stateManager),
		m_gui(*stateManager.getContext().window)
	{
		const sf::Vector2u windowSize = stateManager.getContext().window->getSize();
		m_view.setSize(sf::Vector2f(windowSize.x, windowSize.y));
		m_view.setCenter(windowSize.x / 2, windowSize.y / 2);
	}

	virtual ~BaseMenu() {};

	virtual void onCreate() = 0;
	virtual void onDestroy() = 0;

	virtual void activate() = 0;
	virtual void deactivate() = 0;

	virtual void render() = 0;
	virtual bool update(float deltaTime) = 0;
	virtual bool handleEvent(const sf::Event & event) {
		if (event.type == sf::Event::Resized) {
			Graphics::applyResize(m_view, event);
			m_gui.setView(m_view);
		}

		return false;
	}

protected:
	tgui::Gui m_gui;
};