#pragma once
#include "Entity.h"
#include "Enums.h"
#include "AnimatorComponent.h"

class ChessPieceEntity : public Entity
{
private:
	std::unique_ptr<AnimatorComponent> m_animator;
	char m_pieceData;

public:
	ChessPieceEntity(const PieceColour colour, const PieceType type, const sf::Texture& textureSheet);
	void transform(const PieceColour colour, const PieceType type);

	virtual ~ChessPieceEntity();

	virtual void update(const float& deltaTime) override;

	virtual void move(const sf::Vector2f& position) override;
	void stopAnimator();

	PieceColour getColour() const;
	PieceType getType() const;

	char xPos;
	char yPos;
};

