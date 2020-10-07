#include "pch.h"
#include "GuiPauseMenu.h"

GuiPauseMenu::GuiPauseMenu(const SharedContext & sharedContext, bool isClient) :
	GuiWindowBlank(sharedContext),
	m_isClient(isClient)
{ }

GuiPauseMenu::~GuiPauseMenu()
{ 
	m_sharedContext.fontManager->releaseResource(AssetNames::f_opensans_reg);
	m_sharedContext.themeManager->releaseResource(AssetNames::theme_default);
}

void GuiPauseMenu::initialize()
{
	FontManager* fontManager = m_sharedContext.fontManager;
	sf::Font& openSans = *fontManager->requireAndGet(AssetNames::f_opensans_reg);
	FontManager::setSmoothing(openSans, 20, false);

	ThemeManager* themeManager = m_sharedContext.themeManager;
	tgui::Theme& defaultTheme = *themeManager->requireAndGet(AssetNames::theme_default);

	SoundManager* soundManager = m_sharedContext.soundManager;
	const sf::View& view = m_sharedContext.window->getView();

	m_guiWindow->setInheritedFont(openSans);
	int xSize = 300;
	int ySize = 60;

	int xOffset = (view.getSize().x - xSize) / 2;
	int yOffset = view.getSize().y / 4;

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
	background->setPosition(sf::Vector2f((view.getSize().x - background->getSize().x) / 2, view.getSize().y / 4 - 50));
}

void GuiPauseMenu::onAddedToContainer(const sf::View & containerView)
{

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
	OnExitGameEvent.invoke();
}


