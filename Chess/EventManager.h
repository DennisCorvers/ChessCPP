#pragma once

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

	static sf::Vector2i GetMousePos(const sf::RenderWindow* const window = nullptr) {
		return (window ? sf::Mouse::getPosition(*window) : sf::Mouse::getPosition());
	}

	static sf::Vector2f GetPixelPosition(const sf::RenderWindow& window, const sf::View& view) {
		return window.mapPixelToCoords(sf::Mouse::getPosition(window), view);
	}

private:
	std::queue<MyEvent> m_inputs;
};

