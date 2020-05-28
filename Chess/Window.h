#pragma once
#include <string>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

namespace Graphics {

	class Window {
	public:
		Window(const std::string& title, const sf::Vector2u& size);
		~Window();

		void beginDraw();
		void draw(const sf::Drawable& drawable);
		void endDraw();

		bool isOpen();
		sf::RenderWindow* RenderWindow();
		sf::Vector2u WindowSize();


	private:
		void setup(const std::string& title, const sf::Vector2u& size);
		void initialize();

		sf::RenderWindow m_window;
		sf::Vector2u m_windowSize;
		std::string m_windowTitle;
	};

}

