#include "pch.h"
#include "GuiPauseMenu.h"


GuiPauseMenu::GuiPauseMenu(bool isClient) :
	m_isClient(isClient)
{ }

GuiPauseMenu::~GuiPauseMenu()
{ }

void GuiPauseMenu::onInitialize(const SharedContext & sharedContext)
{
	FontManager* fontManager = sharedContext.fontManager;
	sf::Font& openSans = *fontManager->requireAndGet(AssetNames::f_opensans_reg);
	FontManager::setSmoothing(openSans, 20, false);

	ThemeManager* themeManager = sharedContext.themeManager;
	tgui::Theme& defaultTheme = *themeManager->requireAndGet(AssetNames::theme_default);

	SoundManager* soundManager = sharedContext.soundManager;
	sf::RenderWindow* window = sharedContext.window;

	m_guiWindow->setSize(window->getSize().x, window->getSize().y);
	m_guiWindow->setInheritedFont(openSans);
	int xSize = 300;
	int ySize = 60;
	int xOffset = (window->getSize().x - xSize) / 2;
	int yOffset = window->getSize().y / 4;

	tgui::Panel::Ptr background = tgui::Panel::create();
	background->setRenderer(defaultTheme.getRenderer("PauseBackground"));
	m_guiWindow->add(background);

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
			m_guiWindow->add(button);
		}
		yOffset += ySize + 50;
	}

	background->setSize(sf::Vector2f(xSize + 100, yOffset - ySize * 3));
	background->setPosition(sf::Vector2f((window->getSize().x - background->getSize().x) / 2, window->getSize().y / 4 - 50));
}

void GuiPauseMenu::onDispose(const SharedContext & sharedContext)
{
	sharedContext.fontManager->releaseResource(AssetNames::f_opensans_reg);
	sharedContext.themeManager->releaseResource(AssetNames::theme_default);
}

void GuiPauseMenu::createButtons(std::vector<tgui::Button::Ptr>& buttons)
{
	buttons.push_back(tgui::Button::create("Resume"));
	buttons.back()->connect("pressed", &GuiPauseMenu::hide, this);

	//Don't 1 and 2 these when Network Client
	if (!m_isClient) {
		buttons.push_back(tgui::Button::create("New Game"));
		buttons.back()->connect("pressed", &GuiPauseMenu::onNewGame, this);

		buttons.push_back(tgui::Button::create("Swap Colour"));
		buttons.back()->connect("pressed", &GuiPauseMenu::onSwapColour, this);
	}

	buttons.push_back(tgui::Button::create("Quit Game"));
	buttons.back()->connect("pressed", &GuiPauseMenu::onExitGame, this);
}

void GuiPauseMenu::onNewGame()
{
	hide();
	OnNewGameEvent.invoke();
}

void GuiPauseMenu::onSwapColour()
{
	hide();
	OnSwapColourEvent.invoke();
}

void GuiPauseMenu::onExitGame()
{
	hide();
	OnExitGameEvent.invoke();
}


