#pragma once
#include "GuiWindowBlank.hpp"

class GuiInputWindow : public GuiWindowBlank
{
protected:
	tgui::Button::Ptr m_confirmButton;
	tgui::Button::Ptr m_cancelButton;
	tgui::TextBox::Ptr m_textField;
	tgui::Label::Ptr m_title;
	tgui::Panel::Ptr m_background;

public:
	GuiInputWindow(const SharedContext & sharedContext);

	virtual ~GuiInputWindow();

	void setText(const std::string& text);
	const std::string getText() const;

protected:
	void onAddedToContainer(const sf::View& containerView) override;
	virtual void validateInput(const std::string & input) = 0;

private:
	void initialize();

	void onConfirmClick();

	void onEscapePress() override {
		close();
	}
};

