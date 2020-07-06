#include "pch.h"
#include "GameState.h"
#include "EventManager.h"
#include "BoardManager.h"
#include "BaseState.h"
#include "StateManager.h"

void GameState::loadAssets()
{
	if (!textures[AssetFlags::t_board].loadFromFile("Assets\\Sprites\\Board.png"))
		throw std::exception("Unable to load Board texture");
	textures[AssetFlags::t_board].setSmooth(true);
	if (!textures[AssetFlags::t_pieces].loadFromFile("Assets\\Sprites\\Pieces.png"))
		throw std::exception("Unable to load Pieces texture");
	textures[AssetFlags::t_pieces].setSmooth(true);

	if (!sounds[AssetFlags::s_piece_check].loadFromFile("Assets\\Sounds\\piece_check.ogg"))
		throw std::exception("Unable to load piece_check.ogg");
	if (!sounds[AssetFlags::s_piece_move].loadFromFile("Assets\\Sounds\\piece_move.ogg"))
		throw std::exception("Unable to load piece_move.ogg");
	if (!sounds[AssetFlags::s_piece_take].loadFromFile("Assets\\Sounds\\piece_take.ogg"))
		throw std::exception("Unable to load piece_take.ogg");
	if (!sounds[AssetFlags::s_piece_castle].loadFromFile("Assets\\Sounds\\piece_castle.ogg"))
		throw std::exception("Unable to load piece_castle.ogg");

	if (!m_font.loadFromFile("Assets\\Fonts\\OpenSans-Regular.ttf"))
		throw std::exception("Unable to load OpenSans-Regular font!");
}

GameState::GameState(StateManager& stateManager)
	:BaseState(stateManager)
{
	m_window = stateManager.getContext()->window;
	loadAssets();

	m_boardManager = std::unique_ptr<BoardManager>(new BoardManager(textures, sounds));

	auto context = stateManager.getContext();
	float x = static_cast<float>(m_window->getSize().x);
	float y = static_cast<float>(m_window->getSize().y);

	m_view.setSize(sf::Vector2f(x, y));
	m_view.setCenter(m_boardManager->getBoardCenter());
}

GameState::~GameState()
{}

void GameState::onCreate() {
	m_boardManager->resetGame();
}

void GameState::onDestroy() {
}

void GameState::activate() {
}

void GameState::deactivate() {
}

void GameState::render() {
	m_boardManager->render(*m_window);
}

bool GameState::update(float deltaTime)
{
	sf::Vector2f position = EventManager::GetPixelPosition(*m_window, m_view);
	m_boardManager->updateMousePosition(position);

	if (!m_moveBuffer.empty()) {
		//Animate only enemy moves.
		m_boardManager->inputMove(m_moveBuffer.front(), false);
		m_moveBuffer.pop();
	}

	m_boardManager->update(deltaTime);
	return true;
}

bool GameState::handleEvent(const sf::Event & event)
{
	EType eType = event.type;
	if (event.mouseButton.button == sf::Mouse::Left) {
		sf::Vector2f mousePos = m_window->mapPixelToCoords(sf::Mouse::getPosition(*m_window), m_view);

		if (eType == sf::Event::MouseButtonPressed)
			m_boardManager->startSelection(mousePos, PieceColour::Black);

		if (eType == sf::Event::MouseButtonReleased) {
			ChessMove newMove;
			if (m_boardManager->endSelection(mousePos, newMove))
				m_moveBuffer.push(newMove);
		}
	}

	if (eType == EType::KeyReleased && event.key.code == sf::Keyboard::R) {
		m_boardManager->resetGame();
	}

	if (eType == EType::KeyReleased && event.key.code == sf::Keyboard::F) {
		m_boardManager->flipBoard();
	}

	if (eType == EType::KeyReleased && event.key.code == sf::Keyboard::Escape) {
		m_stateManager->removeState(States::Sandbox);
	}

	return true;
}

