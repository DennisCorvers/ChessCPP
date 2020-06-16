#include "State.h"
#include "EventManager.h"
#include "SFML/Graphics.hpp"


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
