#include "Window.h"

namespace Graphics {
	Window::Window(const std::string& title, const sf::Vector2u& size) {
		setup(title, size);
	}

	Window::~Window() {
		m_window.close();
	}

	void Window::setup(const std::string& title, const sf::Vector2u& size) {

		m_windowTitle = title;
		m_windowSize = size;

		initialize();
	}

	void Window::initialize() {
		sf::Uint32 style = sf::Style::Default;
		m_window.create(sf::VideoMode(m_windowSize.x, m_windowSize.y, 32), m_windowTitle, style);
		m_window.setFramerateLimit(60);
	}

	void Window::beginDraw() {
		m_window.clear(sf::Color::Black);
	}

	void Window::draw(const sf::Drawable& drawable) {
		m_window.draw(drawable);
	}

	void Window::endDraw() {
		m_window.display();
	}

	bool Window::isOpen()
	{
		return m_window.isOpen();
	}

	sf::RenderWindow* Window::RenderWindow() {
		return &m_window;
	}
	sf::Vector2u Window::WindowSize() {
		return m_windowSize;
	}
}
