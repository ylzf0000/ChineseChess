#include "pch.h"
#include "alg_custom.h"
#include "Board.h"

using namespace std;
Search sc;
// 超出边界(Fail-Soft)的Alpha-Beta搜索过程
int searchFull(int alpha, int beta, int depth)
{
	vector<int> mvList;
	Board& board = Board::Instance();
	// 一个Alpha-Beta完全搜索分为以下几个阶段

	// 1. 到达水平线，则返回局面评价值
	if (depth == 0)
		return board.Evaluate();

	// 2. 初始化最佳值和最佳走法
	int valBest = -MATE_VALUE;// 这样可以知道，是否一个走法都没走过(杀棋)
	int mvBest = 0; // 这样可以知道，是否搜索到了Beta走法或PV走法，以便保存到历史表

	// 3. 生成全部走法，并根据历史表排序
	int n = board.GenerateMoves(mvList);
	sort(begin(mvList), end(mvList), [&](int a, int b) {
		return sc.historyTable[a] > sc.historyTable[b]; });

	// 4. 逐一走这些走法，并进行递归
	for (int i = 0; i < n; ++i)
	{
		int mv = mvList[i];
		int pcKilled;
		if (board.MakeMove(mv, pcKilled))
		{
			int val = -searchFull(-beta, -alpha, depth - 1);
			board.UndoMakeMove();

			// 5. 进行Alpha-Beta大小判断和截断
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

	// 5. 所有走法都搜索完了，把最佳走法(不能是Alpha走法)保存到历史表，返回最佳值
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

