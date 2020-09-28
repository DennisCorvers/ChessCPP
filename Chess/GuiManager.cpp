#include "pch.h"
#include "GuiManager.h"
#include "GuiBase.hpp"

GuiManager::GuiManager()
	: m_nextGuiID(0)
{
}

GuiManager::~GuiManager()
{
	for (auto itr = m_guis.begin(); itr != m_guis.end(); ++itr) {
		itr->second->onDispose();
		delete itr->second;
	}
}

void GuiManager::update(float deltaTime) const
{
	for (auto itr = m_guis.rbegin(); itr != m_guis.rend(); ++itr) {
		if (itr->second->m_guiState != GuiState::HIDDEN)
			itr->second->update(deltaTime);
	}
}

void GuiManager::render() const
{
	for (auto itr = m_guis.begin(); itr != m_guis.end(); ++itr) {
		if (itr->second->m_guiState != GuiState::HIDDEN)
			itr->second->render();
	}
}

void GuiManager::handleEvent(const sf::Event & event) const
{
	for (auto itr = m_guis.rbegin(); itr != m_guis.rend(); ++itr) {
		auto gui = itr->second;
		if (gui->m_guiState != GuiState::HIDDEN) {
			if (gui->handleEvent(event) || gui->m_guiState == GuiState::SHOW_ONTOP)
				return;
		}
	}
}

void GuiManager::lateUpdate()
{
	while (!m_toAdd.empty()) {
		auto gui = m_toAdd.front();
		m_guis.emplace_back(gui->m_id, gui);
		m_toAdd.pop();
	}

	while (!m_toShow.empty()) {
		switchInternal(m_toShow.front());
		m_toShow.pop();
	}

	while (!m_toRemove.empty()) {
		removeInternal(m_toRemove.front());
		m_toRemove.pop();
	}
}

void GuiManager::clearGuis()
{
	while (!m_toAdd.empty())
		m_toAdd.pop();

	while (!m_toShow.empty())
		m_toShow.pop();

	for (auto& it : m_guis)
		disposeGui(it.first);
}

void GuiManager::registerGui(GuiBase* guiBase)
{
	guiBase->m_id = m_nextGuiID++;
	m_toAdd.push(guiBase);
}

void GuiManager::disposeGui(const int id) {
	m_toRemove.push(id);
}

void GuiManager::showGui(const int id) {
	m_toShow.push(id);
}

void GuiManager::switchInternal(const int id)
{
	for (auto itr = m_guis.begin(); itr != m_guis.end(); ++itr)
	{
		if (itr->first == id) {
			auto gui = itr->second;
			m_guis.erase(itr);

			m_guis.emplace_back(id, gui);

			gui->onShow();
			return;
		}
	}

	throw new std::exception("Trying to show a disposed Gui.");
}

void GuiManager::removeInternal(const int id)
{
	for (auto it = m_guis.begin(); it != m_guis.end(); ++it) {
		if (it->first != id)
			continue;

		it->second->onDispose();
		m_guis.erase(it);

		delete it->second;

		return;
	}
}


