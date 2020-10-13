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
public:
	Signal<const MessageBox&> OnMessageBoxResult;

private:
	static constexpr unsigned int MAX_X_SIZE = 500;
	static constexpr unsigned int MAX_Y_SIZE = 200;
	static constexpr unsigned int MIN_X_SIZE = 20;
	static constexpr unsigned int MIN_Y_SIZE = 20;

	static constexpr unsigned int TEXT_PAD_LEFT = 20;
	static constexpr unsigned int TEXT_PAD_TOP = 20;
	static constexpr unsigned int TEXT_PAD_BOTTOM = 20;
	static constexpr unsigned int BUTTON_PAD = 20;

	static constexpr int MAX_BUTTONS = 3;
	using Button = tgui::Button::Ptr;
	using Label = tgui::Label::Ptr;

	MessageBoxButtons m_setButtons;
	MessageBoxResult m_result;
	Button m_buttons[MAX_BUTTONS];
	Label m_label;

public:

	MessageBox(const SharedContext& sharedContext);

	virtual ~MessageBox();

	static std::shared_ptr<MessageBox> create(const SharedContext& sharedContext) {
		return std::make_shared<MessageBox>(sharedContext);
	}

	void show(const std::string& text);
	void show(const std::string& text, const std::string& title);
	void show(const std::string& text, const std::string& title, MessageBoxButtons buttons);

	MessageBoxResult getMessageBoxResult() const;

	static sf::Vector2f getMaxSize() {
		return sf::Vector2f(MAX_X_SIZE, MAX_Y_SIZE);
	}

private:
	void initialize();

	void setButtons(MessageBoxButtons buttons);
	void placeLabel();

	unsigned char getButtonCount() const;
	const std::vector<std::string> getButtonText() const;

	void buttonClicked(int buttonID);
};

