#pragma once
#include "GuiManager.h"

enum struct GuiState : char {
	HIDDEN,
	SHOW,
	SHOW_ONTOP
};

class GuiBase
{
private:
	friend class GuiManager;

	GuiManager* m_guiManager;
	int m_id;
	GuiState m_guiState;

public:
	GuiBase(GuiManager& guiManager);
	virtual ~GuiBase();

	virtual void close();
	virtual void show();
	virtual void showDialog();
	virtual void hide();

protected:

	////
	///@brief Updates the Gui element.
	///@param deltaTime Time since last frame.
	////
	virtual void update(float deltaTime) const = 0;
	////
	///@brief Renders the Gui element.
	////
	virtual void render() const = 0;
	////
	///@brief Handles events sent from the application's Window.
	///@param event Event originating from the application's Window.
	///@return Retruns false if the event was ignored by the Gui.
	////
	virtual bool handleEvent(const sf::Event& event) const = 0;

	////
	///@brief Occurs after a Gui is activated
	////
	virtual void onActivate() {};
	////
	///@brief Occurs before a Gui is destroyed
	////
	virtual void onDestroy() {};
	////
	///@brief Occurs after a Gui is closed
	////
	virtual void onClose() {};
	////
	///@brief Occurs after a Gui is hidden
	////
	virtual void onDeactivate() {};
};

