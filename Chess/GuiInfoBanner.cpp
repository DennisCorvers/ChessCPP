#include "pch.h"
#include "GuiInfoBanner.h"

GuiInfoBanner::GuiInfoBanner(const SharedContext & sharedContext) :
	GuiWindowBlank(sharedContext)
{
	m_text = tgui::Label::create();
	m_background = tgui::Panel::create();

	initialize();
}

GuiInfoBanner::~GuiInfoBanner()
{
	m_sharedContext.fontManager->releaseResource(AssetNames::f_opensans_reg);
	m_sharedContext.themeManager->releaseResource(AssetNames::theme_default);
}

std::shared_ptr<GuiInfoBanner> GuiInfoBanner::create(const SharedContext& sharedContext) {
	return std::make_shared<GuiInfoBanner>(sharedContext);
}

void GuiInfoBanner::setText(const std::string & text) {
	m_text->setText(text);

	const auto& mySize = m_guiWindow->getSize();
	int xOffset = (mySize.x - m_text->getSize().x) / 2;
	xOffset = Math::max<int>(xOffset, 20);

	//Center Y
	int yOffset = (mySize.y - m_text->getSize().y) / 2;
	m_text->setPosition(xOffset, yOffset);
}

void GuiInfoBanner::initialize()
{
	//TODO Update layout to be in the middle of the screen?
	FontManager* fontManager = m_sharedContext.fontManager;
	sf::Font& openSans = *fontManager->requireAndGet(AssetNames::f_opensans_reg);
	FontManager::setSmoothing(openSans, 20, false);

	ThemeManager* themeManager = m_sharedContext.themeManager;
	tgui::Theme& defaultTheme = *themeManager->requireAndGet(AssetNames::theme_default);

	const sf::View& view = m_sharedContext.window->getView();

	m_guiWindow->setInheritedFont(openSans);

	int screenXSize = 500;
	int screenYSize = 30;

	//Background
	m_background->setSize(sf::Vector2f(screenXSize, screenYSize));
	m_background->setRenderer(defaultTheme.getRenderer("PauseBackground"));
	m_background->getRenderer()->setBorders(tgui::Borders::Outline(1));
	m_background->getRenderer()->setBorderColor(tgui::Color::Black);
	m_guiWindow->add(m_background);

	//Text
	m_text->setPosition((screenXSize - m_text->getSize().x) / 2, screenYSize - m_text->getSize().y);
	m_text->setRenderer(defaultTheme.getRenderer("PauseLabel"));
	m_text->getRenderer()->setTextColor(tgui::Color::Black);
	m_text->setTextSize(15);
	m_guiWindow->add(m_text);


	m_guiWindow->setSize(m_background->getSize());
}

void GuiInfoBanner::onAddedToContainer(const sf::View & containerView)
{
	float top = Math::max<float>(20, containerView.getSize().y / 30);
	m_guiWindow->setPosition(sf::Vector2f(
		(containerView.getSize().x - m_guiWindow->getSize().x) / 2, top));
}