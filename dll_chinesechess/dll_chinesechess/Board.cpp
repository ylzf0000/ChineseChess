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
	mvsList.reserve(256);
	SetIrrev();
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

int Board::GenerateMoves(std::vector<int>& mvList, BOOL isKill) const
{
	mvList.clear();
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
					mvList.push_back(getMove(src, dst));
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
					mvList.push_back(getMove(src, dst));
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
					mvList.push_back(getMove(src, dst));
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
						mvList.push_back(getMove(src, dst));
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
						mvList.push_back(getMove(src, dst));
					else //Ŀ�ĵ�����
					{
						if (pcDst & pcOppSide)//����Ե���
							mvList.push_back(getMove(src, dst));
						//�������Լ�����
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
						mvList.push_back(getMove(src, dst));
					else  //Ŀ�ĵ�����
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
							mvList.push_back(getMove(src, dst));
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
					mvList.push_back(getMove(src, dst));
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
							mvList.push_back(getMove(src, dst));
					}
				}
			}
			break;
		}
		}
	}
	return static_cast<int>(mvList.size());
}

BOOL Board::IsLegalMove(int mv) const
{
	// 1. �ж���ʼ���Ƿ����Լ�������
	int src = getSrc(mv);
	int pcSrc = squares[src];
	int pcSelfSide = sideTag(player);
	if ((pcSrc & pcSelfSide) == 0)
		return FALSE;

	// 2. �ж�Ŀ����Ƿ����Լ�������
	int dst = getDst(mv);
	int pcDst = squares[dst];
	if (pcDst & pcSelfSide)
		return FALSE;

	// 3. �������ӵ����ͼ���߷��Ƿ����
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

// �ж��Ƿ񱻽���
BOOL Board::IsChecked() const
{
	int pcSelfSide = sideTag(player);
	int pcOppSide = oppSideTag(player);
	int src;
	// �ҵ������ϵ�˧(��)�����������жϣ�
	for (src = 0; src < 256; ++src)
		if (squares[src] == pcSelfSide + PIECE_JIANG)
			break;

	if (src == 256)
		return FALSE;
	// 1. �ж��Ƿ񱻶Է��ı�(��)����
	if (squares[squareForward(src, player)] == pcOppSide + PIECE_BING)
		return TRUE;
	for (int delta = -1; delta <= 1; delta += 2)
		if (squares[src + delta] == pcOppSide + PIECE_BING)
			return TRUE;

	// 2. �ж��Ƿ񱻶Է�������(����(ʿ)�Ĳ�����������)
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

	// 3. �ж��Ƿ񱻶Է��ĳ����ڽ���(������˧����)
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

// �ж��Ƿ񱻽���
BOOL Board::IsMate()
{
	vector<int> mvList;
	int n = GenerateMoves(mvList);
	for (int i = 0; i < n; ++i)
	{
		int pcKilled = MovePiece(mvList[i]);
		if (!IsChecked()) // û�б�������˵�����л�·
		{
			UndoMovePiece(mvList[i], pcKilled);
			return FALSE;
		}
		else // �������ˣ�����������һ���������������ѱ�����, return TRUE
		{
			UndoMovePiece(mvList[i], pcKilled);
		}
	}
	return TRUE;
}

// ����ظ�����
int Board::RepStatus(int nRecur) const
{
	BOOL selfSide = FALSE;
	BOOL selfCheck = TRUE;
	BOOL oppCheck = TRUE;
	for (auto i = mvsList.crbegin(); i != mvsList.crend(); ++i)
	{
		if (selfSide)
		{
			selfCheck = selfCheck && i->checked;
			if (i->key == zobr.key)
			{
				--nRecur;
				if (nRecur == 0)
					return 1 + (selfCheck ? 2 : 0) + (oppCheck ? 4 : 0);
			}
		}
		else
		{
			oppCheck = oppCheck && i->checked;
		}
		selfSide = !selfSide;
	}
	return 0;
}

int Board::GameState()
{
	if (IsMate())
		return 3;
	return RepStatus(3);
}

