#pragma once
#include "BaseState.hpp"



class STest : public BaseState
{
private:


public:
	STest(StateManager& stateManager) :
		BaseState(stateManager)
	{ }

	virtual ~STest() {};

	virtual void onCreate() override
	{

	};


	virtual void onDestroy() override
	{
	};

	virtual void activate() override
	{
	};

	virtual void deactivate() override
	{
	};

	virtual void render() override
	{

	};

	virtual bool update(float deltaTime) override
	{
		return false;
	};

	virtual bool handleEvent(const sf::Event & event) override
	{
		return false;
	};

};

