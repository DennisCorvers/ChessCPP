#pragma once
#include "GuiWindow.hpp"

class MessageBox : public GuiWindow {

public:
	MessageBox(const SharedContext& sharedContext) :
		GuiWindow(sharedContext)
	{ }

	virtual ~MessageBox() { }
};

