#pragma once
#include "GuiWindowBlank.hpp"

class GuiMainMenu : public GuiWindowBlank
{
public:
	Event<> OnQuitEvent;
	Event<> OnSinglePlayerEvent;
	Event<> OnSandboxEvent;

	GuiMainMenu();
	virtual ~GuiMainMenu();

	void afterInitialize(const SharedContext& sharedContext) override;
	void onDispose(const SharedContext & sharedContext) override;

private:
	void onJoinGamePressed();
	void onHostGamePressed();
};

