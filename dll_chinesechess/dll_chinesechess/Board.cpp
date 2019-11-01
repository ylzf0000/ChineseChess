#include "pch.h"
#include "Board.h"
#include <cstring>
using namespace std;
void Board::Init()
{
	player = 0;
	valRed = 0;
	valBlack = 0;
	dist = 0;
	memset(squares, 0, sizeof(squares));
	for (int pos = 0; pos < 256; ++pos)
	{
		int pc = initBoard[pos];
		if (pc)
		{
			AddPiece(pos, pc);
		}
	}
}

int Board::MovePiece(int mv)
{
	int src = getSrc(mv);
	int dst = getDst(mv);
	int pcKilled = squares[dst];
	if (pcKilled)
		DelPiece(dst, pcKilled);
	int pc = squares[src];
	DelPiece(src, pc);
	AddPiece(dst, pc);
	return pcKilled;
}

void Board::UndoMovePiece(int mv, int pcKilled)
{
	int src = getSrc(mv);
	int dst = getDst(mv);
	int pc = squares[dst];
	DelPiece(dst, pc);
	AddPiece(src, pc);
	if (pcKilled)
		AddPiece(dst, pcKilled);
}

bool Board::MakeMove(int mv, int& pcKilled)
{
	pcKilled = MovePiece(mv);
	if (IsChecked())
	{
		UndoMovePiece(mv, pcKilled);
		return false;
	}
	ChangeSide();
	++dist;
	return true;
}

int Board::GenerateMoves(int* mvs) const
{
	return 0;
}

