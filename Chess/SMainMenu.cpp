#include "pch.h"
#include "SMainMenu.h"
#include "Event.h"

SMainMenu::SMainMenu(StateManager & stateManager)
	:BaseState(stateManager)
{}

SMainMenu::~SMainMenu()
{}

void SMainMenu::onCreate()
{

}

void SMainMenu::onDestroy()
{
}

void SMainMenu::activate()
{
}

void SMainMenu::deactivate()
{
}

void SMainMenu::render()
{
}

bool SMainMenu::update(float deltaTime)
{

	return false;
}

bool SMainMenu::handleEvent(const sf::Event & event)
{
	return false;
}
