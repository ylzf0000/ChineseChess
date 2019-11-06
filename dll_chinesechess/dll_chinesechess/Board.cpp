#include "pch.h"
#include "Board.h"
#include <cstring>
using namespace std;
void Board::Startup()
{
	player = 0;
	valRed = 0;
	valBlack = 0;
	nStep = 0;
	sqSelected = 0;
	mvLast = 0;
	mvsList.reserve(256);
	zobr.InitZero();
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

BOOL Board::MakeMove(int mv, int& pcKilled)
{
	pcKilled = MovePiece(mv);
	if (IsChecked())
	{
		UndoMovePiece(mv, pcKilled);
		return FALSE;
	}
	ChangeSide();
	mvsList.push_back({ (WORD)mv,(BYTE)pcKilled,(BYTE)IsChecked(),zobr.key });
	++nStep;
	return TRUE;
}

int Board::PlayerMove_Unchecked(int mv)
{
	int pcKilled;
	if (MakeMove(mv, pcKilled))
	{
		mvLast = mv;
		return 1;
	}
	return 0;

}

int Board::PlayerMove_Checked(int mv)
{
	if (IsLegalMove(mv))
		return PlayerMove_Unchecked(mv);
	else
		return -1;
}

int Board::GenerateMoves(int* mvs, BOOL isKill) const
{
	int nGenMoves = 0;
	int pcSelfSide = sideTag(player);
	int pcOppSide = oppSideTag(player);
	for (int src = 0; src < 256; ++src)
	{
		int pcSrc = squares[src];
		if ((pcSrc & pcSelfSide) == 0)
			continue;
		switch (pcSrc - pcSelfSide)
		{
		case PIECE_JIANG:
		{
			for (int i = 0; i < 4; ++i)
			{
				int dst = src + jiangDelta[i];
				if (!isInJiuGong(dst))
					continue;
				int pcDst = squares[dst];
				if ((pcDst & pcSelfSide) == 0)
					mvs[nGenMoves++] = getMove(src, dst);
			}
			break;
		}
		case PIECE_SHI:
		{
			for (int i = 0; i < 4; ++i)
			{
				int dst = src + shiDelta[i];
				if (!isInJiuGong(dst))
					continue;
				int pcDst = squares[dst];
				if ((pcDst & pcSelfSide) == 0)
					mvs[nGenMoves++] = getMove(src, dst);
			}
			break;
		}
		case PIECE_XIANG:
		{
			for (int i = 0; i < 4; ++i)
			{
				int dst = src + shiDelta[i];
				if (!(isInBoard(dst) && isHomeHalf(dst, player) && squares[dst] == 0))
					continue;
				dst += shiDelta[i];
				int pcDst = squares[dst];
				if ((pcDst & pcSelfSide) == 0)
					mvs[nGenMoves++] = getMove(src, dst);
			}
			break;
		}
		case PIECE_MA:
		{
			for (int i = 0; i < 4; ++i)
			{
				int pin = src + jiangDelta[i];
				if (squares[pin])
					continue;
				for (int j = 0; j < 2; ++j)
				{
					int dst = src + maDelta[i][j];
					if (!isInBoard(dst))
						continue;
					int pcDst = squares[dst];
					if ((pcDst & pcSelfSide) == 0)
						mvs[nGenMoves++] = getMove(src, dst);
				}
			}
			break;
		}
		case PIECE_JU:
		{
			for (int i = 0; i < 4; ++i)
			{
				int delta = jiangDelta[i];
				int dst = src + delta;
				while (isInBoard(dst))
				{
					int pcDst = squares[dst];
					if (pcDst == 0)
						mvs[nGenMoves++] = getMove(src, dst);
					else //目的地有子
					{
						if (pcDst & pcOppSide)//是面对的子
							mvs[nGenMoves++] = getMove(src, dst);
						//否则是自己的子
						break;
					}
					dst += delta;
				}
			}
			break;
		}
		case PIECE_PAO:
		{
			for (int i = 0; i < 4; ++i)
			{
				int delta = jiangDelta[i];
				int dst = src + delta;
				while (isInBoard(dst))
				{
					int pcDst = squares[dst];
					if (pcDst == 0)
						mvs[nGenMoves++] = getMove(src, dst);
					else  //目的地有子
						break;
					dst += delta;
				}
				dst += delta;
				while (isInBoard(dst))
				{
					int pcDst = squares[dst];
					if (pcDst)
					{
						if (pcDst & pcOppSide)
							mvs[nGenMoves++] = getMove(src, dst);
						break;
					}
					dst += delta;
				}
			}
			break;
		}
		case PIECE_BING:
		{
			int dst = squareForward(src, player);
			if (isInBoard(dst))
			{
				int pcDst = squares[dst];
				if ((pcDst & pcSelfSide) == 0)
					mvs[nGenMoves++] = getMove(src, dst);
			}
			if (isAwayHomeHalf(src, player))
			{
				for (int delta = -1; delta <= 1; delta += 2)
				{
					dst = src + delta;
					if (isInBoard(dst))
					{
						int pcDst = squares[dst];
						if ((pcDst & pcSelfSide) == 0)
							mvs[nGenMoves++] = getMove(src, dst);
					}
				}
			}
			break;
		}
		}
	}
	return nGenMoves;
}

BOOL Board::IsLegalMove(int mv) const
{
	// 1. 判断起始格是否有自己的棋子
	int src = getSrc(mv);
	int pcSrc = squares[src];
	int pcSelfSide = sideTag(player);
	if ((pcSrc & pcSelfSide) == 0)
		return false;

	// 2. 判断目标格是否有自己的棋子
	int dst = getDst(mv);
	int pcDst = squares[dst];
	if (pcDst & pcSelfSide)
		return false;

	// 3. 根据棋子的类型检查走法是否合理
	switch (pcSrc - pcSelfSide)
	{
	case PIECE_JIANG:
		return isInJiuGong(dst) && isJiangMoveLegal(src, dst);
	case PIECE_SHI:
		return isInJiuGong(dst) && isShiMoveLegal(src, dst);
	case PIECE_XIANG:
		return isSameHalf(src, dst) && isXiangMoveLegal(src, dst)
			&& squares[getXiangPin(src, dst)] == 0;
	case PIECE_MA:
	{
		int pin = getMaPin(src, dst);
		return pin != src && squares[pin] == 0;
	}
	case PIECE_JU:
	case PIECE_PAO:
	{
		int delta;
		if (isSameY(src, dst))
			delta = dst < src ? -1 : 1;
		else if (isSameX(src, dst))
			delta = dst < src ? -16 : 16;
		else
			return false;
		int pin = src + delta;
		while (pin != dst && squares[pin] == 0)
			pin += delta;
		if (pin == dst)
			return pcDst == 0 || pcSrc - pcSelfSide == PIECE_JU;
		else if (pcDst && pcSrc - pcSelfSide == PIECE_PAO)
		{
			pin += delta;
			while (pin != dst && squares[pin] == 0)
				pin += delta;
			return pin == dst;
		}
		else
			return false;
	}
	case PIECE_BING:
	{
		if (isAwayHomeHalf(dst, player) &&
			(dst == src - 1 || dst == nStep + 1))
			return true;
		return dst == squareForward(src, player);
	}
	default:
		return false;
	}
}

// 判断是否被将军
BOOL Board::IsChecked() const
{
	int pcSelfSide = sideTag(player);
	int pcOppSide = oppSideTag(player);
	int src;
	// 找到棋盘上的帅(将)，再做以下判断：
	for (src = 0; src < 256; ++src)
		if (squares[src] == pcSelfSide + PIECE_JIANG)
			break;

	if (src == 256)
		return false;
	// 1. 判断是否被对方的兵(卒)将军
	if (squares[squareForward(src, player)] == pcOppSide + PIECE_BING)
		return true;
	for (int delta = -1; delta <= 1; delta += 2)
		if (squares[src + delta] == pcOppSide + PIECE_BING)
			return true;

	// 2. 判断是否被对方的马将军(以仕(士)的步长当作马腿)
	for (int i = 0; i < 4; ++i)
	{
		if (squares[src + shiDelta[i]])
			continue;
		for (int j = 0; j < 2; ++j)
		{
			int pcDst = squares[src + maCheckDelta[i][j]];
			if (pcDst == pcOppSide + PIECE_MA)
				return true;
		}
	}

	// 3. 判断是否被对方的车或炮将军(包括将帅对脸)
	for (int i = 0; i < 4; ++i)
	{
		int delta = jiangDelta[i];
		int dst = src + delta;
		while (isInBoard(dst))
		{
			int pcDst = squares[dst];
			if (pcDst)
			{
				if (pcDst == pcOppSide + PIECE_JU ||
					pcDst == pcOppSide + PIECE_JIANG)
					return true;
				break;
			}
			dst += delta;
		}
		dst += delta;
		while (isInBoard(dst))
		{
			int pcDst = squares[dst];
			if (pcDst)
			{
				if (pcDst == pcOppSide + PIECE_PAO)
					return true;
				break;
			}
			dst += delta;
		}
	}
	return false;
}

// 判断是否被将死
BOOL Board::IsMate()
{
	int mvs[MAX_GEN_MOVES];
	int n = GenerateMoves(mvs, TODO);
	for (int i = 0; i < n; ++i)
	{
		int pcKilled = MovePiece(mvs[i]);
		if (!IsChecked()) // 没有被将军，说明还有活路
		{
			UndoMovePiece(mvs[i], pcKilled);
			return false;
		}
		else // 被将军了，若是任意走一步都被将军，则已被将死, return true
			UndoMovePiece(mvs[i], pcKilled);
	}
	return true;
}

