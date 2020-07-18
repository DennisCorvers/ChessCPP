#pragma once
#include "Entity.h"

struct AnimatorComponent {
private:
	float m_t;
	float m_timeToTarget;
	sf::Vector2f m_startPosition;
	sf::Vector2f m_targetPosition;
	Entity* m_target;
	bool m_isAnimating;

public:
	AnimatorComponent(Entity& target, sf::Vector2f targetPosition, float timeToTarget = .2f);
	virtual ~AnimatorComponent();

	inline bool isAnimating() {
		return m_isAnimating;
	}

	void start();
	void update(const float& deltaTime);
	void render(sf::RenderTarget& target);
	void stop();
};

class AnimatorSystem
{
private:
	typedef std::function<void()> AnimatorCallback;

	std::vector<AnimatorComponent*> m_components;
	AnimatorCallback m_callback;

public:

	AnimatorSystem(AnimatorCallback callback);
	virtual ~AnimatorSystem();

	int pendingAnimations() const;
	bool isAnimating() const;

	void queueAnimation(AnimatorComponent* newAnimation);
	void stopAnimations();
	void update(const float& deltaTime);
	void render(sf::RenderTarget& target);
};

