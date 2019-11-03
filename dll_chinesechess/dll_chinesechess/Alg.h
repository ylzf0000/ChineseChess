#pragma once
#include "Board.h"

struct Search
{
	int mvResult;
	int historyTable[65536];
	int MAX_DEPTH = 4;
};
// 超出边界(Fail-Soft)的Alpha-Beta搜索过程
int searchFull(int alpha, int beta, int depth);

// 迭代加深搜索过程
void searchMain();

// 电脑回应一步棋
int responseMove();


 //void setMaxDepth(int d);
