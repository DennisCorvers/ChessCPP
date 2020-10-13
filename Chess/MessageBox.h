#pragma once
#include "GuiWindow.hpp"

enum struct MessageBoxButtons {
	OK = 0,
	OKCancel = 1,
	AbortRetryIgnore = 2,
	YesNoCancel = 3,
	YesNo = 4,
	RetryCancel = 5
};

enum struct MessageBoxResult {
	None = 0,
	OK = 1,
	Cancel = 2,
	Abort = 3,
	Retry = 4,
	Ignore = 5,
	Yes = 6,
	No = 7
};


class MessageBox : public GuiWindow {
private:
	using string = const std::string;
	using Button = tgui::Button::Ptr;
	using Label = tgui::Label::Ptr;

	MessageBoxResult m_result;
	Button m_buttons[3];
	Label m_label;

public:
	MessageBox(const SharedContext& sharedContext);

	virtual ~MessageBox();


	void show(string& text);
	void show(string& text, string& title);
	void show(string& text, string& title, MessageBoxButtons buttons);

	MessageBoxResult getMessageBoxResult() const;

private:
	void initialize();
	void setButtons(MessageBoxButtons buttons);

	void buttonClicked(int buttonID);
};

