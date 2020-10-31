#pragma once
#include "GuiInputWindow.h"
#include "GuiMessageBox.hpp"

class GuiJoinGame : public GuiInputWindow
{
public:
	Signal<> OnConfirm; //Push IP address or whatever SFML uses to connect remotely...

	GuiJoinGame(const SharedContext & sharedContext) :
		GuiInputWindow(sharedContext)
	{

	}

	virtual ~GuiJoinGame() { }

	static std::shared_ptr<GuiJoinGame> create(const SharedContext& sharedContext) {
		return std::make_shared<GuiJoinGame>(sharedContext);
	}

private:

	virtual void validateInput(const std::string & input) override {

	}

	void onEscapePress() override {
		hide();
	}
};