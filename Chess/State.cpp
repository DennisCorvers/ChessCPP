#include "State.h"
#include "SFML/Graphics.hpp"


State::State()
{
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
