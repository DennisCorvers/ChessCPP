#pragma once
#include "BaseState.hpp"
#include "StateManager.h"
#include "GuiContainer.hpp"
#include "MessageBox.hpp"

class STest : public BaseState
{
private:
	GuiContainer m_win;

public:
	STest(StateManager& stateManager) :
		BaseState(stateManager),
		m_win(*stateManager.getContext().window)
	{
		auto mb = MessageBox::create(stateManager.getContext());
		m_win.addWindow(mb);
		mb->show("Hello!\n\nGoodbye...", "Message of the day.", MessageBoxButtons::OK);
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

