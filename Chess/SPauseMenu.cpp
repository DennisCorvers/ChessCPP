#include "pch.h"
#include "SPauseMenu.h"
#include "StateManager.h"

SPauseMenu::SPauseMenu(StateManager & stateManager)
	: BaseState(stateManager),
	m_gui(*m_stateManager->getContext().window)
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
	//buttons[1]->connect("pressed", &SMainMenu::onSandboxPressed, this);
	//buttons[2]->connect("pressed", &SMainMenu::onJoinGamePressed, this);
	buttons[3]->connect("pressed", &SPauseMenu::onQuitGamePressed, this);
}

void SPauseMenu::onResumePressed()
{
	m_stateManager->switchState(States::Sandbox);
}

void SPauseMenu::onQuitGamePressed()
{
	m_stateManager->switchState(States::MainMenu);

	m_stateManager->removeState(States::Sandbox);
	m_stateManager->removeState(States::Pause);
}
