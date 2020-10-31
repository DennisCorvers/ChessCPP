#pragma once
#include "GuiInputWindow.h"
#include "GuiMessageBox.hpp"

class GuiHostGame : public GuiInputWindow
{
public:
	Signal<unsigned short> OnConfirm;

	GuiHostGame(const SharedContext & sharedContext) :
		GuiInputWindow(sharedContext)
	{

	}

	virtual ~GuiHostGame() { }

	static std::shared_ptr<GuiHostGame> create(const SharedContext& sharedContext) {
		return std::make_shared<GuiHostGame>(sharedContext);
	}

private:

	virtual void validateInput(const std::string & input) override {

	}

	void onEscapePress() override {
		hide();
	}
};