#pragma once
#include "GuiWindowBlank.hpp"

class GuiInfoBanner : public GuiWindowBlank
{
public:
	Signal<> OnContinue;

protected:
	tgui::Label::Ptr m_text;
	tgui::Panel::Ptr m_background;

public:
	GuiInfoBanner(const SharedContext& sharedContext);

	virtual ~GuiInfoBanner();

	static std::shared_ptr<GuiInfoBanner> create(const SharedContext& sharedContext);

	void setText(const std::string& text);

protected:
	void onAddedToContainer(const sf::View& containerView) override;

private:
	void initialize();
};
