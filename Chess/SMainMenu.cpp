#include "pch.h"
#include "SMainMenu.h"
#include "GuiMainMenu.h"
#include "GuiContainer.hpp"
#include "StateManager.h"

SMainMenu::SMainMenu(StateManager & stateManager) :
	BaseState(stateManager)
{
	m_gui = std::make_unique<GuiContainer>(*stateManager.getContext().window);
	m_guiMainMenu = std::make_shared<GuiMainMenu>(stateManager.getContext());
	m_gui->addWindow(m_guiMainMenu);
	m_guiMainMenu->showDialog();

	m_conContainer.add(m_guiMainMenu->OnQuitEvent.connect(&SMainMenu::onQuitPressed, this));
	m_conContainer.add(m_guiMainMenu->OnSandboxEvent.connect(&SMainMenu::onSandboxPressed, this));
	m_conContainer.add(m_guiMainMenu->OnSinglePlayerEvent.connect(&SMainMenu::onSinglePlayerPressed, this));
}

SMainMenu::~SMainMenu()
{}

void SMainMenu::onCreate()
{
	m_guiMainMenu->show();
}

void SMainMenu::onDestroy() {}

void SMainMenu::activate() {}

void SMainMenu::deactivate() {}

void SMainMenu::render() {
	m_gui->render();
}

bool SMainMenu::update(float deltaTime) {
	return false;
}

bool SMainMenu::handleEvent(const sf::Event & event)
{
	m_gui->handleEvent(event);

	return false;
}

void SMainMenu::onQuitPressed() {
	m_stateManager->clearStates();
}

void SMainMenu::onSinglePlayerPressed()
{
	m_stateManager->switchState(States::SinglePlayer);
	m_stateManager->removeState(States::MainMenu);
}

void SMainMenu::onJoinGamePressed()
{
}

void SMainMenu::onHostGamePressed()
{
}

void SMainMenu::onSandboxPressed() {
	m_stateManager->switchState(States::Sandbox);
	m_stateManager->removeState(States::MainMenu);
}
