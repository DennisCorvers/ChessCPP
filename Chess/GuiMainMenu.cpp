#include "pch.h"
#include "GuiMainMenu.h"

GuiMainMenu::GuiMainMenu(const SharedContext & sharedContext) :
	GuiWindowBlank(sharedContext)
{
	initialize();
}

GuiMainMenu::~GuiMainMenu()
{ }

void GuiMainMenu::initialize()
{
	FontManager* fontManager = m_sharedContext.fontManager;
	sf::Font& openSans = *fontManager->requireAndGet(States::MainMenu, AssetNames::f_opensans_reg);
	FontManager::setSmoothing(openSans, 20, false);

	ThemeManager* themeManager = m_sharedContext.themeManager;
	tgui::Theme& defaultTheme = *themeManager->requireAndGet(States::MainMenu, AssetNames::theme_default);

	SoundManager* soundManager = m_sharedContext.soundManager;

	m_guiWindow->setInheritedFont(tgui::Font(openSans));
	sf::Vector2f size(300, 60);
	int yOffset = 0;

	tgui::Button::Ptr buttons[5];
	buttons[0] = tgui::Button::create("Single Player");
	buttons[1] = tgui::Button::create("Sandbox");
	buttons[2] = tgui::Button::create("Join Game");
	buttons[3] = tgui::Button::create("Host Game");
	buttons[4] = tgui::Button::create("Quit");

	for (auto& button : buttons) {
		button->setSize(size);
		button->setPosition(0, yOffset);
		button->setTextSize(20);
		button->setRenderer(defaultTheme.getRenderer("MenuButton"));
		button->connect("mouseentered", [soundManager]() { soundManager->playSound(AssetNames::s_button_hover); });
		button->connect("pressed", [soundManager]() { soundManager->playSound(AssetNames::s_button_click); });
		m_guiWindow->add(button);
		yOffset += size.y + 50;
	}

	buttons[0]->connect("pressed", [this]() { OnSinglePlayerEvent(); });
	buttons[1]->connect("pressed", [this]() { OnSandboxEvent(); });
	buttons[2]->connect("pressed", [this]() { OnJoinGameEvent(); });
	buttons[3]->connect("pressed", [this]() { OnHostGameEvent(); });
	buttons[4]->connect("pressed", [this]() { OnQuitEvent(); });

	m_guiWindow->setSize(300, yOffset);
}

void GuiMainMenu::onAddedToContainer(const sf::View & containerView)
{
	sf::Vector2f size(300, 60);
	int xOffset = (containerView.getSize().x - size.x) / 2;
	int yOffset = containerView.getSize().y / 4;
	m_guiWindow->setPosition(xOffset, yOffset);
}
