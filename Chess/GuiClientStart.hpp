#pragma once
#include "GuiInputWindow.h"
#include "GuiMessageBox.hpp"

class GuiClientStart : public GuiInputWindow
{
public:
	Signal<const std::string&, unsigned short> OnConfirm;

	GuiClientStart(const SharedContext & sharedContext);
	virtual ~GuiClientStart();

	static std::shared_ptr<GuiClientStart> create(const SharedContext& sharedContext);

private:
	void validateInput(const std::string & input) override;

	void showMessage(const std::string& message);
};

