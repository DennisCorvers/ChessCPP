#include "pch.h"
#include "GuiInfoBox.h"

GuiInfoBox::GuiInfoBox(const SharedContext & sharedContext) :
	GuiWindowBlank(sharedContext),
	m_isDisplayed(false)
{
	m_title = tgui::Label::create("Title");
	m_background = tgui::Panel::create();
	m_canvas = tgui::Canvas::create();

	initialize();
}

GuiInfoBox::~GuiInfoBox()
{
	m_sharedContext.fontManager->releaseResource(AssetNames::f_opensans_reg);
	m_sharedContext.themeManager->releaseResource(AssetNames::theme_default);
	m_sharedContext.textureManager->releaseResource(AssetNames::t_loadspinner);
}

std::shared_ptr<GuiInfoBox> GuiInfoBox::create(const SharedContext & sharedContext) {
	return std::make_shared<GuiInfoBox>(sharedContext);
}

void GuiInfoBox::setText(const std::string & text) {
	m_title->setText(text);
}

void GuiInfoBox::initialize()
{
	FontManager* fontManager = m_sharedContext.fontManager;
	auto& openSans = *fontManager->requireAndGet(AssetNames::f_opensans_reg);
	FontManager::setSmoothing(openSans, 20, false);

	ThemeManager* themeManager = m_sharedContext.themeManager;
	auto& defaultTheme = *themeManager->requireAndGet(AssetNames::theme_default);

	SoundManager* soundManager = m_sharedContext.soundManager;
	const sf::View& view = m_sharedContext.window->getView();

	TextureManager* textureManager = m_sharedContext.textureManager;
	auto& loadSpinner = *textureManager->requireAndGet(AssetNames::t_loadspinner);

	m_guiWindow->setInheritedFont(openSans);

	//Background
	m_background->setSize(sf::Vector2f(SCREEN_X_SIZE, SCREEN_Y_SIZE));
	m_background->setRenderer(defaultTheme.getRenderer("PauseBackground"));
	m_background->getRenderer()->setBorders(tgui::Borders::Outline(1));
	m_background->getRenderer()->setBorderColor(tgui::Color::Black);
	auto col = m_background->getRenderer()->getBackgroundColor();
	m_background->getRenderer()->setBackgroundColor(tgui::Color(col.getRed(), col.getGreen(), col.getBlue(), 200));
	m_guiWindow->add(m_background);

	//Header
	tgui::Panel::Ptr headerBack = tgui::Panel::create();
	headerBack->setSize(SCREEN_X_SIZE, 30);
	headerBack->setRenderer(defaultTheme.getRenderer("PauseHeader"));
	headerBack->getRenderer()->setBorders(tgui::Borders::Outline(1));
	headerBack->getRenderer()->setBorderColor(tgui::Color::Black);
	m_guiWindow->add(headerBack);

	//Header label
	m_title->setPosition(10, 2);
	m_title->setTextSize(20);
	m_title->setRenderer(defaultTheme.getRenderer("PauseLabel"));
	m_guiWindow->add(m_title);

	//Load Spinner
	float spinnerSize = 110;
	m_canvas->setSize(spinnerSize, spinnerSize);
	m_canvas->setPosition((SCREEN_X_SIZE - spinnerSize) / 2, (SCREEN_Y_SIZE + 30 - spinnerSize) / 2);
	m_loadSpinner.setTexture(loadSpinner, true);
	m_loadSpinner.setColor(sf::Color(255, 255, 255, 200));
	sf::Vector2f scale = sf::Vector2f(
		spinnerSize / m_loadSpinner.getGlobalBounds().width,
		spinnerSize / m_loadSpinner.getGlobalBounds().height
	);
	m_loadSpinner.setScale(scale);

	m_spinnerCenter = sf::Vector2f(
		m_loadSpinner.getPosition().x + spinnerSize / 2,
		m_loadSpinner.getPosition().y + spinnerSize / 2
	);
	m_guiWindow->add(m_canvas);

	m_guiWindow->setSize(m_background->getSize());
}

void GuiInfoBox::onAddedToContainer(const sf::View & containerView)
{
	m_guiWindow->setPosition(sf::Vector2f(
		(containerView.getSize().x - m_guiWindow->getSize().x) / 2,
		(containerView.getSize().y - m_guiWindow->getSize().y) / 2
	));
}

void GuiInfoBox::onHide() {
	m_isDisplayed = false;
}

void GuiInfoBox::onShow() {
	m_isDisplayed = true;
}

void GuiInfoBox::onClose() {
	m_isDisplayed = false;
}
