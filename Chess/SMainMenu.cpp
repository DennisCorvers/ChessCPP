#include "pch.h"
#include "SMainMenu.h"
#include "ResourceManagers.hpp"

SMainMenu::SMainMenu(StateManager & stateManager) :
	BaseMenu(stateManager)
{}

SMainMenu::~SMainMenu()
{}

void SMainMenu::onCreate()
{
	initializeUI();
}

void SMainMenu::onDestroy() {}

void SMainMenu::activate() {}

void SMainMenu::deactivate() {}

void SMainMenu::render() {
	m_gui.draw();
}

bool SMainMenu::update(float deltaTime) {
	return false;
}

bool SMainMenu::handleEvent(const sf::Event & event)
{
	BaseMenu::handleEvent(event);
	m_gui.handleEvent(event);

	return false;
}

void SMainMenu::initializeUI()
{
	FontManager* fontManager = m_stateManager->getContext().fontManager;
	sf::Font& openSans = *fontManager->requireAndGet(States::MainMenu, AssetNames::f_opensans_reg);
	FontManager::setSmoothing(openSans, 20, false);

	ThemeManager* themeManager = m_stateManager->getContext().themeManager;
	tgui::Theme& defaultTheme = *themeManager->requireAndGet(States::MainMenu, AssetNames::theme_default);

	SoundManager* soundManager = m_stateManager->getContext().soundManager;
	sf::RenderWindow* window = m_stateManager->getContext().window;

	m_gui.setFont(tgui::Font(openSans));
	sf::Vector2f size(300, 60);
	int xOffset = (window->getSize().x - size.x) / 2;
	int yOffset = window->getSize().y / 4;

	tgui::Button::Ptr buttons[5];
	buttons[0] = tgui::Button::create("Single Player");
	buttons[1] = tgui::Button::create("Sandbox");
	buttons[2] = tgui::Button::create("Join Game");
	buttons[3] = tgui::Button::create("Host Game");
	buttons[4] = tgui::Button::create("Quit");

	for (auto& button : buttons) {
		button->setSize(size);
		button->setPosition(xOffset, yOffset);
		button->setTextSize(20);
		button->setRenderer(defaultTheme.getRenderer("MenuButton"));
		button->connect("mouseentered", [soundManager]() {soundManager->playSound(AssetNames::s_button_hover); });
		button->connect("pressed", [soundManager]() {soundManager->playSound(AssetNames::s_button_click); });
		m_gui.add(button);
		yOffset += size.y + 50;
	}

	buttons[0]->connect("pressed", &SMainMenu::onSinglePlayerPressed, this);
	buttons[1]->connect("pressed", &SMainMenu::onSandboxPressed, this);
	buttons[2]->connect("pressed", &SMainMenu::onJoinGamePressed, this);
	buttons[3]->connect("pressed", &SMainMenu::onHostGamePressed, this);
	buttons[4]->connect("pressed", &SMainMenu::onQuitPressed, this);
}

void SMainMenu::onQuitPressed() {
	m_stateManager->clearStates();
}

void SMainMenu::onSinglePlayerPressed()
{
	m_stateManager->switchState(States::SinglePlayer);
	m_stateManager->removeState(States::MainMenu);
}

void SMainMenu::onJoinGamePressed()
{
}

void SMainMenu::onHostGamePressed()
{
}

void SMainMenu::onSandboxPressed() {
	m_stateManager->switchState(States::Sandbox);
	m_stateManager->removeState(States::MainMenu);
}
