#include "pch.h"
#include "GUIInterface.h"



GUIInterface::GUIInterface(const std::string & name)
	:GUIComponent(name), NeedsRedraw(false)
{
}

GUIInterface::~GUIInterface()
{
	for (auto component : m_components)
		delete component.second;
}

void GUIInterface::onClick(const GUIEvent & event)
{
}

void GUIInterface::onRelease()
{
}

void GUIInterface::onHover(const sf::Vector2f & mousePos)
{
}

void GUIInterface::onLeave()
{
}

void GUIInterface::onTextEntered(const char nextChar)
{
}

void GUIInterface::update(float deltaTime)
{
}

void GUIInterface::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(m_background);
	target.draw(m_content);
}

void GUIInterface::redraw()
{
	if (m_contentTexture->getSize().x != m_size.x || m_contentTexture->getSize().y != m_size.y) {
		m_contentTexture->create(m_size.x, m_size.y);
	}

	m_contentTexture->clear(sf::Color(0));

	for (auto& item : m_components) {
		GUIComponent* component = item.second;
		if (!component->isActive())
			continue;

		component->draw(*m_contentTexture);
	}

	m_contentTexture->display();
	m_content.setTexture(m_contentTexture->getTexture());
	m_content.setTextureRect(sf::IntRect(0, 0, m_size.x, m_size.y));
	NeedsRedraw = false;
}

void GUIInterface::addComponent(GUIComponent* const component) {
	component->m_parent = this;
	m_components.emplace(component->getName(), component);
	NeedsRedraw = true;
}

GUIComponent* GUIInterface::getComponent(const std::string & name) const {
	auto it = m_components.find(name);
	if (it != m_components.end())
		return it->second;
	return nullptr;
}

bool GUIInterface::removeComponent(const std::string& name) {
	auto it = m_components.find(name);
	if (it == m_components.end())
		return false;

	delete it->second;
	m_components.erase(it);
	NeedsRedraw = true;
	return true;
}

void GUIInterface::setSize(const sf::Vector2f& size) {
	m_size = size;
}

sf::Vector2f GUIInterface::getSize() const {
	return m_size;
}

void GUIInterface::markForRedraw(bool state) {
	m_needsRedraw = state;
	if (m_parent)
		m_parent->markForRedraw(state);
}

bool GUIInterface::needsRedraw() const {
	return m_needsRedraw;
}

void GUIInterface::setPosition(sf::Vector2f & position)
{
	GUIComponent::m_position = position;
	markForRedraw(true);
	m_background.setPosition(position);
	m_content.setPosition(position);
}

