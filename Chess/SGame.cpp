#include "pch.h"
#include "SGame.h"
#include "EventManager.h"
#include "BoardManager.h"
#include "BaseState.h"
#include "StateManager.h"
#include "TextureManager.hpp"

void SGame::loadAssets()
{
	auto* textureManager = m_stateManager->getContext()->textureManager;
	textureManager->requireResource(States::Sandbox, AssetNames::t_board);
	textureManager->requireResource(States::Sandbox, AssetNames::t_pieces);

	if (!sounds[AssetNames::s_piece_check].loadFromFile("Assets\\Sounds\\piece_check.ogg"))
		throw std::exception("Unable to load piece_check.ogg");
	if (!sounds[AssetNames::s_piece_move].loadFromFile("Assets\\Sounds\\piece_move.ogg"))
		throw std::exception("Unable to load piece_move.ogg");
	if (!sounds[AssetNames::s_piece_take].loadFromFile("Assets\\Sounds\\piece_take.ogg"))
		throw std::exception("Unable to load piece_take.ogg");
	if (!sounds[AssetNames::s_piece_castle].loadFromFile("Assets\\Sounds\\piece_castle.ogg"))
		throw std::exception("Unable to load piece_castle.ogg");
}

SGame::SGame(StateManager& stateManager)
	:BaseState(stateManager)
{
	m_window = stateManager.getContext()->window;
	loadAssets();

	m_boardManager = std::unique_ptr<BoardManager>(
		new BoardManager(
			*stateManager.getContext()->textureManager,
			sounds,
			static_cast<int>(m_window->getSize().y * .85f)
		));

	auto context = stateManager.getContext();
	float x = static_cast<float>(m_window->getSize().x);
	float y = static_cast<float>(m_window->getSize().y);

	m_view.setSize(sf::Vector2f(x, y));
	m_view.setCenter(m_boardManager->getBoardCenter());
}

SGame::~SGame()
{}

void SGame::onCreate() {
	m_boardManager->resetGame();
}

void SGame::onDestroy() {
	TextureManager* txm = m_stateManager->getContext()->textureManager;
	txm->releaseResource(States::Sandbox);
}

void SGame::activate() {
}

void SGame::deactivate() {
}

void SGame::render() {

	m_boardManager->render(*m_window);
}

bool SGame::update(float deltaTime)
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

bool SGame::handleEvent(const sf::Event & event)
{
	EType eType = event.type;
	if (eType == sf::Event::Resized)
		Graphics::applyResize(m_view, m_window->getSize().x, m_window->getSize().y);


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

