#include "pch.h"
#include "GuiContainer.hpp"

GuiContainer::GuiContainer(const SharedContext& sharedContext) :
	m_sharedContext(sharedContext),
	m_guiBase(*sharedContext.window),
	m_maintainAspectRatio(true),
	nextChildID(-1)
{
	const sf::Vector2u windowSize = sharedContext.window->getSize();
	m_view.setSize(sf::Vector2f(windowSize.x, windowSize.y));
	m_view.setCenter(windowSize.x / 2, windowSize.y / 2);
}

GuiContainer::~GuiContainer()
{
	removeAllWindows();
}

void GuiContainer::addWindow(std::shared_ptr<GuiWindow> window, bool showOnCreate) {
	if (window->m_windowID != -1)
		throw new std::exception("Window was already added to container.");

	m_childWindows.emplace(++window->m_windowID, window);
	window->onInitialize(m_sharedContext);
	if (!showOnCreate)
		window->hide();

	m_guiBase.add(window->m_guiWindow);
}

void GuiContainer::removeWindow(const std::shared_ptr<GuiWindow>& window)
{
	window->onDispose(m_sharedContext);
	m_guiBase.remove(window->m_guiWindow);
}

void GuiContainer::removeAllWindows()
{
	for (auto window : m_childWindows) {
		window.second->onDispose(m_sharedContext);
	}

	m_guiBase.removeAllWidgets();
	m_childWindows.empty();
}

void GuiContainer::setDebug()
{
	for (auto window : m_childWindows) {
		window.second->setDebug();
	}
}
