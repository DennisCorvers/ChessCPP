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

void GuiContainer::addWindow(std::shared_ptr<GuiWindow> window) {
	if (window->m_windowID != -1)
		throw new std::exception("Window was already added to container.");

	m_childWindows.emplace(++window->m_windowID, window);
	window->onInitialize(m_sharedContext);
	m_guiBase.add(window->m_guiWindow);
}

void GuiContainer::removeWindow(const std::shared_ptr<GuiWindow>& window)
{
	window->onDispose(m_sharedContext);
}

void GuiContainer::removeAllWindows()
{
	for (auto window : m_childWindows) {
		removeWindow(window.second);
	}

	m_childWindows.empty();
}
