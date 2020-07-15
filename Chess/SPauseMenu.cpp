#include "pch.h"
#include "SPauseMenu.h"
#include "StateManager.h"

SPauseMenu::SPauseMenu(StateManager & stateManager)	: 
	BaseMenu(stateManager),
	m_previousState(States::Sandbox)
{} 

SPauseMenu::~SPauseMenu()
{}

void SPauseMenu::onCreate()
{
	initializeUI();
}

void SPauseMenu::onDestroy()
{}

void SPauseMenu::activate()
{
	setActive(true);
	setTransparent(false);
}

void SPauseMenu::deactivate()
{
	setActive(false);
	setTransparent(true);
}

void SPauseMenu::render()
{
	m_stateManager->getContext().window->draw(m_backdrop);
	m_gui.draw();
}

bool SPauseMenu::update(float deltaTime)
{
	return true;
}

bool SPauseMenu::handleEvent(const sf::Event & event)
{
	sf::Event::EventType eType = event.type;
	if (eType == sf::Event::EventType::KeyReleased && event.key.code == sf::Keyboard::Escape) {
		return false;
	}

	m_gui.handleEvent(event);
	return false;
}

void SPauseMenu::onPauseMenuOpen(States callingState) {
	m_previousState = callingState;
}

void SPauseMenu::initializeUI()
{
	FontManager* fontManager = m_stateManager->getContext().fontManager;
	sf::Font& openSans = *fontManager->requireAndGet(States::Pause, AssetNames::f_opensans_reg);

	ThemeManager* themeManager = m_stateManager->getContext().themeManager;
	tgui::Theme& defaultTheme = *themeManager->requireAndGet(States::Pause, AssetNames::theme_default);

	SoundManager* soundManager = m_stateManager->getContext().soundManager;
	sf::RenderWindow* window = m_stateManager->getContext().window;

	m_gui.setFont(tgui::Font(openSans));
	int xSize = 300;
	int ySize = 60;
	int xOffset = (window->getSize().x - xSize) / 2;
	int yOffset = window->getSize().y / 4;


	tgui::Button::Ptr buttons[4];
	buttons[0] = tgui::Button::create("Resume");
	buttons[1] = tgui::Button::create("New Game");
	buttons[2] = tgui::Button::create("Swap Colour");
	buttons[3] = tgui::Button::create("Quit Game");

	for (auto& button : buttons) {
		button->setSize(xSize, ySize);
		button->setPosition(xOffset, yOffset);
		button->setTextSize(20);
		button->setRenderer(defaultTheme.getRenderer("BorderlessButton"));
		button->connect("mouseentered", [soundManager]() {soundManager->playSound(AssetNames::s_button_hover); });
		m_gui.add(button);
		yOffset += ySize + 50;
	}

	buttons[0]->connect("pressed", &SPauseMenu::onResumePressed, this);
	buttons[1]->connect("pressed", &SPauseMenu::onNewGamePressed, this);
	buttons[2]->connect("pressed", &SPauseMenu::onSwapColourPressed, this);
	buttons[3]->connect("pressed", &SPauseMenu::onQuitGamePressed, this);

	sf::Vector2f backSize = sf::Vector2f(300, 60);//xSize + 100, yOffset + 100);
	m_backdrop = sf::RectangleShape(backSize);
	sf::Vector2f pos = sf::Vector2f((window->getSize().x - backSize.x) / 2, (window->getSize().y - backSize.y) / 2);
	pos.x = 400;
	m_backdrop.setPosition(pos);
	//m_backdrop.setPosition(sf::Vector2f((window->getSize().x - backSize.x) / 2, (window->getSize().y - backSize.y) / 2));
	m_backdrop.setFillColor(sf::Color(200, 200, 200, 200));
}

void SPauseMenu::onResumePressed() {
	m_stateManager->switchState(m_previousState);
}

void SPauseMenu::onNewGamePressed()
{
	//Call eventmanager
}

void SPauseMenu::onSwapColourPressed()
{
	//Call eventmanager
}

void SPauseMenu::onQuitGamePressed()
{
	//Quit warning???
	//Shut down game if networked...
	m_stateManager->switchState(States::MainMenu);

	m_stateManager->removeState(m_previousState);
	m_stateManager->removeState(States::Pause);
}
