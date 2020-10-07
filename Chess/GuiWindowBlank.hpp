#pragma once
#include "TGUI/Gui.hpp"
#include "SFML/Main.hpp"
#include "GuiWindow.hpp"

class GuiWindowBlank : public GuiWindow {

public:
	GuiWindowBlank(const SharedContext& sharedContext) :
		GuiWindow(sharedContext)
	{
		m_guiWindow->setTitleButtons(tgui::ChildWindow::TitleButton::None);
		m_renderer->setBackgroundColor(tgui::Color::Transparent);
		m_renderer->setBorderColor(tgui::Color::Transparent);
		m_guiWindow->setPositionLocked(true);
		m_renderer->setTitleBarHeight(0);
	}

	virtual void afterInitialize() = 0;

private:
	void onInitialize() override {
		m_guiWindow->setSize(getContainerSize().x, getContainerSize().y);
		afterInitialize();
	}
};