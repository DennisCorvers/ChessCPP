#include "pch.h"
#include "GuiContainer.hpp"
#include "GuiWindow.hpp"

GuiContainer::GuiContainer(sf::RenderWindow& target) :
	m_guiBase(target),
	m_maintainAspectRatio(true),
	nextChildID(-1)
{
	sf::Vector2u windowSize(Graphics::DEFAULT_SIZE.x, Graphics::DEFAULT_SIZE.y);
	m_view.setSize(sf::Vector2f(windowSize.x, windowSize.y));
	m_view.setCenter(windowSize.x / 2, windowSize.y / 2);
	Graphics::applyResize(m_view, target.getSize().x, target.getSize().y);
}

GuiContainer::~GuiContainer()
{
	removeAllWindows();
}

void GuiContainer::addWindow(std::shared_ptr<GuiWindow> window, bool showOnCreate) {
	if (window->m_container != nullptr)
		throw new std::exception("Window already has a container parent!");

	m_childWindows.push_back(window);

	window->m_container = this;
	window->m_windowID = ++nextChildID;

	window->onAddedToContainer(m_view);

	m_guiBase.add(window->m_guiWindow);

	if (showOnCreate)
		window->show();
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
			window->dispose();

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

void GuiContainer::showWindow(GuiWindow& window)
{
	bool isDialog = false;
	for (auto itr = m_childWindows.begin(); itr != m_childWindows.end(); ++itr)
	{
		if ((**itr).m_windowID == window.m_windowID) {

			auto window = std::move(*itr);
			m_childWindows.erase(itr);

			m_childWindows.push_back(window);

			isDialog = (**itr).m_windowStatus == WindowStatus::DIALOG;
			break;
		}
	}

	if (isDialog)
	{
		auto itr = m_childWindows.rbegin();
		itr++;

		while (itr != m_childWindows.rend()) {
			(**itr).setEnabled(false);
		}
	}
}

void GuiContainer::hideWindow(GuiWindow& window)
{
	bool focussedWindow = false;
	for (auto itr = m_childWindows.rbegin(); itr != m_childWindows.rend(); ++itr)
	{
		if ((**itr).m_windowStatus == WindowStatus::NONE)
			continue;

		(**itr).setEnabled(true);
		if (!focussedWindow) {
			(**itr).setEnabled(true);
			focussedWindow = true;
		}

		if ((**itr).m_windowStatus == WindowStatus::DIALOG)
			return;
	}
}
