#pragma once
#include "GuiWindowBlank.hpp"

class GuiMainMenu : public GuiWindowBlank
{
public:
	Signal<> OnQuitEvent;
	Signal<> OnSinglePlayerEvent;
	Signal<> OnSandboxEvent;

	Signal<> OnJoinGameEvent;
	Signal<> OnHostGameEvent;

	GuiMainMenu(const SharedContext & sharedContext);
	virtual ~GuiMainMenu();

private:
	void initialize();
	void onAddedToContainer(const sf::View& containerView) override;
};

