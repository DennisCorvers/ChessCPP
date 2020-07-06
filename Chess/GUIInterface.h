#pragma once
#include "GUIComponent.h"

class GUIInterface : public GUIComponent
{
public:
	bool NeedsRedraw;

	GUIInterface(const std::string& name);
	virtual ~GUIInterface();

	virtual void onClick(const GUIEvent & event) override;
	virtual void onRelease() override;
	virtual void onHover(const sf::Vector2f & mousePos) override;
	virtual void onLeave() override;
	virtual void onTextEntered(const char nextChar);
	virtual void update(float deltaTime) override;
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states = sf::RenderStates::Default) const override;
	virtual void redraw();

	void setPosition(sf::Vector2f & position) override;

	virtual void addComponent(GUIComponent* const component);
	virtual GUIComponent* getComponent(const std::string& name) const;
	virtual bool removeComponent(const std::string& name);

	virtual void setSize(const sf::Vector2f& size);
	virtual sf::Vector2f getSize() const;

	bool mouseInside(const sf::Vector2f& mousePos) const;

protected:
	std::unordered_map<std::string, GUIComponent*> m_components;
	sf::Vector2f m_size;

	sf::Sprite m_background;

	sf::RenderTexture* m_contentTexture;
	sf::Sprite m_content;

	virtual void markForRedraw(bool state);
	virtual bool needsRedraw() const;

private:
	bool m_needsRedraw;
};

