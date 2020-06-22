#include "pch.h"
#include "Window.h"
#include "Entity.h"



void Entity::initialize()
{
	setActive(true);
}

Entity::Entity()
{
	initialize();
}

Entity::~Entity()
{

}

void Entity::createSprite(sf::Texture * texture)
{
	m_sprite.setTexture(*texture);
}

void Entity::setPosition(const float x, const float y)
{
	if (isActive())
		m_sprite.setPosition(x, y);
}

void Entity::setPosition(const sf::Vector2f& position)
{
	setPosition(position.x, position.y);
}

void Entity::setCenter(const float x, const float y)
{
	if (isActive()) {
		m_sprite.setPosition(
			x - m_sprite.getGlobalBounds().width / 2.f,
			y - m_sprite.getGlobalBounds().height / 2.f);
	}
}

void Entity::setCenter(const sf::Vector2f& position)
{
	setCenter(position.x, position.y);
}

void Entity::setScale(const float x, const float y)
{
	if (isActive())
		m_sprite.setScale(x, y);
}

void Entity::setScale(const sf::Vector2f& position)
{
	setScale(position.x, position.y);
}

void Entity::move(const sf::Vector2f & position)
{
}

const sf::Vector2f Entity::getPosition() const
{
	return m_sprite.getPosition();
}

const sf::Vector2f Entity::getCenter() const
{
	return
		m_sprite.getPosition() +
		sf::Vector2f
		(
			m_sprite.getGlobalBounds().width / 2.f,
			m_sprite.getGlobalBounds().height / 2.f
		);
}

const sf::FloatRect Entity::getGlobalBounds() const
{
	return m_sprite.getGlobalBounds();
}

const bool Entity::boundsContains(float x, float y) const
{
	return m_sprite.getGlobalBounds().contains(x, y);
}

void Entity::update(const float& dt)
{
}

void Entity::lateUpdate(const float& dt)
{
}

void Entity::render(sf::RenderTarget* const target)
{
	if (isActive())
		target->draw(m_sprite);
}

