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


void GuiContainer::addWindow(std::shared_ptr<GuiWindow> window) {
	if (window->m_container != nullptr)
		throw new std::exception("Window already has a container parent!");

	m_childWindows.push_back(window);

	window->m_container = this;
	window->m_windowID = ++nextChildID;

	window->onAddedToContainer(m_view);

	m_guiBase.add(window->m_guiWindow);

	window->setEnabled(false);
	window->setVisible(false);
}

void GuiContainer::addShow(std::shared_ptr<GuiWindow> window) {
	addWindow(window);
	window->show();
}

void GuiContainer::addShowDialog(std::shared_ptr<GuiWindow> window) {
	addWindow(window);
	window->showDialog();
}

bool GuiContainer::removeWindow(GuiWindow & window)
{
	return removeWindow(window.m_windowID);
}

bool GuiContainer::removeWindow(int windowID)
{
	std::shared_ptr<GuiWindow> window;

	for (auto itr = m_childWindows.begin(); itr != m_childWindows.end(); ++itr)
	{
		if ((**itr).m_windowID == windowID) {
			window = std::move(*itr);

			m_childWindows.erase(itr);
			m_guiBase.remove(window->m_guiWindow);
			break;
		}
	}

	if (!window)
		return false;

	window->m_windowStatus = WindowStatus::NONE;
	hideWindow(*window);
	window->innerClose();
	return true;
}

void GuiContainer::removeAllWindows()
{
	for (auto& window : m_childWindows) {
		window->innerClose();
	}
}

bool GuiContainer::handleEvent(const sf::Event & event)
{
	if (event.type == sf::Event::Resized) {
		if (m_maintainAspectRatio)
			Graphics::applyResize(m_view, event);
	}

	if (event.type == sf::Event::EventType::KeyReleased && event.key.code == sf::Keyboard::Escape) {
		GuiWindow* window = nullptr;

		//Close foccused window...
		for (auto itr = m_childWindows.rbegin(); itr != m_childWindows.rend(); ++itr)
		{
			if ((**itr).m_windowStatus != WindowStatus::NONE) {
				window = (*itr).get();
				break;
			}
		}

		if (window != nullptr) {
			window->onEscapePress();
			return true;
		}
	}

	return m_guiBase.handleEvent(event);
}

void GuiContainer::setDebug()
{
	for (auto& window : m_childWindows) {
		window->setDebug();
	}
}

bool GuiContainer::showWindowOnTop(GuiWindow & window)
{
	if (hasWindow(window.m_windowID)) {
		window.m_windowStatus = WindowStatus::DIALOG;
		innerShow(window);
		return true;
	}

	return false;
}

bool GuiContainer::showWindow(GuiWindow& window)
{
	if (hasWindow(window.m_windowID)) {
		window.m_windowStatus = WindowStatus::SHOW;
		innerShow(window);
		return true;
	}

	return false;
}

bool GuiContainer::hideWindow(GuiWindow& window)
{
	if (hasWindow(window.m_windowID)) {
		window.m_windowStatus = WindowStatus::NONE;
		window.innerHide();
	}

	bool focusSet = false;
	for (auto itr = m_childWindows.rbegin(); itr != m_childWindows.rend(); ++itr)
	{
		if ((**itr).m_windowStatus == WindowStatus::NONE)
			continue;

		(**itr).setEnabled(true);
		if (!focusSet) {
			(**itr).setFocus(true);
			focusSet = true;
		}

		if ((**itr).m_windowStatus == WindowStatus::DIALOG)
			return true;
	}

	return true;
}

void GuiContainer::innerShow(GuiWindow& window)
{
	bool isDialog = false;
	for (auto itr = m_childWindows.begin(); itr != m_childWindows.end(); ++itr)
	{
		if ((**itr).m_windowID == window.m_windowID) {

			auto window = std::move(*itr);
			m_childWindows.erase(itr);

			isDialog = window->m_windowStatus == WindowStatus::DIALOG;
			m_childWindows.push_back(window);

			break;
		}
	}

	if (isDialog)
	{
		auto itr = m_childWindows.rbegin();
		itr++;

		while (itr != m_childWindows.rend()) {
			(**itr).setEnabled(false);
			itr++;
		}
	}

	window.innerShow();
}

bool GuiContainer::hasWindow(int windowID)
{
	for (auto& itr : m_childWindows) {
		if (itr->m_windowID == windowID)
			return true;
	}

	return false;
}
