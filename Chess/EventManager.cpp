#include "EventManager.h"



EventManager::EventManager()
{
}


EventManager::~EventManager()
{
	while (!m_inputs.empty()) {
		m_inputs.pop();
	}
}

void EventManager::handleEvent(const sf::Event& sfmlEvent)
{
	m_inputs.empty();
	sf::Event::EventType eType = sfmlEvent.type;
	MyEvent myEvent;

	//Handle keyboard
	if (eType == sf::Event::KeyPressed || eType == sf::Event::KeyReleased) {
		myEvent.eventType = MyEventType(eType);
		myEvent.keyCode = sfmlEvent.key.code;
	}
	//Handle mouse
	else if (eType == sf::Event::MouseButtonPressed || eType == sf::Event::MouseButtonReleased) {
		myEvent.eventType = MyEventType(eType);
		myEvent.keyCode = sfmlEvent.mouseButton.button;

		myEvent.mousePos.x = sfmlEvent.mouseButton.x;
		myEvent.mousePos.y = sfmlEvent.mouseButton.y;
	}

	if (myEvent.eventType != MyEventType::None) {
		m_inputs.push(myEvent);
	}
}

void EventManager::update()
{
}

bool EventManager::pollEvent(MyEvent* const eventOut)
{
	if (m_inputs.empty()) {
		return false;
	}
	
	*eventOut = m_inputs.front();
	m_inputs.pop();
	return true;
}
