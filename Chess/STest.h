#pragma once
#include "BaseState.hpp"
#include "StateManager.h"
#include "GuiContainer.hpp"
#include "GuiGameOver.h"
#include "GuiJoinGame.hpp"

class STest : public BaseState
{
private:
	GuiContainer m_win;

public:
	STest(StateManager& stateManager) :
		BaseState(stateManager),
		m_win(*stateManager.getContext().window)
	{
		auto mb = GuiJoinGame::create(stateManager.getContext());
		//mb->setText("Game ended in a Draw.");
		m_win.addShowDialog(mb);
	}

	virtual ~STest() {};

	virtual void onCreate() override
	{

	};

	virtual void onDestroy() override
	{
	};

	virtual void activate() override
	{
	};

	virtual void deactivate() override
	{
	};

	virtual void render() override
	{
		m_win.render();
	};

	virtual bool update(float deltaTime) override
	{
		return false;
	};

	virtual bool handleEvent(const sf::Event & event) override
	{
		return m_win.handleEvent(event);
	};

};

