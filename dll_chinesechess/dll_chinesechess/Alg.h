#pragma once
#include "Board.h"

struct Search
{
	int mvResult;
	int historyTable[65536];
	int MAX_DEPTH = 4;
};
// �����߽�(Fail-Soft)��Alpha-Beta��������
int searchFull(int alpha, int beta, int depth);

// ����������������
void searchMain();

// ���Ի�Ӧһ����
int responseMove();


 //void setMaxDepth(int d);
