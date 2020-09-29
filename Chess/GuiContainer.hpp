#pragma once
#include "GuiWindow.hpp"
#include "GuiWindowBlank.hpp"
#include "SharedContext.hpp"

class GuiContainer {
private:
	tgui::Gui m_guiBase;
	sf::View m_view;

	std::unordered_map<int, std::shared_ptr<GuiWindow>> m_childWindows;
	const SharedContext& m_sharedContext;

	bool m_maintainAspectRatio;
	int nextChildID;

public:
	GuiContainer(const SharedContext& sharedContext);
	virtual ~GuiContainer();

	void addWindow(std::shared_ptr<GuiWindow> window);
	void removeWindow(const std::shared_ptr<GuiWindow>& window);
	void removeAllWindows();

	////
	///@brief Renders the Gui element.
	////
	inline void render() {
		m_guiBase.draw();
	}
	////
	///@brief Handles events sent from the application's Window.
	///@param event Event originating from the application's Window.
	///@return Retruns false if the event was ignored by the Gui.
	////
	inline bool handleEvent(const sf::Event& event) {
		if (event.type == sf::Event::Resized) {
			Graphics::applyResize(m_view, event);
			if (m_maintainAspectRatio)
				m_guiBase.setView(m_view);
		}

		return m_guiBase.handleEvent(event);
	};

	inline bool getMaintainAspectRatio() {
		return m_maintainAspectRatio;
	}

	inline bool setMaintainAspectRatio(bool state) {
		m_maintainAspectRatio = state;
		if (state)
			m_guiBase.setView(m_view);
	}
};