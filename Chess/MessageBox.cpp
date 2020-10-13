#include "pch.h"
#include "MessageBox.h"

MessageBox::MessageBox(const SharedContext & sharedContext) :
	GuiWindow(sharedContext),
	m_result(MessageBoxResult::None)
{
	initialize();
}

MessageBox::~MessageBox()
{

}

void MessageBox::initialize()
{
	SoundManager* soundManager = m_sharedContext.soundManager;

	for (int i = 0; i < 3; i++) {
		m_buttons[i] = tgui::Button::create();
		m_buttons[i]->connect("pressed", [this, i]() { buttonClicked(i); });
		m_buttons[i]->connect("mouseentered", [soundManager]() { soundManager->playSound(AssetNames::s_button_hover); });
		m_buttons[i]->connect("pressed", [soundManager]() { soundManager->playSound(AssetNames::s_button_click); });
		m_buttons[i]->setEnabled(false);
		m_guiWindow->add(m_buttons[i]);
	}

	m_label = tgui::Label::create();
	m_guiWindow->add(m_label);
}


void MessageBox::show(string & text) {
	show(text, "Message");
}

void MessageBox::show(string & text, string & title) {
	show(text, title, MessageBoxButtons::OK);
}

void MessageBox::show(string & text, string & title, MessageBoxButtons buttons) {

	m_label->setText(text);
	m_guiWindow->setTitle(title);
	setButtons(buttons);

	GuiWindow::showDialog();
}


MessageBoxResult MessageBox::getMessageBoxResult() const {
	return m_result;
}



void MessageBox::setButtons(MessageBoxButtons buttons)
{

}

void MessageBox::buttonClicked(int buttonID)
{

}
