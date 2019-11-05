#include "pch.h"
#include "alg_custom.h"
#include "Board.h"

using namespace std;
Search sc;
// �����߽�(Fail-Soft)��Alpha-Beta��������
int searchFull(int alpha, int beta, int depth)
{
	int mvs[MAX_GEN_MOVES];
	Board& board = Board::Instance();
	// һ��Alpha-Beta��ȫ������Ϊ���¼����׶�

	// 1. ����ˮƽ�ߣ��򷵻ؾ�������ֵ
	if (depth == 0)
		return board.Evaluate();

	// 2. ��ʼ�����ֵ������߷�
	int valBest = -MATE_VALUE;// ��������֪�����Ƿ�һ���߷���û�߹�(ɱ��)
	int mvBest = 0; // ��������֪�����Ƿ���������Beta�߷���PV�߷����Ա㱣�浽��ʷ��

	// 3. ����ȫ���߷�����������ʷ������
	int n = board.GenerateMoves(mvs);
	sort(mvs, mvs + n, [&](int a, int b) {
		return sc.historyTable[a] > sc.historyTable[b]; });

	// 4. ��һ����Щ�߷��������еݹ�
	for (int i = 0; i < n; ++i)
	{
		int mv = mvs[i];
		int pcKilled;
		if (board.MakeMove(mv, pcKilled))
		{
			int val = -searchFull(-beta, -alpha, depth - 1);
			board.UndoMakeMove(mv, pcKilled);

			// 5. ����Alpha-Beta��С�жϺͽض�
			if (val > valBest)
			{
				valBest = val;
				if (val >= beta)
				{
					mvBest = mv;
					break;
				}
				if (val > alpha)
				{
					mvBest = mv;
					alpha = val;
				}
			}
		}
	}

	// 5. �����߷����������ˣ�������߷�(������Alpha�߷�)���浽��ʷ���������ֵ
	if (valBest == -MATE_VALUE)
		return board.nStep - MATE_VALUE;
	if (mvBest)
	{
		sc.historyTable[mvBest] += depth * depth;
		if (board.nStep == 0)
			sc.mvResult = mvBest;
	}
	return valBest;
}

void searchMain()
{
	memset(sc.historyTable, 0, sizeof(sc.historyTable));
	Board& board = Board::Instance();
	board.nStep = 0;
	int t = clock();
	for (int i = 1; i <= sc.MAX_DEPTH; ++i)
	{
		int val = searchFull(-MATE_VALUE, MATE_VALUE, i);
		if (val > WIN_VALUE || val < -WIN_VALUE)
			break;
		if (clock() - t > 2 * CLOCKS_PER_SEC)
			break;
	}

}

int responseMove()
{
	searchMain();
	Board& board = Board::Instance();
	board.PlayerMove_Unchecked(sc.mvResult);
	if (board.IsMate())
		return -1;
	return sc.mvResult;
}

BEGIN_EXTERNC

DLL_EXPORT void setMaxDepth(int d)
{
	if (d > 0)
		sc.MAX_DEPTH = d;
}
DLL_EXPORT int getMaxDepth()
{
	return sc.MAX_DEPTH;
}

END_EXTERNC

