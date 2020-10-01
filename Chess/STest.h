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
		Event<int> ev;
		auto connector = ev.connect(&STest::someFunc, this);
		//auto staticCon = ev.connect(&STest::someOtherFunc);
		//subscribing
	//	ev.subscribe(&STest::someFunc, this);
	//	ev.subscribe(&someOtherFunc);

	//	//invoking
	//	ev(123, "123");

	//	//unsubscribing (also happens when ev goes out of scope)
	//	ev.unsubscribe(&STest::someFunc, this);
	//	ev.unsubscribe(&someOtherFunc);

	};

	void someFunc(int num) {

	}

	static void someOtherFunc(int num) {

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

