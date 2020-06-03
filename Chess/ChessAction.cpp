#include "ChessAction.h"


namespace Mechanics {

	ChessAction::ChessAction()
	{
		m_resultingAction = 0;
	}

	ChessAction::ChessAction(ActionType actionType, PieceType pieceType)
	{
		m_resultingAction = (char)actionType << 4;
		m_resultingAction = m_resultingAction | (char)pieceType;
	}

	ActionType ChessAction::getActionType() const
	{
		return ActionType(m_resultingAction >> 4);
	}

	PieceType ChessAction::getPieceType() const
	{
		return PieceType(m_resultingAction & 0x0F);
	}

}
