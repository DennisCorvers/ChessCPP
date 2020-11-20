#pragma once
#include "GuiWindowBlank.hpp"

class GuiGameMessage : public GuiWindowBlank
{
public:
	Signal<> OnContinue;

protected:
	tgui::Button::Ptr m_continueButton;
	tgui::Label::Ptr m_text;
	tgui::Label::Ptr m_title;
	tgui::Panel::Ptr m_background;

public:
	GuiGameMessage(const SharedContext & sharedContext);

	virtual ~GuiGameMessage();

	static std::shared_ptr<GuiGameMessage> create(const SharedContext& sharedContext);

	void setText(const std::string& text);
	void setTitle(const std::string& title);
	void setButton(const std::string& text);

protected:
	void onAddedToContainer(const sf::View& containerView) override;

private:
	void initialize();

	void onContinueClick();

	void onEscapePress() override;
};

