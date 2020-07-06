#include "pch.h"
#include "GUIComponent.h"
#include "GUIInterface.h"

GUIComponent::GUIComponent(const std::string & name)
{
	m_name = name;
	m_isActive = true;
	m_state = GUIComponentState::Neutral;
}

sf::Vector2f GUIComponent::getGlobalPosition() const
{
	sf::Vector2f pos = m_position;
	GUIInterface* parent = m_parent;
	while (parent != nullptr) {
		pos += parent->getPosition();
		parent = parent->m_parent;
	}
	return pos;
}

const std::string & GUIComponent::getName() const {
	return m_name;
}

void GUIComponent::setPosition(sf::Vector2f & position) {
	m_position = position;
}

const sf::Vector2f & GUIComponent::getPosition() const{
	return m_position;
}

void GUIComponent::setState(GUIComponentState state)
{
	if (state != m_state) {
		m_state = state;
		if (m_parent)
			m_parent->NeedsRedraw = true;
	}
}

GUIComponentState GUIComponent::getState() const {
	return m_state;
}

void GUIComponent::setActive(bool state)
{
}

bool GUIComponent::isActive() const {
	return m_isActive;
}


