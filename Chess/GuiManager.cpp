#include "pch.h"
#include "GuiManager.h"
#include "GuiBase.h"

GuiManager::GuiManager()
	: m_nextGuiID(0)
{
}


GuiManager::~GuiManager()
{
	for (auto itr = m_guis.begin(); itr != m_guis.end(); ++itr) {
		itr->second->onDestroy();
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

void GuiManager::registerGui(GuiBase* guiBase)
{
	guiBase->m_id = m_nextGuiID++;
	m_toAdd.push(guiBase);
}

void GuiManager::switchInternal(const int id)
{
	for (auto itr = m_guis.begin(); itr != m_guis.end(); ++itr)
	{
		if (itr->first == id) {
			auto gui = itr->second;
			m_guis.erase(itr);

			m_guis.emplace_back(id, gui);

			gui->onActivate();
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

		it->second->onDestroy();
		m_guis.erase(it);

		return;
	}
}
