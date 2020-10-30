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

class GuiMessageBox : public GuiWindow {
public:
	Signal<const MessageBoxResult&> OnMessageBoxResult;

private:
	static constexpr unsigned int MAX_X_SIZE = 800;
	static constexpr unsigned int MAX_Y_SIZE = 500;
	static constexpr unsigned int MIN_X_SIZE = 250;
	static constexpr unsigned int MIN_Y_SIZE = 120;

	static constexpr unsigned int TEXT_PAD_LEFT = 20;
	static constexpr unsigned int TEXT_PAD_Y = 20;
	static constexpr unsigned int BUTTON_PAD = 20;
	static constexpr unsigned int BUTTON_LEFT = 30;


	static constexpr int MAX_BUTTONS = 3;
	using Button = tgui::Button::Ptr;
	using Label = tgui::Label::Ptr;

	MessageBoxButtons m_setButtons;
	MessageBoxResult m_result;
	Button m_buttons[MAX_BUTTONS];
	Label m_label;
	sf::Vector2f m_viewSize;

public:

	GuiMessageBox(const SharedContext& sharedContext);

	virtual ~GuiMessageBox();

	static std::shared_ptr<GuiMessageBox> create(const SharedContext& sharedContext) {
		return std::make_shared<GuiMessageBox>(sharedContext);
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

	void setGuiWindow();
	void setButtons();
	void placeLabel();

	void onAddedToContainer(const sf::View& containerView) override;

	unsigned char getButtonCount() const;
	const std::vector<std::string> getButtonText() const;

	void buttonClicked(int buttonID);
};

