#pragma once
#include <queue>
#include "BaseState.h"
#include "ChessMove.h"
#include "assetNames.h"

class BoardManager;

class SGame : public BaseState
{
protected:
	using EType = sf::Event::EventType;

	sf::RenderWindow* m_window;

	std::unique_ptr<BoardManager> m_boardManager;
	std::queue<ChessMove> m_moveBuffer;

	std::map<AssetNames, sf::SoundBuffer> sounds;

	void loadAssets();

public:
	SGame(StateManager& stateManager);
	~SGame();

	virtual void onCreate() override;
	virtual void onDestroy() override;

	virtual void activate() override;
	virtual void deactivate() override;

	virtual void render() override;
	virtual bool update(float deltaTime) override;
	virtual bool handleEvent(const sf::Event & event) override;
};

