#include "pch.h"
#include "GuiInputWindow.h"


GuiInputWindow::GuiInputWindow(const SharedContext & sharedContext) :
	GuiWindowBlank(sharedContext)
{
	m_cancelButton = tgui::Button::create("Cancel");
	m_confirmButton = tgui::Button::create("Confirm");
	m_textField = tgui::TextBox::create();
	m_commentLabel = tgui::Label::create();

	initialize();
}

GuiInputWindow::~GuiInputWindow()
{
	m_sharedContext.fontManager->releaseResource(AssetNames::f_opensans_reg);
	m_sharedContext.themeManager->releaseResource(AssetNames::theme_default);
}

void GuiInputWindow::initialize()
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
	tgui::Panel::Ptr background = tgui::Panel::create();
	background->setSize(sf::Vector2f(screenXSize, screenYSize));
	background->setRenderer(defaultTheme.getRenderer("PauseBackground"));
	background->getRenderer()->setBorders(tgui::Borders::Outline(1));
	background->getRenderer()->setBorderColor(tgui::Color::Black);
	m_guiWindow->add(background);

	//Cancel Button
	m_confirmButton->setSize(butXSize, butYSize);
	m_confirmButton->setPosition((screenXSize / 2) - butXSize - butXOffset, butYPosition);
	m_confirmButton->setTextSize(20);
	m_confirmButton->setRenderer(defaultTheme.getRenderer("PauseButton"));
	m_confirmButton->connect("mouseentered", [soundManager]() {soundManager->playSound(AssetNames::s_button_hover); });
	m_confirmButton->connect("pressed", [soundManager]() {soundManager->playSound(AssetNames::s_button_click); });
	m_guiWindow->add(m_cancelButton);

	//Confirm button
	m_cancelButton->setSize(butXSize, butYSize);
	m_cancelButton->setPosition((screenXSize / 2) + butXOffset, butYPosition);
	m_cancelButton->setTextSize(20);
	m_cancelButton->setRenderer(defaultTheme.getRenderer("PauseButton"));
	m_cancelButton->connect("mouseentered", [soundManager]() {soundManager->playSound(AssetNames::s_button_hover); });
	m_cancelButton->connect("pressed", [soundManager]() {soundManager->playSound(AssetNames::s_button_click); });
	m_guiWindow->add(m_confirmButton);

	//Textbox
	m_textField->setSize(screenXSize - 80, butYSize);
	m_textField->setPosition((screenXSize - m_textField->getSize().x) / 2, butYPosition - m_textField->getSize().y - 30);
	m_textField->setTextSize(15);
	m_textField->setVerticalScrollbarPolicy(tgui::Scrollbar::Policy::Never);
	m_textField->setMaximumCharacters(30);
	m_guiWindow->add(m_textField);

	//Header
	tgui::Panel::Ptr headerBack = tgui::Panel::create();
	headerBack->setSize(screenXSize, 30);
	headerBack->setRenderer(defaultTheme.getRenderer("PauseHeader"));
	headerBack->getRenderer()->setBorders(tgui::Borders::Outline(1));
	headerBack->getRenderer()->setBorderColor(tgui::Color::Black);
	m_guiWindow->add(headerBack);

	//Header label
	m_commentLabel->setPosition(10, 2);
	m_commentLabel->setTextSize(20);
	m_commentLabel->setText("Server IP...");
	m_commentLabel->setRenderer(defaultTheme.getRenderer("PauseLabel"));
	m_guiWindow->add(m_commentLabel);

	//Center screen...
	//m_guiWindow->setPosition(sf::Vector2f((view.getSize().x - background->getSize().x) / 2, view.getSize().y / 4 - 50));
}

void GuiInputWindow::afterInitialize()
{
}

void GuiInputWindow::validateInput(const std::string & input) const
{

}
