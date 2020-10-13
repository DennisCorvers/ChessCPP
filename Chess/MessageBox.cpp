#include "pch.h"
#include "MessageBox.hpp"

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

	for (int i = 0; i < MAX_BUTTONS; i++) {
		m_buttons[i] = tgui::Button::create();
		m_buttons[i]->connect("pressed", [this, i]() { buttonClicked(i); });
		m_buttons[i]->connect("mouseentered", [soundManager]() { soundManager->playSound(AssetNames::s_button_hover); });
		m_buttons[i]->connect("pressed", [soundManager]() { soundManager->playSound(AssetNames::s_button_click); });
		m_buttons[i]->setSize(80, 30);
		m_buttons[i]->setVisible(false);
		m_guiWindow->add(m_buttons[i]);
	}

	m_label = tgui::Label::create();
	m_guiWindow->add(m_label);
}


void MessageBox::show(const std::string & text) {
	show(text, "Message");
}

void MessageBox::show(const std::string & text, const std::string & title) {
	show(text, title, MessageBoxButtons::OK);
}

void MessageBox::show(const std::string & text, const std::string & title, MessageBoxButtons buttons) {

	m_result = MessageBoxResult::None;
	m_guiWindow->setTitle(title);
	m_label->setText(text);
	setButtons(buttons);
	placeLabel();

	GuiWindow::showDialog();
}


MessageBoxResult MessageBox::getMessageBoxResult() const {
	return m_result;
}


void MessageBox::setButtons(MessageBoxButtons buttons)
{
	m_setButtons = buttons;
	unsigned char buttonCount = getButtonCount();

	if (buttonCount < 1)
		throw new std::exception("Cannot form an empty Messagebox");

	auto buttonTexts = getButtonText();

	//Hide all buttons first
	for (auto& button : m_buttons) {
		button->setVisible(false);
	}

	const tgui::Vector2f& buttonSize = m_buttons[0]->getSize();

	sf::Vector2u minSize;
	minSize.x = m_label->getSize().x + TEXT_PAD_LEFT * 2;
	minSize.y = m_label->getSize().y + TEXT_PAD_TOP * 2 + buttonSize.y + TEXT_PAD_BOTTOM;

	unsigned int buttonsWidth = 40 + buttonSize.x * buttonCount + (buttonCount - 1) * 20;

	m_guiWindow->setSize(buttonsWidth, minSize.y);


	int yOffset = m_guiWindow->getSize().y - buttonSize.y - BUTTON_PAD;
	for (int i = 0; i < buttonCount; i++) {
		m_buttons[i]->setText(buttonTexts[i]);
		m_buttons[i]->setVisible(true);
		m_buttons[i]->setPosition(50, yOffset);
	}
}

void MessageBox::placeLabel() {

	//Center X
	const auto& mySize = m_guiWindow->getSize();
	int xOffset = (mySize.x - m_label->getSize().x) / 2;
	xOffset = xOffset < TEXT_PAD_LEFT ? TEXT_PAD_LEFT : xOffset;


	//Center Y
	int buttonRegion = m_buttons[0]->getSize().y + BUTTON_PAD;
	int yOffset = (mySize.y - m_label->getSize().y - buttonRegion - TEXT_PAD_BOTTOM) / 2;


	m_label->setPosition(xOffset, yOffset);
}

unsigned char MessageBox::getButtonCount() const
{
	switch (m_setButtons) {
	case MessageBoxButtons::OK:
		return 1;
	case MessageBoxButtons::OKCancel:
		return 2;
	case MessageBoxButtons::AbortRetryIgnore:
		return 3;
	case MessageBoxButtons::YesNoCancel:
		return 3;
	case MessageBoxButtons::YesNo:
		return 2;
	case MessageBoxButtons::RetryCancel:
		return 2;
	default:
		break;
	}

	return 0;
}

const std::vector<std::string> MessageBox::getButtonText() const
{
	auto buttonTexts = std::vector<std::string>();
	buttonTexts.reserve(MAX_BUTTONS);

	switch (m_setButtons) {
	case MessageBoxButtons::OK:
	{
		buttonTexts.emplace_back("Ok");
		break;
	}
	case MessageBoxButtons::OKCancel:
	{
		buttonTexts.emplace_back("Ok");
		buttonTexts.emplace_back("Cancel");
		break;
	}
	case MessageBoxButtons::AbortRetryIgnore:
	{
		buttonTexts.emplace_back("Abort");
		buttonTexts.emplace_back("Retry");
		buttonTexts.emplace_back("Ignore");
		break;
	}
	case MessageBoxButtons::YesNoCancel:
	{
		buttonTexts.emplace_back("Yes");
		buttonTexts.emplace_back("No");
		buttonTexts.emplace_back("Cancel");
		break;
	}
	case MessageBoxButtons::YesNo:
	{
		buttonTexts.emplace_back("Yes");
		buttonTexts.emplace_back("No");
		break;
	}
	case MessageBoxButtons::RetryCancel:
	{
		buttonTexts.emplace_back("Retry");
		buttonTexts.emplace_back("Cancel");
		break;
	}
	default: break;
	}

	return buttonTexts;
}

void MessageBox::buttonClicked(int buttonID)
{
	switch (m_setButtons) {
	case MessageBoxButtons::OK:
	{
		m_result = MessageBoxResult::OK;
		break;
	}
	case MessageBoxButtons::OKCancel:
	{
		m_result = buttonID == 0 ? MessageBoxResult::OK : MessageBoxResult::Cancel;
		break;
	}
	case MessageBoxButtons::AbortRetryIgnore:
	{
		if (buttonID == 0)
			m_result = MessageBoxResult::Abort;
		else if (buttonID == 1)
			m_result = MessageBoxResult::Retry;
		else
			m_result = MessageBoxResult::Ignore;
		break;
	}
	case MessageBoxButtons::YesNoCancel:
	{
		if (buttonID == 0)
			m_result = MessageBoxResult::Yes;
		else if (buttonID == 1)
			m_result = MessageBoxResult::No;
		else
			m_result = MessageBoxResult::Cancel;
		break;
	}
	case MessageBoxButtons::YesNo:
	{
		m_result = buttonID == 0 ? MessageBoxResult::Yes : MessageBoxResult::No;
		break;
	}
	case MessageBoxButtons::RetryCancel:
	{
		m_result = buttonID == 0 ? MessageBoxResult::Retry : MessageBoxResult::Cancel;
		break;
	}
	default:
	{
		m_result = MessageBoxResult::None;
		break;
	}
	}

	OnMessageBoxResult.invoke(*this);
}
