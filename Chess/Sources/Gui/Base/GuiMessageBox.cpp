#include "pch.h"
#include "GuiMessageBox.hpp"

GuiMessageBox::GuiMessageBox(const SharedContext & sharedContext) :
	GuiWindow(sharedContext),
	m_result(MessageBoxResult::None)
{
	initialize();
}

GuiMessageBox::~GuiMessageBox()
{ }

void GuiMessageBox::initialize()
{
	SoundManager* soundManager = m_sharedContext.soundManager;

	for (int i = 0; i < MAX_BUTTONS; i++) {
		m_buttons[i] = tgui::Button::create();
		m_buttons[i]->connect("pressed", [this, i]() { buttonClicked(i); });
		m_buttons[i]->connect("mouseentered", [soundManager]() { soundManager->playSound(AssetNames::s_button_hover); });
		m_buttons[i]->connect("pressed", [soundManager]() { soundManager->playSound(AssetNames::s_button_click); });
		m_buttons[i]->setSize(80, 25);
		m_buttons[i]->setVisible(false);
		m_guiWindow->add(m_buttons[i]);
	}

	m_label = tgui::Label::create();
	m_guiWindow->setTitleAlignment(tgui::ChildWindow::TitleAlignment::Left);
	m_guiWindow->add(m_label);
}


void GuiMessageBox::show(const std::string & text) {
	show(text, "Message");
}

void GuiMessageBox::show(const std::string & text, const std::string & title) {
	show(text, title, MessageBoxButtons::OK);
}

void GuiMessageBox::show(const std::string & text, const std::string & title, MessageBoxButtons buttons) {

	//Setproperties first (for auto-sizing)
	m_result = MessageBoxResult::None;
	m_setButtons = buttons;

	m_guiWindow->setTitle(" " + title);
	m_label->setText(text);

	//Calculate window size and placement to fit everything to the window
	setGuiWindow();
	setButtons();
	placeLabel();

	GuiWindow::showDialog();
}


MessageBoxResult GuiMessageBox::getMessageBoxResult() const {
	return m_result;
}


void GuiMessageBox::setGuiWindow()
{
	unsigned char buttonCount = getButtonCount();
	const auto& buttonSize = m_buttons[0]->getSize();

	//Calculate minimum required size for label + buttons.
	sf::Vector2u windowSize;
	windowSize.x = m_label->getSize().x + TEXT_PAD_LEFT * 2;
	windowSize.y = m_label->getSize().y + TEXT_PAD_Y * 2 + buttonSize.y + BUTTON_PAD;

	unsigned int buttonsWidth = BUTTON_LEFT * 2 + buttonSize.x * buttonCount + (buttonCount - 1) * 20;
	windowSize.x = Math::max(windowSize.x, buttonsWidth);
	windowSize.x = Math::clamp(windowSize.x, MIN_X_SIZE, MAX_X_SIZE);
	windowSize.y = Math::clamp(windowSize.y, MIN_Y_SIZE, MAX_Y_SIZE);

	m_guiWindow->setSize(windowSize.x, windowSize.y);

	m_guiWindow->setPosition((m_viewSize.x - m_guiWindow->getSize().x) / 2, (m_viewSize.y - m_guiWindow->getSize().y) / 2);
}

void GuiMessageBox::setButtons()
{
	unsigned char buttonCount = getButtonCount();
	auto buttonTexts = getButtonText();

	//Hide all buttons first
	for (auto& button : m_buttons) {
		button->setVisible(false);
	}

	const auto& buttonSize = m_buttons[0]->getSize();

	//Calculate Y and X offset to center all buttons
	int yOffset = m_guiWindow->getSize().y - buttonSize.y - BUTTON_PAD;
	int xOffset = (m_guiWindow->getSize().x - (buttonSize.x * buttonCount + (buttonCount - 1) * 20)) / 2;
	for (int i = 0; i < buttonCount; i++) {
		m_buttons[i]->setText(buttonTexts[i]);
		m_buttons[i]->setVisible(true);
		m_buttons[i]->setPosition(xOffset, yOffset);
		xOffset += buttonSize.x + 20;
	}
}

void GuiMessageBox::placeLabel() {

	//Center X
	const auto& mySize = m_guiWindow->getSize();
	int xOffset = (mySize.x - m_label->getSize().x) / 2;
	xOffset = Math::max<int>(xOffset, TEXT_PAD_LEFT);

	//Center Y
	int yOffset = (mySize.y - m_label->getSize().y - m_buttons[0]->getSize().y - BUTTON_PAD) / 2;
	m_label->setPosition(xOffset, yOffset);
}

void GuiMessageBox::onAddedToContainer(const sf::View & containerView)
{
	m_viewSize = containerView.getSize();
}

unsigned char GuiMessageBox::getButtonCount() const
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

const std::vector<std::string> GuiMessageBox::getButtonText() const
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

void GuiMessageBox::buttonClicked(int buttonID)
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

	OnMessageBoxResult.invoke(m_result);
	close();
}
