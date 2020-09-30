#pragma once
#include "BaseState.hpp"
#include "StateManager.h"
#include "GuiPauseMenu.h"
#include "GuiContainer.hpp"
#include "Event.hpp"

class STest : public BaseState
{
private:
	GuiContainer m_win;

public:
	STest(StateManager& stateManager) :
		BaseState(stateManager),
		m_win(stateManager.getContext())
	{

	}

	virtual ~STest() {};

	virtual void onCreate() override
	{
		auto child = std::make_shared<GuiPauseMenu>();
		m_win.addWindow(child);
		Event<int, std::string> ev;

		ev.subscribe(&STest::someFunc, this);
		ev.subscribe(&someOtherFunc);
		ev(123, "123");

		ev.unsubscribe(&STest::someFunc, this);
		ev.unsubscribe(&someOtherFunc);
		//ev.clearHandlers();
	};

	void someFunc(int num, std::string str) {

	}

	static void someOtherFunc(int num, std::string str) {

	}

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

