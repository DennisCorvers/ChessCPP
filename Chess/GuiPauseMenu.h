#pragma once
#include "GuiWindowBlank.hpp"

class GuiPauseMenu : public GuiWindowBlank
{
private:
	bool m_isClient;

public:
	Event<> OnNewGameEvent;
	Event<> OnSwapColourEvent;
	Event<> OnExitGameEvent;

	GuiPauseMenu(const SharedContext& sharedContext, bool isClient = false);
	virtual ~GuiPauseMenu();

private:
	void initialize();
	void onAddedToContainer(const sf::View& containerView) override;

	void createButtons(std::vector<tgui::Button::Ptr>& buttons);

	void onNewGame();
	void onSwapColour();
	void onExitGame();
};