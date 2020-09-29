#pragma once
#include "GuiWindowBlank.hpp"

class GuiPauseMenu : public GuiWindowBlank
{
public:
	GuiPauseMenu();
	virtual ~GuiPauseMenu();

	void onInitialize(const SharedContext& sharedContext) override;
	void onDispose(const SharedContext& sharedContext) override;

	void createButtons(std::vector<tgui::Button::Ptr>& buttons);
};

