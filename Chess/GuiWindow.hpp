#pragma once
#include "TGUI/Gui.hpp"
#include "SFML/Main.hpp"
#include "SharedContext.hpp"
#include "GuiContainer.hpp"

enum struct WindowStatus {
	NONE,
	SHOW,
	DIALOG
};

class GuiWindow : private NonCopyable {
private:
	GuiContainer* m_container;
	int m_windowID = -1;
	WindowStatus m_windowStatus;

protected:
	friend GuiContainer;
	using ChildGuiWindow = std::shared_ptr<tgui::ChildWindow>;
	using Renderer = tgui::ChildWindowRenderer*;

	ChildGuiWindow m_guiWindow;
	Renderer m_renderer;
	const SharedContext& m_sharedContext;

public:
	Event<> OnClose; //Subscribe onClose function to this event...

	GuiWindow(const SharedContext& sharedContext) :
		m_container(nullptr),
		m_sharedContext(sharedContext),
		m_windowStatus(WindowStatus::NONE)
	{
		m_guiWindow = tgui::ChildWindow::create();
		m_guiWindow->onEscapeKeyPressed.connect([this]() { this->onEscapePress(); });
		m_guiWindow->onClose.connect([this]() { this->close(); });
		m_renderer = m_guiWindow->getRenderer();
	}

	void addChildWindow(std::shared_ptr<GuiWindow> window) {
		if (!m_container)
			throw new std::exception("GuiWindow is not owned by a GuiContainer!");

		m_container->addWindow(window);
	}


	virtual void close() {
		if (!m_container)
			throw new std::exception("GuiWindow is not owned by a GuiContainer!");

		m_windowStatus = WindowStatus::NONE;
		m_container->removeWindow(*this);
	}

	virtual void hide() {
		if (!m_container)
			throw new std::exception("GuiWindow is not owned by a GuiContainer!");

		m_container->hideWindow(*this);
	}

	virtual void show() {
		if (!m_container)
			throw new std::exception("GuiWindow is not owned by a GuiContainer!");

		m_container->showWindow(*this);
	}

	virtual void showDialog() {
		if (!m_container)
			throw new std::exception("GuiWindow is not owned by a GuiContainer!");

		m_container->showWindowOnTop(*this);
	}


	void moveToFront() {
		m_guiWindow->moveToFront();
	}

	void setVisible(bool state) {
		m_guiWindow->setVisible(state);
	}

	void setEnabled(bool state) {
		m_guiWindow->setEnabled(state);
	}

	void setFocus(bool state) {
		m_guiWindow->moveToFront();
	}

	bool isOnTop() const {
		return false;
	}

	bool isVisible() const {
		return m_guiWindow->isVisible();
	}

	bool isEnabled() const {
		return m_guiWindow->isEnabled();
	}

	bool isClosed() const {
		return !m_container;
	}

	void setDebug() {
		m_renderer->setBackgroundColor(tgui::Color(0, 0, 100, 100));
		m_renderer->setBorderColor(tgui::Color::Magenta);
	}

protected:
	virtual void onAddedToContainer(const sf::View& containerView) {};
	virtual void onHide() {};
	virtual void onShow() {};

	virtual void onEscapePress() {};

	virtual void onClose() {};

private:
	inline void innerShow() {
		setEnabled(true);
		setVisible(true);

		moveToFront();

		onShow();
	}

	inline void innerHide() {
		setEnabled(false);
		setVisible(false);

		onHide();
	}

	inline void innerClose() {
		m_windowID = -1;
		m_container = nullptr;

		OnClose.invoke();
	}
};