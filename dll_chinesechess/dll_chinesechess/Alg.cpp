#include "pch.h"
#include "Alg.h"
#include <algorithm>
#include <cstring>
#include <windows.h>
#include <time.h>
using namespace std;
Search sc;
// �����߽�(Fail-Soft)��Alpha-Beta��������
int SearchFull(int alpha, int beta, int depth)
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
			int val = -SearchFull(-beta, -alpha, depth - 1);
			board.UndoMakeMove(mv, pcKilled);

			// 5. ����Alpha-Beta��С�жϺͽض�
			if (val > valBest)
			{
				valBest = val;
				if (val >= valBest)
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
		return board.depth - MATE_VALUE;
	if (mvBest)
	{
		sc.historyTable[mvBest] += depth * depth;
		if (board.depth == 0)
			sc.mvResult = mvBest;
	}
	return valBest;
}

void SearchMain()
{
	memset(sc.historyTable, 0, sizeof(sc.historyTable));
	Board& board = Board::Instance();
	board.depth = 0;
	int t = clock();
	for (int i = 1; i <= MAX_DEPTH; ++i)
	{
		int val = SearchFull(-MATE_VALUE, MATE_VALUE, i);
		if (val > WIN_VALUE || val < -WIN_VALUE)
			break;

		if (clock() - t > CLOCKS_PER_SEC)
			break;
	}

}

int ResponseMove()
{
	SearchMain();
	Board& board = Board::Instance();
	int pcKilled;
	board.MakeMove(sc.mvResult, pcKilled);
	board.mvLast = sc.mvResult;
	return 0;
}


