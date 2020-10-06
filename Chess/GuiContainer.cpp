#include "pch.h"
#include "GuiContainer.hpp"
#include "GuiWindow.hpp"

GuiContainer::GuiContainer(sf::RenderWindow& target) :
	m_guiBase(target),
	m_maintainAspectRatio(true),
	nextChildID(-1)
{
	const sf::Vector2u windowSize = target.getSize();
	m_view.setSize(sf::Vector2f(windowSize.x, windowSize.y));
	m_view.setCenter(windowSize.x / 2, windowSize.y / 2);
}

GuiContainer::~GuiContainer()
{
	removeAllWindows();
}

void GuiContainer::addWindow(std::shared_ptr<GuiWindow> window, bool showOnCreate) {
	if (window->m_windowID != -1)
		throw new std::exception("Window already has a container parent!");

	m_childWindows.push_back(window);

	window->m_container = this;
	window->m_windowID = ++nextChildID;

	window->onInitialize();

	m_guiBase.add(window->m_guiWindow);

	if (!showOnCreate)
		window->hide();
}

void GuiContainer::removeWindow(const std::shared_ptr<GuiWindow>& window)
{
	removeWindow(window->m_windowID);
}

void GuiContainer::removeWindow(int windowID)
{
	for (auto itr = m_childWindows.begin(); itr != m_childWindows.end(); ++itr)
	{
		if ((**itr).m_windowID == windowID) {
			std::shared_ptr<GuiWindow> window = std::move(*itr);


			m_childWindows.erase(itr);

			window->m_container = nullptr;
			window->m_windowID = -1;
			window->onDispose();
			m_guiBase.remove(window->m_guiWindow);
			return;
		}
	}
}

void GuiContainer::removeAllWindows()
{
	for (auto& window : m_childWindows) {
		window->onClose();
	}
}

void GuiContainer::setDebug()
{
	for (auto& window : m_childWindows) {
		window->setDebug();
	}
}
