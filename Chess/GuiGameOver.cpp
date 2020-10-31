#include "pch.h"
#include "GuiGameOver.h"


GuiGameOver::GuiGameOver(const SharedContext & sharedContext) :
	GuiWindowBlank(sharedContext)
{
	m_continueButton = tgui::Button::create("Confirm");
	m_text = tgui::Label::create();
	m_title = tgui::Label::create();
	m_background = tgui::Panel::create();

	initialize();
}

GuiGameOver::~GuiGameOver()
{
	m_sharedContext.fontManager->releaseResource(AssetNames::f_opensans_reg);
	m_sharedContext.themeManager->releaseResource(AssetNames::theme_default);
}

void GuiGameOver::setText(const std::string & text) {
	m_text->setText(text);
}

void GuiGameOver::setTitle(const std::string & title) {
	m_title->setText(title);
}

void GuiGameOver::initialize()
{
	FontManager* fontManager = m_sharedContext.fontManager;
	sf::Font& openSans = *fontManager->requireAndGet(AssetNames::f_opensans_reg);
	FontManager::setSmoothing(openSans, 20, false);

	ThemeManager* themeManager = m_sharedContext.themeManager;
	tgui::Theme& defaultTheme = *themeManager->requireAndGet(AssetNames::theme_default);

	SoundManager* soundManager = m_sharedContext.soundManager;
	const sf::View& view = m_sharedContext.window->getView();

	m_guiWindow->setInheritedFont(openSans);

	int screenXSize = 500;
	int screenYSize = 200;
	int butXSize = 200;
	int butYSize = 30;
	int butXOffset = 10;
	int butYOffset = 20;
	int butYPosition = screenYSize - butYSize - butYOffset;

	//Background
	m_background->setSize(sf::Vector2f(screenXSize, screenYSize));
	m_background->setRenderer(defaultTheme.getRenderer("PauseBackground"));
	m_background->getRenderer()->setBorders(tgui::Borders::Outline(1));
	m_background->getRenderer()->setBorderColor(tgui::Color::Black);
	m_guiWindow->add(m_background);

	//Cancel Button
	m_continueButton->setSize(butXSize, butYSize);
	m_continueButton->setPosition((screenXSize / 2) - butXSize - butXOffset, butYPosition);
	m_continueButton->setTextSize(20);
	m_continueButton->setRenderer(defaultTheme.getRenderer("PauseButton"));
	m_continueButton->connect("mouseentered", [soundManager]() {soundManager->playSound(AssetNames::s_button_hover); });
	m_continueButton->connect("pressed", [soundManager]() {soundManager->playSound(AssetNames::s_button_click); });
	m_guiWindow->add(m_continueButton);

	//Textbox
	m_text->setSize(screenXSize - 80, butYSize);
	m_text->setPosition((screenXSize - m_text->getSize().x) / 2, butYPosition - m_text->getSize().y - 30);
	m_text->setTextSize(15);
	m_guiWindow->add(m_text);

	//Header
	tgui::Panel::Ptr headerBack = tgui::Panel::create();
	headerBack->setSize(screenXSize, 30);
	headerBack->setRenderer(defaultTheme.getRenderer("PauseHeader"));
	headerBack->getRenderer()->setBorders(tgui::Borders::Outline(1));
	headerBack->getRenderer()->setBorderColor(tgui::Color::Black);
	m_guiWindow->add(headerBack);

	//Header label
	m_title->setPosition(10, 2);
	m_title->setTextSize(20);
	m_title->setText("Game Over");
	m_title->setRenderer(defaultTheme.getRenderer("PauseLabel"));
	m_guiWindow->add(m_title);

	m_guiWindow->setSize(m_background->getSize());
}

void GuiGameOver::onContinueClick() {
	hide();
	OnContinue();
}

void GuiGameOver::onEscapePress() {
	hide();
	OnContinue();
}

void GuiGameOver::onAddedToContainer(const sf::View & containerView)
{
	m_guiWindow->setPosition(sf::Vector2f(
		(containerView.getSize().x - m_guiWindow->getSize().x) / 2,
		(containerView.getSize().y - m_guiWindow->getSize().y) / 2
	));
}

