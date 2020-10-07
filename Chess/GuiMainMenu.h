#pragma once
#include "GuiWindowBlank.hpp"

class GuiMainMenu : public GuiWindowBlank
{
public:
	Event<> OnQuitEvent;
	Event<> OnSinglePlayerEvent;
	Event<> OnSandboxEvent;

	GuiMainMenu(const SharedContext & sharedContext);
	virtual ~GuiMainMenu();

private:
	void afterInitialize() override;

	void onJoinGamePressed();
	void onHostGamePressed();
};

