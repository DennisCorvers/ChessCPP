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

	virtual void setPosition(const float x, const float y);
	const sf::Vector2f getPosition() const;
	const sf::Vector2f getCenter() const;
	virtual void move(const float dt, const float x, const float y);

	virtual void update();
	virtual void lateUpdate();
	virtual void render(sf::RenderTarget* target);
};

