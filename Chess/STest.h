#pragma once
#include "BaseState.hpp"
#include "StateManager.h"
#include "GuiContainer.hpp"
#include "GuiBotInput.hpp"

class STest : public BaseState
{
private:
	GuiContainer m_win;

public:
	STest(StateManager& stateManager) :
		BaseState(stateManager),
		m_win(*stateManager.getContext().window)
	{
		auto mb = GuiBotInput::create(m_stateManager->getContext());
		m_win.addShowDialog(mb);
		mb->OnConfirm.connect([](int i) { std::cout << i << std::endl; });
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

