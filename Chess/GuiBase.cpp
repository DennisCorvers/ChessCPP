#include "pch.h"
#include "GuiBase.h"


GuiBase::GuiBase(GuiManager& guiManager)
	: m_guiManager(&guiManager)
{
	m_guiManager->registerGui(this);
}


GuiBase::~GuiBase()
{
	m_guiState = GuiState::HIDDEN;
	if (m_guiManager)
		m_guiManager->disposeGui(m_id);
	//Unsubscribe Events
}

void GuiBase::show() {
	m_guiState = GuiState::SHOW;
	m_guiManager->showGui(m_id);
}

void GuiBase::showDialog() {
	m_guiState = GuiState::SHOW_ONTOP;
	m_guiManager->showGui(m_id);
}

void GuiBase::hide() {
	m_guiState = GuiState::HIDDEN;
	onHide();
}
