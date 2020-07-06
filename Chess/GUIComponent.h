#pragma once
#include <string>
#include "SFML/Graphics.hpp"

enum struct GUIComponentState : char
{
	Neutral, Focused, Clicked
};
enum struct MouseButton : char
{
	Left, Middle, Right
};
struct GUIEvent {
	sf::Vector2f MousePos;
	MouseButton Button;
};

class GUIComponent : public sf::Drawable
{
	friend class GUIInterface;
public:

	GUIComponent(const std::string& name);
	virtual ~GUIComponent() {};

	virtual void onClick(const GUIEvent& event) = 0;
	virtual void onRelease() = 0;
	virtual void onHover(const sf::Vector2f& mousePos) = 0;
	virtual void onLeave() = 0;

	virtual void update(float deltaTime) = 0;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override = 0;

	sf::Vector2f getGlobalPosition() const;

	const std::string& getName() const;

	virtual void setPosition(sf::Vector2f& position);
	const sf::Vector2f& getPosition() const;

	void setState(GUIComponentState state);
	GUIComponentState getState() const;

	void setActive(bool state);
	bool isActive() const;

protected:
	std::string m_name;
	sf::Vector2f m_position;
	GUIInterface* m_parent;

	GUIComponentState m_state;
	bool m_isActive;
};

