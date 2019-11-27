#include "pch.h"
#include "utility.h"
BOOL isLegalMove(BYTE* squares, int mv, int player)
{
	// 1. 判断起始格是否有自己的棋子
	int src = getSrc(mv);
	int pcSrc = squares[src];
	int pcSelfSide = sideTag(player);
	if ((pcSrc & pcSelfSide) == 0)
		return FALSE;

	// 2. 判断目标格是否有自己的棋子
	int dst = getDst(mv);
	int pcDst = squares[dst];
	if (pcDst & pcSelfSide)
		return FALSE;

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
			return FALSE;
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
			return FALSE;
	}
	case PIECE_BING:
	{
		if (isAwayHomeHalf(dst, player) &&
			(dst == src - 1 || dst == src + 1))
			return TRUE;
		return dst == squareForward(src, player);
	}
	default:
		return FALSE;
	}
}

BOOL hasLegalMove(BYTE* squares, int player)
{
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
					return TRUE;
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
					return TRUE;
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
					return TRUE;
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
						return TRUE;
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
						return TRUE;
					else //目的地有子
					{
						if (pcDst & pcOppSide)//是面对的子
							return TRUE;
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
						return TRUE;
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
							return TRUE;
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
					return TRUE;
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
							return TRUE;
					}
				}
			}
			break;
		}
		}
	}
	return FALSE;
}

int generateMoves(BYTE* squares, int** mvs, int player)
{
	auto genMvList = *mvs;
	int n = 0;
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
					genMvList[n++] = getMove(src, dst);
				//genMvList.push_back(getMove(src, dst));
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
					genMvList[n++] = getMove(src, dst);
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
					genMvList[n++] = getMove(src, dst);
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
						genMvList[n++] = getMove(src, dst);
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
						genMvList[n++] = getMove(src, dst);
					else //目的地有子
					{
						if (pcDst & pcOppSide)//是面对的子
							genMvList[n++] = getMove(src, dst);
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
						genMvList[n++] = getMove(src, dst);
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
							genMvList[n++] = getMove(src, dst);
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
					genMvList[n++] = getMove(src, dst);
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
							genMvList[n++] = getMove(src, dst);
					}
				}
			}
			break;
		}
		}
	}
	return n;
}

void initSquares(BYTE* squares)
{
	memset(squares, 0, (sizeof BYTE) * 256);
	for (int pos = 0; pos < 256; ++pos)
	{
		int pc = initBoard[pos];
		if (pc)
		{
			addPiece(squares, pos, pc);
		}
	}
}

int movePiece(BYTE* squares, int mv)
{
	int src = getSrc(mv);
	int dst = getDst(mv);
	int pcKilled = squares[dst];
	if (pcKilled)
		delPiece(squares, dst, pcKilled);
	int pc = squares[src];
	delPiece(squares, src, pc);
	addPiece(squares, dst, pc);
	return pcKilled;
}

void undoMovePiece(BYTE* squares, int mv, int pcKilled)
{
	int src = getSrc(mv);
	int dst = getDst(mv);
	int pc = squares[dst];
	delPiece(squares, dst, pc);
	addPiece(squares, src, pc);
	if (pcKilled)
		addPiece(squares, dst, pcKilled);
}

BOOL makeMove(BYTE* squares, int mv, int player)
{
	int pcKilled = movePiece(squares, mv);
	if (isChecked(squares, player))
	{
		undoMovePiece(squares, mv, pcKilled);
		return FALSE;
	}
	return TRUE;
}

void addPiece(BYTE* squares, int pos, int pc)
{
	squares[pos] = pc;
}

void delPiece(BYTE* squares, int pos, int pc)
{
	squares[pos] = 0;
}

BOOL isChecked(BYTE* squares, int player)
{
	int pcSelfSide = sideTag(player);
	int pcOppSide = oppSideTag(player);
	int src;
	// 找到棋盘上的帅(将)，再做以下判断：
	for (src = 0; src < 256; ++src)
		if (squares[src] == pcSelfSide + PIECE_JIANG)
			break;

	if (src == 256)
		return FALSE;
	// 1. 判断是否被对方的兵(卒)将军
	if (squares[squareForward(src, player)] == pcOppSide + PIECE_BING)
		return TRUE;
	for (int delta = -1; delta <= 1; delta += 2)
		if (squares[src + delta] == pcOppSide + PIECE_BING)
			return TRUE;

	// 2. 判断是否被对方的马将军(以仕(士)的步长当作马腿)
	for (int i = 0; i < 4; ++i)
	{
		if (squares[src + shiDelta[i]])
			continue;
		for (int j = 0; j < 2; ++j)
		{
			int pcDst = squares[src + maCheckDelta[i][j]];
			if (pcDst == pcOppSide + PIECE_MA)
				return TRUE;
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
					return TRUE;
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
					return TRUE;
				break;
			}
			dst += delta;
		}
	}
	return FALSE;
}

BOOL isMate(BYTE* squares, int player)
{
	int* mvList;
	int n = generateMoves(squares, &mvList, player);
	for (int i = 0; i < n; ++i)
	{
		int pcKilled = movePiece(squares, mvList[i]);
		if (!isChecked(squares, player)) // 没有被将军，说明还有活路
		{
			undoMovePiece(squares, mvList[i], pcKilled);
			return FALSE;
		}
		else // 被将军了，若是任意走一步都被将军，则已被将死, return TRUE
		{
			undoMovePiece(squares, mvList[i], pcKilled);
		}
	}
	return TRUE;
}
