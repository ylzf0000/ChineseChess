#pragma once
#include "Board.h"

struct Search
{
	int mvResult;
	int historyTable[65536];
};
// 超出边界(Fail-Soft)的Alpha-Beta搜索过程
int SearchFull(int alpha, int beta, int depth);

// 迭代加深搜索过程
void SearchMain();

// 电脑回应一步棋
int ResponseMove();