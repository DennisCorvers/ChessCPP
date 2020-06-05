#pragma once

#include <SFML/Graphics.hpp>
#include <queue>

enum struct MyEventType {
	None = -1,
	KeyDown = sf::Event::KeyPressed,
	KeyUp = sf::Event::KeyReleased,
	MouseDown = sf::Event::MouseButtonPressed,
	MouseUp = sf::Event::MouseButtonReleased
};

struct MyEvent {

	MyEvent() {
		clear();
	}

	MyEventType eventType;
	int keyCode;
	sf::Vector2f mousePos;

	void clear() {
		keyCode = -1;
		eventType = MyEventType::None;
	}
};

class EventManager
{
public:
	EventManager();
	~EventManager();

	void handleEvent(const sf::Event& sfmlEvent);
	void update();
	bool pollEvent(MyEvent* const eventOut);

	sf::Vector2i GetMousePos(sf::RenderWindow* l_wind = nullptr) {
		return (l_wind ? sf::Mouse::getPosition(*l_wind) : sf::Mouse::getPosition());
	}

private:
	std::queue<MyEvent> m_inputs;
};

