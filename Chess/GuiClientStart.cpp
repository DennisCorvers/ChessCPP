#include "pch.h"
#include "GuiClientStart.hpp"

GuiClientStart::GuiClientStart(const SharedContext & sharedContext) :
	GuiInputWindow(sharedContext)
{
	m_title->setText("Connect to game");
}

GuiClientStart::~GuiClientStart()
{ }

std::shared_ptr<GuiClientStart> GuiClientStart::create(const SharedContext & sharedContext) {
	return std::make_shared<GuiClientStart>(sharedContext);
}

void GuiClientStart::validateInput(const std::string & input)
{
	if (input.length() < 1) {
		showMessage("Address cannot be empty.");
		return;
	}

	int port = 0;
	bool hasPort = false;
	std::string s_port;
	std::string s_ip;

	int index = static_cast<int>(input.size() - 1);

	for (; index >= 0; index--) {
		if (input[index] == ':') {
			hasPort = true;
			s_port = input.substr(index + 1);
			s_ip = input.substr(0, index);
			break;
		}
	}

	if (hasPort) {
		if (s_ip.length() < 1) {
			showMessage("No valid address found.");
			return;
		}
	}
	else {
		s_ip = input;
	}

	if (s_port.length() > 0) {
		try { port = std::stoi(s_port); }
		catch (const std::out_of_range& ex) { UNUSED(ex); }
		catch (const std::invalid_argument& ex) { UNUSED(ex); }

		if (port < 1 || port > 65535) {
			showMessage("Port needs to be between 1 and 65535.");
			return;
		}
	}

	OnConfirm(s_ip, static_cast<unsigned short>(port));
	close();
}

void GuiClientStart::showMessage(const std::string & message)
{
	auto win = GuiMessageBox::create(m_sharedContext);
	addChildWindow(win);
	win->show(message, "Error", MessageBoxButtons::OK);
}
