#include "pch.h"
#include "GuiBase.h"


GuiBase::GuiBase(GuiManager& guiManager)
	: m_guiManager(&guiManager)
{
	m_guiManager->registerGui(this);
}


GuiBase::~GuiBase()
{
	//Unsubscribe Events
}

void GuiBase::close() {
	m_guiState = GuiState::HIDDEN;
	m_guiManager->disposeGui(m_id);
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
	onDeactivate();
}
