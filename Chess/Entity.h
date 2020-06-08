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

public:
	bool isEnabled;

	Entity();
	virtual ~Entity();

	virtual void createSprite(sf::Texture* texture);


	virtual const sf::Vector2f getPosition() const;
	virtual const sf::Vector2f getCenter() const;
	virtual const sf::FloatRect getGlobalBounds() const;
	virtual const bool boundsContains(float x, float y) const;

	virtual void setPosition(const float x, const float y);
	virtual void setCenter(const float x, const float y);
	virtual void setScale(const float x, const float y);

	virtual void move(const float dt, const float x, const float y);

	virtual void update(const float& dt);
	virtual void lateUpdate(const float& dt);
	virtual void render(sf::RenderTarget* const target);
};

