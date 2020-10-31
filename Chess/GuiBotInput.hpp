#pragma once
#include "GuiInputWindow.h"
#include "GuiMessageBox.hpp"

class GuiBotInput : public GuiInputWindow
{
public:
	Signal<int> OnConfirm;

	GuiBotInput(const SharedContext & sharedContext) :
		GuiInputWindow(sharedContext)
	{
		m_title->setText("Bot Level 0 - 20");
	}

	virtual ~GuiBotInput() { }

	static std::shared_ptr<GuiBotInput> create(const SharedContext& sharedContext) {
		return std::make_shared<GuiBotInput>(sharedContext);
	}

private:

	virtual void validateInput(const std::string & input) override {
		int botLevel = -1;
		try {
			botLevel = std::stoi(input);
		}
		catch (const std::out_of_range& ex) { UNUSED(ex); }
		catch (const std::invalid_argument& ex) { UNUSED(ex); }

		if (botLevel < 0 || botLevel > 20) {

			auto win = GuiMessageBox::create(m_sharedContext);
			addChildWindow(win);
			win->show("Bot level needs to be between 0 and 20.", "Error", MessageBoxButtons::OK);
		}
		else {
			OnConfirm(botLevel);
			hide();
		}
	}

	void onEscapePress() override {
		hide();
	}
};