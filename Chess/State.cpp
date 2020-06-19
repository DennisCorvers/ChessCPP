#include "pch.h"
#include "State.h"
#include "EventManager.h"

State::State(StateData* data)
{
	stateData = data;
}


State::~State()
{
}

const bool State::getQuit() const
{
	return false;
}

void State::endState()
{

}
