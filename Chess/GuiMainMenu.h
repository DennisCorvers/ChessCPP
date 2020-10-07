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
	void initialize();
	void onAddedToContainer(const sf::View& containerView) override;

	void onJoinGamePressed();
	void onHostGamePressed();
};

