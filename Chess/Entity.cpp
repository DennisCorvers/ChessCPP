#include "Window.h"
#include "Entity.h"



void Entity::initialize()
{
	isEnabled = true;
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
	if (isEnabled)
		m_sprite.setPosition(x, y);
}

void Entity::setCenter(const float x, const float y)
{
	if (isEnabled) {
		m_sprite.setPosition(
			x - m_sprite.getGlobalBounds().width / 2.f,
			y - m_sprite.getGlobalBounds().height / 2.f);
	}
}

void Entity::setScale(const float x, const float y)
{
	if (isEnabled)
		m_sprite.setScale(x, y);
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

void Entity::move(const float dt, const float x, const float y)
{
}

void Entity::update()
{
}

void Entity::lateUpdate()
{
}

void Entity::render(sf::RenderTarget* target)
{
	if (isEnabled)
		target->draw(m_sprite);
}
