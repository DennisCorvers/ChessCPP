#include "pch.h"
#include "GuiMainMenu.h"


GuiMainMenu::GuiMainMenu()
{
}


GuiMainMenu::~GuiMainMenu()
{
}

void GuiMainMenu::afterInitialize(const SharedContext & sharedContext)
{
	FontManager* fontManager = sharedContext.fontManager;
	sf::Font& openSans = *fontManager->requireAndGet(States::MainMenu, AssetNames::f_opensans_reg);
	FontManager::setSmoothing(openSans, 20, false);

	ThemeManager* themeManager = sharedContext.themeManager;
	tgui::Theme& defaultTheme = *themeManager->requireAndGet(States::MainMenu, AssetNames::theme_default);

	SoundManager* soundManager = sharedContext.soundManager;
	sf::RenderWindow* window = sharedContext.window;

	m_guiWindow->setInheritedFont(tgui::Font(openSans));
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
		m_guiWindow->add(button);
		yOffset += size.y + 50;
	}

	buttons[0]->connect("pressed", [this]() { OnSinglePlayerEvent(); });
	buttons[1]->connect("pressed", [this]() { OnSandboxEvent(); });
	buttons[2]->connect("pressed", &GuiMainMenu::onJoinGamePressed, this);
	buttons[3]->connect("pressed", &GuiMainMenu::onHostGamePressed, this);
	buttons[4]->connect("pressed", [this]() { OnQuitEvent(); });
}

void GuiMainMenu::onDispose(const SharedContext & sharedContext)
{ }

void GuiMainMenu::onJoinGamePressed()
{
	//Open IP Window...
}

void GuiMainMenu::onHostGamePressed()
{
	//Open Port Window...
}
