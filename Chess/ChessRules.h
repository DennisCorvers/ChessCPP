#pragma once
#include <vector>
#include "ChessPosition.h"

class ChessBoard;
class ChessPiece;
struct ChessMove;

namespace ChessRules {
	std::vector<ChessPosition> getValidPositions(const ChessPosition & selectedPosition, const ChessBoard& board);

	bool isPromotion(const ChessMove & move, const ChessPiece & piece, const ChessBoard & board);
	bool isEnpassant(const ChessMove & move, const ChessPiece & piece, const ChessBoard & board);
	bool isCheck(const ChessPosition& king, const ChessBoard& board);
}


