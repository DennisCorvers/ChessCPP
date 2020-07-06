#pragma once
#include "GUIComponent.h"
class GUILabel :
	public GUIComponent
{
public:
	GUILabel(const std::string & name)
		:GUIComponent(name) {}
	virtual ~GUILabel() {};

	virtual void onClick(const GUIEvent & event) override {}
	virtual void onRelease() override {}
	virtual void onHover(const sf::Vector2f & mousePos) override {}
	virtual void onLeave() override {}
	virtual void update(float deltaTime) {}

	virtual void draw(sf::RenderTarget & target, sf::RenderStates states = sf::RenderStates::Default) const override {
		target.draw(m_text);
	}

	virtual void setText(const sf::Text& text) {
		m_text = text;
	}

	virtual void setPosition(sf::Vector2f & position) override {
		m_text.setPosition(position);
	}
	virtual const sf::Vector2f & getPosition() const override {
		return m_text.getPosition();
	}

protected:
	sf::Text m_text;
};

