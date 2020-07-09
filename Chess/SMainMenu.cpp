#include "pch.h"
#include "SMainMenu.h"
#include "Event.h"

#include "TextureManager.h"
#include "FontManager.h"

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
	FontManager* fontManager = m_stateManager->getContext()->fontManager;
	fontManager->releaseResources(AssetFlags::f_opensans_reg);

	TextureManager* txManager = m_stateManager->getContext()->textureManager;
	txManager->releaseResources(States::MainMenu);
}

void SMainMenu::activate()
{
}

void SMainMenu::deactivate()
{
}

void SMainMenu::render()
{
	auto* window = m_stateManager->getContext()->window;
	window->draw(m_backDrop);

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
	FontManager* fontManager = m_stateManager->getContext()->fontManager;
	sf::Font& openSans = *fontManager->aquireAndGet(AssetFlags::f_opensans_reg, "Assets\\Fonts\\OpenSans-Regular.ttf");

	tgui::Button::Ptr but = tgui::Button::create("TestButton");
	m_gui.setFont(tgui::Font(openSans));

	but->setPosition("10%", 10);

	m_gui.add(but);
	//https://tgui.eu/examples/0.8/many-different-widgets/

}
