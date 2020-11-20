#pragma once
#include "GuiInputWindow.h"
#include "GuiMessageBox.hpp"

class GuiHostStart : public GuiInputWindow
{
public:
	Signal<unsigned short> OnConfirm;

	GuiHostStart(const SharedContext & sharedContext) :
		GuiInputWindow(sharedContext)
	{
		m_title->setText("Enter Port Number");
	}

	virtual ~GuiHostStart() { }

	static std::shared_ptr<GuiHostStart> create(const SharedContext& sharedContext) {
		return std::make_shared<GuiHostStart>(sharedContext);
	}

private:
	void validateInput(const std::string & input) override {
		int port = 0;
		try {
			port = std::stoi(input);
		}
		catch (const std::out_of_range& ex) { UNUSED(ex); }
		catch (const std::invalid_argument& ex) { UNUSED(ex); }

		if (port < 1 || port > 65535) {

			auto win = GuiMessageBox::create(m_sharedContext);
			addChildWindow(win);
			win->show("Port needs to be between 1 and 65535.", "Error", MessageBoxButtons::OK);
		}
		else {
			OnConfirm(static_cast<unsigned short>(port));
			close();
		}
	}
};