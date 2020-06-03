#include "State.h"
#include "SFML/Graphics.hpp"


State::State(StateData* data)
{
	stateData = data;
	window = data->window;
}


State::~State()
{
}

const bool & State::getQuit() const
{
	return false;
}

const bool State::getKeytime()
{
	return false;
}

void State::endState()
{

}
