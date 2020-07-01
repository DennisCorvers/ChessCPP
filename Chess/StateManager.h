#pragma once
#include "BaseState.h"
#include "States.h"
#include <stack>


struct SharedContext {

};

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
	void remove(const States stateID) {
		m_toRemove.emplace_back(stateID);
	}

public:
	StateManager(SharedContext& context);
	~StateManager();

	void update(float deltaTime);
	void render();
	void handleEvent(const sf::Event& event);

	void lateUpdate();

	SharedContext* getContext();
	bool hasState(const States stateID);

	void switchState(const States stateID);
	void removeState(const States stateID);

	template <typename T>
	void registerState(States stateID) {
		m_factories[stateID] = [this]() {
			return BaseState::state_ptr(new T(*this));
		};
	}
};
