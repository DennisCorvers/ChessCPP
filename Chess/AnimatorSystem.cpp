#include "pch.h"
#include "AnimatorSystem.h"

AnimatorComponent::AnimatorComponent(Entity& target, sf::Vector2f targetPosition, float timeToTarget)
{
	m_target = &target;
	m_targetPosition = targetPosition;
	m_timeToTarget = timeToTarget;
	m_t = 0;
}

AnimatorComponent::~AnimatorComponent() {

}

void AnimatorComponent::start()
{
	m_startPosition = m_target->getCenter();
	m_isAnimating = true;
}

void AnimatorComponent::update(const float & deltaTime)
{
	if (!m_isAnimating)
		return;

	m_t += deltaTime / m_timeToTarget;
	m_target->setCenter(Math::lerp(m_startPosition, m_targetPosition, m_t));

	if (m_t > 1)
		stop();
}

void AnimatorComponent::render(sf::RenderTarget& target)
{
	m_target->render(target);
}

void AnimatorComponent::stop() {
	m_target->setCenter(m_targetPosition);
	m_isAnimating = false;
	m_t = 0;
}

AnimatorSystem::AnimatorSystem(AnimatorCallback callback)
{
	m_callback = callback;
	m_components.reserve(2);
}

AnimatorSystem::~AnimatorSystem()
{
	stopAnimations();
}

int AnimatorSystem::pendingAnimations() const
{
	int count = 0;
	for (auto& it : m_components) {
		if (it->isAnimating())
			count++;
	}
	return 0;
}

bool AnimatorSystem::isAnimating() const
{
	return m_components.size() > 0;
}

void AnimatorSystem::queueAnimation(AnimatorComponent* newAnimation)
{
	m_components.push_back(std::move(newAnimation));
	newAnimation->start();
}

void AnimatorSystem::stopAnimations()
{
	for (auto it : m_components) {
		it->stop();
		delete it;
	}

	m_components.clear();
}

void AnimatorSystem::update(const float & deltaTime)
{
	for (auto it = m_components.begin(); it != m_components.end();) {
		if (!(*it)->isAnimating()) {
			m_callback();

			delete *it;
			it = m_components.erase(it);
		}
		else {
			(*it)->update(deltaTime);
			++it;
		}
	}
}

void AnimatorSystem::render(sf::RenderTarget& target)
{
	//Double rendering to ensure moving piece is on top.
	for (auto it : m_components) {
		it->render(target);
	}
}
