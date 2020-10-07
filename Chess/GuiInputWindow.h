#pragma once
#include "GuiWindowBlank.hpp"

class GuiInputWindow : public GuiWindowBlank
{
protected:
	tgui::Button::Ptr m_confirmButton;
	tgui::Button::Ptr m_cancelButton;
	tgui::TextBox::Ptr m_textField;
	tgui::Label::Ptr m_commentLabel;

public:
	Event<> OnConfirm;

	GuiInputWindow(const SharedContext & sharedContext);
	virtual ~GuiInputWindow();

private:
	void initialize();

protected:
	void afterInitialize() override;
	void validateInput(const std::string& input) const;
};

