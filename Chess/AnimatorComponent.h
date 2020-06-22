#pragma once
#include "Entity.h"
#include "SFML/Graphics.hpp"
#include "Utils.h"

class AnimatorComponent
{
private:
	float m_t;
	float m_timeToTarget;
	sf::Vector2f m_startPosition;
	sf::Vector2f m_targetPosition;
	Entity* m_target;
	bool m_isAnimating;

public:
	AnimatorComponent(Entity* entity) {
		m_target = entity;
	}

	virtual ~AnimatorComponent() {

	}

	void start(const sf::Vector2f& targetPosition, float timeToTarget) {
		m_isAnimating = true;
		m_startPosition = m_target->getCenter();
		m_targetPosition = targetPosition;
		m_timeToTarget = timeToTarget;
	}

	void update(const float& deltaTime) {
		if (!m_isAnimating)
			return;

		m_t += deltaTime / m_timeToTarget;
		m_target->setCenter(Math::lerp(m_startPosition, m_targetPosition, m_t));

		if (m_t > 1)
			stop();
	}

	void stop() {
		m_target->setCenter(m_targetPosition);
		m_isAnimating = false;
		m_t = 0;
	}

	bool isFinished() {
		return !m_isAnimating;
	}
};

