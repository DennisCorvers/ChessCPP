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

	GuiPauseMenu(bool isClient = false);
	virtual ~GuiPauseMenu();

	void afterInitialize(const SharedContext& sharedContext) override;
	void onDispose(const SharedContext& sharedContext) override;

	void createButtons(std::vector<tgui::Button::Ptr>& buttons);

protected:
	void onNewGame();
	void onSwapColour();
	void onExitGame();
};