#pragma once
#include "BaseState.hpp"
#include "StateManager.h"
#include "NetServer.hpp"

class STest : public BaseState
{
private:
	NetServer m_server;

public:
	STest(StateManager& stateManager) :
		BaseState(stateManager), m_server(1000)
	{

	}

	virtual ~STest() {};

	virtual void onCreate() override
	{
		m_server.startListening();
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

