#include "pch.h"
#include "SMainMenu.h"
#include "GuiMainMenu.h"
#include "GuiContainer.hpp"
#include "StateManager.h"
#include "GuiHostStart.hpp"
#include "GuiBotInput.hpp"
#include "GuiClientStart.hpp"


SMainMenu::SMainMenu(StateManager & stateManager) :
	BaseState(stateManager)
{
	m_gui = std::make_unique<GuiContainer>(*stateManager.getContext().window);
	m_guiMainMenu = std::make_shared<GuiMainMenu>(stateManager.getContext());
	m_gui->addWindow(m_guiMainMenu);
	m_guiMainMenu->showDialog();

	m_guiMainMenu->OnQuitEvent.connect(&SMainMenu::onQuitPressed, this);
	m_guiMainMenu->OnSandboxEvent.connect(&SMainMenu::onSandboxPressed, this);
	m_guiMainMenu->OnSinglePlayerEvent.connect(&SMainMenu::onSinglePlayerPressed, this);

	m_guiMainMenu->OnHostGameEvent.connect(&SMainMenu::onHostGamePressed, this);
	m_guiMainMenu->OnJoinGameEvent.connect(&SMainMenu::onJoinGamePressed, this);
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
	auto joinGameUI = GuiClientStart::create(m_stateManager->getContext());
	joinGameUI->setText(m_stateManager->getContext().netSettings.toString());
	joinGameUI->OnConfirm.connect(&SMainMenu::onClientConnect, this);

	m_gui->addShowDialog(joinGameUI);
}

void SMainMenu::onHostGamePressed()
{
	auto hostGameUI = GuiHostStart::create(m_stateManager->getContext());
	hostGameUI->setText(std::to_string(m_stateManager->getContext().netSettings.getPort()));
	hostGameUI->OnConfirm.connect(&SMainMenu::onHostPort, this);

	m_gui->addShowDialog(hostGameUI);
}

void SMainMenu::onSandboxPressed() {
	m_stateManager->switchState(States::Sandbox);
	m_stateManager->removeState(States::MainMenu);
}

void SMainMenu::onHostPort(unsigned short port)
{
	m_stateManager->getContext().netSettings.setPort(port);
	m_stateManager->switchState(States::MultiplayerHost);
	m_stateManager->removeState(States::MainMenu);
}

void SMainMenu::onClientConnect(const std::string& ip, unsigned short port)
{
	auto& netSettings = m_stateManager->getContext().netSettings;
	netSettings.IpAddress = ip;
	netSettings.setPort(port);

	m_stateManager->switchState(States::MultiplayerClient);
	m_stateManager->removeState(States::MainMenu);
}
