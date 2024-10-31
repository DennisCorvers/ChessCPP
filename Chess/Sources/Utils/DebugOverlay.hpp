#pragma once
#include "SFML/Graphics.hpp"
#include "TGUI/Widget.hpp"

class DebugOverlay {
private:
	template<int Size>
	struct FloatBuffer {
	private:
		char m_buffer[Size];

	public:
		std::string toString(float value, int precision) {
			std::snprintf(m_buffer, Size, "%.*f", precision, value);
			return std::string(m_buffer);
		}
	};

	int m_framecount;
	float m_lastDebugTime;

	tgui::Label::Ptr m_fpsLabel;
	FloatBuffer<8> m_fpsString;
	float m_lastDebugDisplay = 0;

public:
	DebugOverlay() :
		m_framecount(0),
		m_lastDebugTime(0)
	{
		m_fpsLabel = tgui::Label::create("FpsLabel");

		m_fpsLabel->getRenderer()->setTextColor(tgui::Color().Green);
		m_fpsLabel->getRenderer()->setTextOutlineColor(tgui::Color().White);
		m_fpsLabel->getRenderer()->setTextOutlineThickness(.2f);
	}

	~DebugOverlay() {}

	void update(const float& deltaTime) {
		m_framecount++;
		m_lastDebugTime += deltaTime;
	}

	void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) {
		if (m_lastDebugTime > 0.25f) {
			m_fpsLabel->setText("FPS: " + m_fpsString.toString(m_framecount / m_lastDebugTime, 1));
			m_lastDebugTime = 0;
			m_framecount = 0;
		}

		m_fpsLabel->draw(target, states);
	}
};