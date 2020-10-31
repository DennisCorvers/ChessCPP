#pragma once
#include "GuiWindowBlank.hpp"

class GuiGameOver : public GuiWindowBlank
{
public:
	Signal<> OnContinue;

protected:
	tgui::Button::Ptr m_continueButton;
	tgui::Label::Ptr m_text;
	tgui::Label::Ptr m_title;
	tgui::Panel::Ptr m_background;

public:
	GuiGameOver(const SharedContext & sharedContext);

	virtual ~GuiGameOver();

	static std::shared_ptr<GuiGameOver> create(const SharedContext& sharedContext);

	void setText(const std::string& text);
	void setTitle(const std::string& title);

protected:
	void onAddedToContainer(const sf::View& containerView) override;

private:
	void initialize();

	void onContinueClick();

	void onEscapePress() override;
};

