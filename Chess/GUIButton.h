#pragma once
#include "GUIComponent.h"
class GUIButton :
	public GUIComponent
{
public:
	GUIButton(const std::string & name)
		: GUIComponent(name) {}
	virtual ~GUIButton() {};
	virtual void onClick(const GUIEvent & event) override {
		setState(GUIComponentState::Clicked);
	}
	virtual void onRelease() override {
		setState(GUIComponentState::Neutral);
	}
	virtual void onHover(const sf::Vector2f & mousePos) override {
		setState(GUIComponentState::Focused);
	}
	virtual void onLeave() override {
		setState(GUIComponentState::Neutral);
	}
	virtual void update(float deltaTime) {}
	virtual void draw(sf::RenderTarget & target, sf::RenderStates states = sf::RenderStates::Default) const override {
		target.draw(m_background);
		target.draw(m_text);
	}

	virtual void setPosition(sf::Vector2f & position) override {
		m_background.setPosition(position);

		const sf::FloatRect bounds(m_text.getGlobalBounds());
		const sf::Vector2f box(m_background.getGlobalBounds().width, m_background.getGlobalBounds().height);
		m_text.setOrigin((bounds.width - box.x) / 2 + bounds.left, (bounds.height - box.y) / 2 + bounds.top);
	}
	virtual const sf::Vector2f & getPosition() const override {
		return m_background.getPosition();
	}

	virtual void setText(const sf::Text& text) {
		m_text = text;
	}

protected:
	sf::Text m_text;
	sf::Sprite m_background;
};


