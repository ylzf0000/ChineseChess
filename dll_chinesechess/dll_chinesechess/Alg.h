#pragma once
#include "Board.h"

struct Search
{
	int mvResult;
	int historyTable[65536];
};
// �����߽�(Fail-Soft)��Alpha-Beta��������
int SearchFull(int alpha, int beta, int depth);

// ����������������
void SearchMain();

// ���Ի�Ӧһ����
int ResponseMove();