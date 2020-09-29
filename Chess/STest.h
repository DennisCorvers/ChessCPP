#pragma once
#include "BaseState.hpp"
#include "StateManager.h"
#include "GuiPauseMenu.h"
#include "GuiContainer.hpp"

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
		//auto child = tgui::ChildWindow::create();
		//m_gui.add(child);

		//auto button = tgui::Button::create("Button");
		//child->add(button);
		//auto renderer = child->getRenderer();
		//renderer->setBackgroundColor(tgui::Color::Transparent);
		//renderer->setBorderColor(tgui::Color::Magenta);
		////child->setEnabled(false);

		//auto child2 = tgui::ChildWindow::create();
		//m_gui.add(child2);

		//auto button2 = tgui::Button::create("Child2 Button");
		//child2->add(button2);
		//auto renderer2 = child2->getRenderer();
		//renderer2->setBackgroundColor(tgui::Color::Green);

		//child->moveToFront();
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

