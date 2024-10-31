#pragma once
#include "SFML/Graphics.hpp"

namespace Graphics {
	class Window;
}

class Entity
{
private:
	void initialize();

protected:
	sf::Sprite m_sprite;
	bool m_isActive;

public:


	Entity();
	virtual ~Entity();

	virtual void createSprite(sf::Texture* texture);

	virtual const sf::Vector2f getPosition() const;
	virtual const sf::Vector2f getCenter() const;
	virtual const sf::FloatRect getGlobalBounds() const;
	virtual const bool boundsContains(float x, float y) const;

	virtual void setPosition(const float x, const float y);
	virtual void setPosition(const sf::Vector2f& position);
	virtual void setCenter(const float x, const float y);
	virtual void setCenter(const sf::Vector2f& position);
	virtual void setScale(const float x, const float y);
	virtual void setScale(const sf::Vector2f& position);

	virtual void move(const sf::Vector2f& position);

	virtual void update(const float& dt);
	virtual void lateUpdate(const float& dt);
	virtual void render(sf::RenderTarget& target);

	inline virtual void setActive(bool state) {
		m_isActive = state;
	}
	inline virtual bool isActive() {
		return m_isActive;
	}
};

