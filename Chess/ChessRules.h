#pragma once
#include <vector>
#include "ChessPosition.h"
class ChessBoard;

namespace ChessRules {
	std::vector<ChessPosition> getValidPositions(const ChessPosition& selectedPosition, const ChessBoard& board);
}
