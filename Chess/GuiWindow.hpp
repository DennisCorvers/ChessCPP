#pragma once
#include "TGUI/Gui.hpp"
#include "SFML/Main.hpp"
#include "SharedContext.hpp"
#include "GuiContainer.hpp"

class GuiWindow : private NonCopyable {
private:
	GuiContainer* m_container;
	int m_windowID = -1;

protected:
	friend GuiContainer;
	using ChildGuiWindow = std::shared_ptr<tgui::ChildWindow>;
	using Renderer = tgui::ChildWindowRenderer*;

	ChildGuiWindow m_guiWindow;
	Renderer m_renderer;
	const SharedContext& m_sharedContext;

public:
	Event<> OnClose;

	GuiWindow(const SharedContext& sharedContext) :
		m_container(nullptr),
		m_sharedContext(sharedContext)
	{
		m_guiWindow = tgui::ChildWindow::create();
		m_guiWindow->onEscapeKeyPressed.connect([this]() { this->onEscapePress(); });
		m_guiWindow->onClose.connect([this]() { this->onClose(); });
		m_renderer = m_guiWindow->getRenderer();
	}

	virtual void close() {
		onClose();
	}

	virtual void hide() {
		setEnabled(false);
		m_guiWindow->setVisible(false);
		onHide();
	}

	virtual void show() {
		if (!m_container)
			throw new std::exception("GuiWindow is not owned by a GuiContainer!");

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
	virtual void onDispose() {};

	virtual void onEscapePress() {};

	virtual void onClose() {
		if (m_container) {
			OnClose.invoke();
			m_container->removeWindow(m_windowID);
		}
	}
};