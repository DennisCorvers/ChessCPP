#include "pch.h"
#include "SMainMenu.h"
#include "Event.h"

SMainMenu::SMainMenu(StateManager & stateManager) :
	BaseState(stateManager),
	m_gui(*m_stateManager->getContext()->window)
{}

SMainMenu::~SMainMenu()
{}

void SMainMenu::onCreate()
{
	initializeUI();
}

void SMainMenu::onDestroy()
{

}

void SMainMenu::activate()
{
}

void SMainMenu::deactivate()
{
}

void SMainMenu::render()
{
	m_gui.draw();
}

bool SMainMenu::update(float deltaTime)
{
	return false;
}

bool SMainMenu::handleEvent(const sf::Event & event)
{
	m_gui.handleEvent(event);
	
	return false;
}

void SMainMenu::initializeUI()
{
	tgui::Button::Ptr but = tgui::Button::create("TestButton");
	but->setPosition("10%", 10);

	sf::Texture t; t.loadFromFile("Assets\\Sprites\\Board.png");

	tgui::BitmapButton::Ptr bmb = tgui::BitmapButton::create();


	//auto func = [this]() {
	//	auto a = m_stateManager;
	//	a->clearStates();
	//};
	////but->connect("pressed", &SMainMenu::onDestroy, this);
	//but->connect("pressed", func);
	m_gui.add(but);
	//https://tgui.eu/examples/0.8/many-different-widgets/

}
