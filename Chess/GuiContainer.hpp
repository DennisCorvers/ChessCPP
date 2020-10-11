#pragma once
#include "SharedContext.hpp"

class GuiWindow;
class GuiContainer {
private:
	tgui::Gui m_guiBase;
	sf::View m_view;

	std::vector<std::shared_ptr<GuiWindow>> m_childWindows;

	bool m_maintainAspectRatio;
	int nextChildID;

public:
	GuiContainer(sf::RenderWindow& target);
	virtual ~GuiContainer();

	void addWindow(std::shared_ptr<GuiWindow> window);
	bool removeWindow(GuiWindow& window);
	bool removeWindow(int windowID);
	void removeAllWindows();

	////
	///@brief Renders the Gui element.
	////
	inline void render() {
		m_guiBase.setView(m_view);
		m_guiBase.draw();
	}

	////
	///@brief Handles events sent from the application's Window.
	///@param event Event originating from the application's Window.
	///@return Retruns false if the event was ignored by the Gui.
	////
	inline bool handleEvent(const sf::Event& event) {
		if (event.type == sf::Event::Resized) {
			if (m_maintainAspectRatio)
				Graphics::applyResize(m_view, event);
		}

		return m_guiBase.handleEvent(event);
	};

	inline bool getMaintainAspectRatio() {
		return m_maintainAspectRatio;
	}

	inline bool setMaintainAspectRatio(bool state) {
		m_maintainAspectRatio = state;
	}

	const sf::View& getView() const {
		return m_view;
	}

	void setDebug();

	////
	///@brief Displays the indicated window on top.
	////
	bool showWindowOnTop(GuiWindow& window);
	////
	///@brief Displays the indicated window on top.
	////
	bool showWindow(GuiWindow& window);
	////
	///@brief Displays the indicated window on top.
	////
	bool hideWindow(GuiWindow& window);

private:
	void innerShow(GuiWindow& window);

	bool hasWindow(int windowID);
};