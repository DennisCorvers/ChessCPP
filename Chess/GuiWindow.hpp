#pragma once
#include "TGUI/Gui.hpp"
#include "SFML/Main.hpp"
#include "SharedContext.hpp"

class GuiContainer;
class GuiWindow : private NonCopyable {
private:
	int m_windowID = -1;

protected:
	friend GuiContainer;
	using ChildGuiWindow = std::shared_ptr<tgui::ChildWindow>;
	using Renderer = tgui::ChildWindowRenderer*;

	ChildGuiWindow m_guiWindow;
	Renderer m_renderer;

public:
	GuiWindow() {
		m_guiWindow = tgui::ChildWindow::create();
		m_renderer = m_guiWindow->getRenderer();
	}

	virtual void onInitialize(const SharedContext& sharedContext) = 0;
	virtual void onHide() {};
	virtual void onShow() {};
	virtual void onDispose(const SharedContext& sharedContext) = 0;

	virtual void hide() {
		setEnabled(false);
		m_guiWindow->setVisible(false);
		onHide();
	}

	virtual void show() {
		setEnabled(true);
		onShow();
		m_guiWindow->setVisible(true);
		m_guiWindow->moveToFront();
	}

	virtual void showDialog() {
		//setEnabled(true);
		//m_guiWindow->setVisible(true);

	}

	virtual void setEnabled(bool state) {
		m_guiWindow->setEnabled(state);
	}
};