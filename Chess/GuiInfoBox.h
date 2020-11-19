#pragma once
#include "GuiWindowBlank.hpp"

class GuiInfoBox : public GuiWindowBlank
{
private:
	static const int SCREEN_X_SIZE = 350;
	static const int SCREEN_Y_SIZE = 200;
	bool m_isDisplayed;

protected:
	tgui::Label::Ptr m_title;
	tgui::Panel::Ptr m_background;
	tgui::Canvas::Ptr m_canvas;
	tgui::Color m_canvasColour;

	sf::Sprite m_loadSpinner;
	sf::Transform m_spinnerTransform;
	sf::Vector2f m_spinnerCenter;

public:
	GuiInfoBox(const SharedContext& sharedContext);

	virtual ~GuiInfoBox();

	static std::shared_ptr<GuiInfoBox> create(const SharedContext& sharedContext);

	void setText(const std::string& text);

	inline void update(float deltaTime) {
		if (m_isDisplayed)
			m_spinnerTransform.rotate(180 * deltaTime, m_spinnerCenter);
	}

	inline void render(sf::RenderWindow & window) {
		if (m_isDisplayed) {
			m_canvas->clear(tgui::Color(0, 0, 0, 0));
			m_canvas->draw(m_loadSpinner, m_spinnerTransform);
			m_canvas->display();
		}
	}

protected:
	void onAddedToContainer(const sf::View& containerView) override;

	void onHide() override;
	void onShow() override;
	void onClose() override;

private:
	void initialize();
};
