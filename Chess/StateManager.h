#pragma once
#include "States.h"
#include "SharedContext.h"

class BaseState;
class StateManager : private sf::NonCopyable
{
private:
	using StatePair = std::pair<States, std::unique_ptr<BaseState>>;
	using FactoryFunc = std::function<std::unique_ptr<BaseState>()>;

	std::vector<StatePair> m_states;
	std::vector<States> m_toRemove;
	std::map<States, FactoryFunc> m_factories;
	SharedContext* m_sharedContext;

	void createState(const States stateID);
	void removeInternal(const States stateID);

public:
	StateManager(SharedContext& context);
	~StateManager();

	void update(float deltaTime) const;
	void render() const;
	void handleEvent(const sf::Event& event) const;

	void lateUpdate();

	inline SharedContext& getContext() const {
		return *m_sharedContext;
	}
	bool hasState(const States stateID) const;
	bool isEmpty() const;

	void switchState(const States stateID);
	void removeState(const States stateID);

	void clearStates();

	template <typename T>
	void registerState(States stateID) {
		m_factories[stateID] = [this]() {
			return std::make_unique<T>(*this);
		};
	}
};
