#include "pch.h"
#include "SPauseMenu.h"
#include "StateManager.h"
#include "EventManager.h"

SPauseMenu::SPauseMenu(StateManager & stateManager) :
	BaseMenu(stateManager)
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
	m_gui.unfocusAllWidgets();
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
	BaseMenu::handleEvent(event);

	if (event.type == sf::Event::KeyReleased && event.key.code == sf::Keyboard::Escape)
		deactivate();

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

	tgui::Panel::Ptr background = tgui::Panel::create();
	background->setRenderer(defaultTheme.getRenderer("PauseBackground"));
	m_gui.add(background);

	std::vector<tgui::Button::Ptr> buttons;
	buttons.reserve(4);
	createButtons(buttons);

	for (auto& button : buttons) {
		if (button) {
			button->setSize(xSize, ySize);
			button->setPosition(xOffset, yOffset);
			button->setTextSize(20);
			button->setRenderer(defaultTheme.getRenderer("PauseButton"));
			button->connect("mouseentered", [soundManager]() {soundManager->playSound(AssetNames::s_button_hover); });
			button->connect("pressed", [soundManager]() {soundManager->playSound(AssetNames::s_button_click); });
			m_gui.add(button);
		}
		yOffset += ySize + 50;
	}

	background->setSize(sf::Vector2f(xSize + 100, yOffset - ySize * 3));
	background->setPosition(sf::Vector2f((window->getSize().x - background->getSize().x) / 2, window->getSize().y / 4 - 50));
	//m_backdrop = sf::RectangleShape(sf::Vector2f(xSize + 100, yOffset - ySize * 3));
	//m_backdrop.setPosition(sf::Vector2f((window->getSize().x - m_backdrop.getSize().x) / 2, window->getSize().y / 4 - 50));
	//m_backdrop.setFillColor(sf::Color(255, 255, 255, 250));
	//m_backdrop.setOutlineColor(sf::Color(50, 50, 50, 120));
	//m_backdrop.setOutlineThickness(2);
}

void SPauseMenu::createButtons(std::vector<tgui::Button::Ptr>& buttons)
{
	buttons.push_back(tgui::Button::create("Resume"));
	buttons.back()->connect("pressed", &SPauseMenu::onResumePressed, this);

	//Don't 1 and 2 these when Network Client
	buttons.push_back(tgui::Button::create("New Game"));
	buttons.back()->connect("pressed", &SPauseMenu::onNewGamePressed, this);

	if (!m_stateManager->hasState(States::SinglePlayer)) {
		buttons.push_back(tgui::Button::create("Swap Colour"));
		buttons.back()->connect("pressed", &SPauseMenu::onSwapColourPressed, this);
	}
	else {
		buttons.push_back(nullptr);
	}

	buttons.push_back(tgui::Button::create("Quit Game"));
	buttons.back()->connect("pressed", &SPauseMenu::onQuitGamePressed, this);
}

void SPauseMenu::onResumePressed() {
	deactivate();
}

void SPauseMenu::onNewGamePressed() {
	deactivate();
	m_stateManager->getContext().eventManager->handleEvent(States::Pause, "onResetBoard", EventArgs());
}

void SPauseMenu::onSwapColourPressed() {
	deactivate();
	m_stateManager->getContext().eventManager->handleEvent(States::Pause, "onSwitchBoard", EventArgs());
}

void SPauseMenu::onQuitGamePressed()
{
	m_stateManager->getContext().eventManager->handleEvent(States::Pause, "onQuitGame", EventArgs());
	//Quit warning?
	m_stateManager->removeState(States::Pause);
}
