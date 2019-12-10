#include "pch.h"
#include "alg_custom.h"
#include "Board.h"

using namespace std;
#define board  Board::Instance() 
Search sc;


// "qsort"��MVV/LVAֵ����ıȽϺ���
inline int CompareMvvLva(int lpmv1, int lpmv2) {
	return MvvLva(lpmv2) - MvvLva(lpmv1);
}

// "qsort"����ʷ������ıȽϺ���
inline int CompareHistory(int lpmv1, int lpmv2) {
	return sc.historyTable[lpmv2] - sc.historyTable[lpmv1];
}

int ProbeHash(int vlAlpha, int vlBeta, int nDepth, int& mv)
{
	BOOL bMate; // ɱ���־�������ɱ�壬��ô����Ҫ�����������
	HashItem hsh;

	hsh = sc.HashTable[board.zobr.key & (HASH_SIZE - 1)];
	if (hsh.dwLock0 != board.zobr.lock0 || hsh.dwLock1 != board.zobr.lock1) {
		mv = 0;
		return -MATE_VALUE;
	}
	mv = hsh.wmv;
	bMate = FALSE;
	if (hsh.svl > WIN_VALUE) {
		if (hsh.svl < BAN_VALUE) {
			return -MATE_VALUE; // ���ܵ��������Ĳ��ȶ��ԣ������˳���������ŷ������õ�
		}
		hsh.svl -= board.mvsList.size();
		bMate = TRUE;
	}
	else if (hsh.svl < -WIN_VALUE) {
		if (hsh.svl > -BAN_VALUE) {
			return -MATE_VALUE; // ͬ��
		}
		hsh.svl += board.mvsList.size();
		bMate = TRUE;
	}
	if (hsh.ucDepth >= nDepth || bMate) {
		if (hsh.ucFlag == HASH_BETA) {
			return (hsh.svl >= vlBeta ? hsh.svl : -MATE_VALUE);
		}
		else if (hsh.ucFlag == HASH_ALPHA) {
			return (hsh.svl <= vlAlpha ? hsh.svl : -MATE_VALUE);
		}
		return hsh.svl;
	}
	return -MATE_VALUE;
}
void RecordHash(int nFlag, int vl, int nDepth, int mv)
{
	HashItem hsh;
	hsh = sc.HashTable[board.zobr.key & (HASH_SIZE - 1)];
	if (hsh.ucDepth > nDepth) {
		return;
	}
	hsh.ucFlag = nFlag;
	hsh.ucDepth = nDepth;
	if (vl > WIN_VALUE) {
		if (mv == 0 && vl <= BAN_VALUE) {
			return; // ���ܵ��������Ĳ��ȶ��ԣ�����û������ŷ��������˳�
		}
		hsh.svl = vl + board.mvsList.size();
	}
	else if (vl < -WIN_VALUE) {
		if (mv == 0 && vl >= -BAN_VALUE) {
			return; // ͬ��
		}
		hsh.svl = vl - board.mvsList.size();
	}
	else {
		hsh.svl = vl;
	}
	hsh.wmv = mv;
	hsh.dwLock0 = board.zobr.lock0;
	hsh.dwLock1 = board.zobr.lock1;
	sc.HashTable[board.zobr.key & (HASH_SIZE - 1)] = hsh;
}
int MvvLva(int mv)
{
	return (cucMvvLva[board.squares[getDst(mv)]] << 3) - cucMvvLva[board.squares[getSrc(mv)]];
}
void SetBestMove(int mv, int nDepth)
{
	sc.historyTable[mv] += nDepth * nDepth;
	int* lpmvKillers = sc.mvKillers[board.mvsList.size()];
	if (lpmvKillers[0] != mv)
	{
		lpmvKillers[1] = lpmvKillers[0];
		lpmvKillers[0] = mv;
	}
}
int SearchQuiesc(int vlAlpha, int vlBeta)
{
	int i, nGenMoves;
	int vl, vlBest;
	vector<int> mvs;
	// һ����̬������Ϊ���¼����׶�

	// 1. ����ظ�����
	vl = board.RepStatus();
	if (vl != 0) {
		return board.RepValue(vl);
	}

	// 2. ���Ｋ����Ⱦͷ��ؾ�������
	if (board.mvsList.size() == LIMIT_DEPTH) {
		return board.Evaluate();
	}

	// 3. ��ʼ�����ֵ
	vlBest = -MATE_VALUE; // ��������֪�����Ƿ�һ���߷���û�߹�(ɱ��)

	if (board.InCheck()) {
		// 4. �����������������ȫ���߷�
		nGenMoves = board.GenerateMoves(mvs);
		sort(mvs.begin(), mvs.end(), CompareHistory);
	}
	else {

		// 5. �������������������������
		vl = board.Evaluate();
		if (vl > vlBest) {
			vlBest = vl;
			if (vl >= vlBeta) {
				return vl;
			}
			if (vl > vlAlpha) {
				vlAlpha = vl;
			}
		}

		// 6. �����������û�нضϣ������ɳ����߷�
		nGenMoves = board.GenerateMoves(mvs, GEN_CAPTURE);
		sort(mvs.begin(), mvs.end(), CompareMvvLva);
		//qsort(mvs, nGenMoves, sizeof(int), CompareMvvLva);
	}

	// 7. ��һ����Щ�߷��������еݹ�
	for (i = 0; i < nGenMoves; i++) {
		if (board.MakeMove(mvs[i])) {
			vl = -SearchQuiesc(-vlBeta, -vlAlpha);
			board.UndoMakeMove();

			// 8. ����Alpha-Beta��С�жϺͽض�
			if (vl > vlBest) {    // �ҵ����ֵ(������ȷ����Alpha��PV����Beta�߷�)
				vlBest = vl;        // "vlBest"����ĿǰҪ���ص����ֵ�����ܳ���Alpha-Beta�߽�
				if (vl >= vlBeta) { // �ҵ�һ��Beta�߷�
					return vl;        // Beta�ض�
				}
				if (vl > vlAlpha) { // �ҵ�һ��PV�߷�
					vlAlpha = vl;     // ��СAlpha-Beta�߽�
				}
			}
		}
	}

	// 9. �����߷����������ˣ��������ֵ
	return vlBest == -MATE_VALUE ? board.mvsList.size() - MATE_VALUE : vlBest;
}

// "SearchFull"�Ĳ���
const BOOL NO_NULL = TRUE;

// �����߽�(Fail-Soft)��Alpha-Beta��������
int SearchFull(int vlAlpha, int vlBeta, int nDepth, BOOL bNoNull)
{
	vector<int> mvList;
	int nHashFlag, vl, vlBest;
	int mv, mvBest, mvHash, nNewDepth;
	SortStruct Sort;
	// һ��Alpha-Beta��ȫ������Ϊ���¼����׶�

	// 1. ����ˮƽ�ߣ�����þ�̬����(ע�⣺���ڿղ��ü�����ȿ���С����)
	if (nDepth <= 0) {
		return SearchQuiesc(vlAlpha, vlBeta);
	}

	// 1-1. ����ظ�����(ע�⣺��Ҫ�ڸ��ڵ��飬�����û���߷���)
	vl = board.RepStatus();
	if (vl != 0) {
		return board.RepValue(vl);
	}

	// 1-2. ���Ｋ����Ⱦͷ��ؾ�������
	if (board.mvsList.size() == LIMIT_DEPTH) {
		return board.Evaluate();
	}

	// 1-3. �����û���ü������õ��û����߷�
	vl = ProbeHash(vlAlpha, vlBeta, nDepth, mvHash);
	if (vl > -MATE_VALUE) {
		return vl;
	}

	// 1-4. ���Կղ��ü�(���ڵ��Betaֵ��"MATE_VALUE"�����Բ����ܷ����ղ��ü�)
	if (!bNoNull && !board.InCheck() && board.NullOkay()) {
		board.NullMove();
		vl = -SearchFull(-vlBeta, 1 - vlBeta, nDepth - NULL_DEPTH - 1, NO_NULL);
		board.UndoNullMove();
		if (vl >= vlBeta) {
			return vl;
		}
	}

	// 2. ��ʼ�����ֵ������߷�
	nHashFlag = HASH_ALPHA;
	vlBest = -MATE_VALUE; // ��������֪�����Ƿ�һ���߷���û�߹�(ɱ��)
	mvBest = 0;           // ��������֪�����Ƿ���������Beta�߷���PV�߷����Ա㱣�浽��ʷ��

	// 3. ��ʼ���߷�����ṹ
	Sort.Init(mvHash);

	// 4. ��һ����Щ�߷��������еݹ�
	while ((mv = Sort.Next()) != 0) {
		if (board.MakeMove(mv)) {
			// ��������
			nNewDepth = board.InCheck() ? nDepth : nDepth - 1;
			// PVS
			if (vlBest == -MATE_VALUE) {
				vl = -SearchFull(-vlBeta, -vlAlpha, nNewDepth);
			}
			else {
				vl = -SearchFull(-vlAlpha - 1, -vlAlpha, nNewDepth);
				if (vl > vlAlpha&& vl < vlBeta) {
					vl = -SearchFull(-vlBeta, -vlAlpha, nNewDepth);
				}
			}
			board.UndoMakeMove();

			// 5. ����Alpha-Beta��С�жϺͽض�
			if (vl > vlBest) {    // �ҵ����ֵ(������ȷ����Alpha��PV����Beta�߷�)
				vlBest = vl;        // "vlBest"����ĿǰҪ���ص����ֵ�����ܳ���Alpha-Beta�߽�
				if (vl >= vlBeta) { // �ҵ�һ��Beta�߷�
					nHashFlag = HASH_BETA;
					mvBest = mv;      // Beta�߷�Ҫ���浽��ʷ��
					break;            // Beta�ض�
				}
				if (vl > vlAlpha) { // �ҵ�һ��PV�߷�
					nHashFlag = HASH_PV;
					mvBest = mv;      // PV�߷�Ҫ���浽��ʷ��
					vlAlpha = vl;     // ��СAlpha-Beta�߽�
				}
			}
		}
	}

	// 5. �����߷����������ˣ�������߷�(������Alpha�߷�)���浽��ʷ���������ֵ
	if (vlBest == -MATE_VALUE) {
		// �����ɱ�壬�͸���ɱ�岽����������
		return board.mvsList.size() - MATE_VALUE;
	}
	// ��¼���û���
	RecordHash(nHashFlag, vlBest, nDepth, mvBest);
	if (mvBest != 0) {
		// �������Alpha�߷����ͽ�����߷����浽��ʷ��
		SetBestMove(mvBest, nDepth);
	}
	return vlBest;
}

// ���ڵ��Alpha-Beta��������
static int SearchRoot(int nDepth) {
	int vl, vlBest, mv, nNewDepth;
	SortStruct Sort;

	vlBest = -MATE_VALUE;
	Sort.Init(sc.mvResult);
	while ((mv = Sort.Next()) != 0) {
		if (board.MakeMove(mv)) {
			nNewDepth = board.InCheck() ? nDepth : nDepth - 1;
			if (vlBest == -MATE_VALUE) {
				vl = -SearchFull(-MATE_VALUE, MATE_VALUE, nNewDepth, NO_NULL);
			}
			else {
				vl = -SearchFull(-vlBest - 1, -vlBest, nNewDepth);
				if (vl > vlBest) {
					vl = -SearchFull(-MATE_VALUE, -vlBest, nNewDepth, NO_NULL);
				}
			}
			board.UndoMakeMove();
			if (vl > vlBest) {
				vlBest = vl;
				sc.mvResult = mv;
				if (vlBest > -WIN_VALUE && vlBest < WIN_VALUE) {
					vlBest += (rand() & RANDOM_MASK) - (rand() & RANDOM_MASK);
				}
			}
		}
	}
	RecordHash(HASH_PV, vlBest, nDepth, sc.mvResult);
	SetBestMove(sc.mvResult, nDepth);
	return vlBest;
}

void SearchMain()
{
	int i, t, vl, nGenMoves;
	vector<int> mvs;

	// ��ʼ��
	memset(sc.historyTable, 0, 65536 * sizeof(int));       // �����ʷ��
	memset(sc.mvKillers, 0, LIMIT_DEPTH * 2 * sizeof(int)); // ���ɱ���߷���
	memset(sc.HashTable, 0, HASH_SIZE * sizeof(HashItem));  // ����û���
	t = clock();       // ��ʼ����ʱ��
	//board.mvsList.clear();

	// �������ֿ�
	//sc.mvResult = SearchBook();
	//if (sc.mvResult != 0) {
	//	board.MakeMove(sc.mvResult);
	//	if (board.RepStatus(3) == 0) {
	//		board.UndoMakeMove();
	//		return;
	//	}
	//	board.UndoMakeMove();
	//}

	// ����Ƿ�ֻ��Ψһ�߷�
	vl = 0;
	nGenMoves = board.GenerateMoves(mvs);
	for (i = 0; i < nGenMoves; i++) {
		if (board.MakeMove(mvs[i])) {
			board.UndoMakeMove();
			sc.mvResult = mvs[i];
			vl++;
		}
	}
	if (vl == 1) {
		return;
	}

	// �����������
	for (i = 1; i <= LIMIT_DEPTH; i++) {
		vl = SearchRoot(i);
		// ������ɱ�壬����ֹ����
		if (vl > WIN_VALUE || vl < -WIN_VALUE) {
			break;
		}
		// ����һ�룬����ֹ����
		if (clock() - t > CLOCKS_PER_SEC) {
			break;
		}
	}
}

int responseMove()
{
	SearchMain();
	board.PlayerMove_Unchecked(sc.mvResult);
	if (board.IsMate())
		return -1;
	return sc.mvResult;
}

BEGIN_EXTERNC

DLL_EXPORT void setMaxDepth(int d)
{
	//if (d > 0)
	//	sc.MAX_DEPTH = d;
}
DLL_EXPORT int getMaxDepth()
{
	//return sc.MAX_DEPTH;
	return 0;
}

END_EXTERNC

void SortStruct::Init(int mvHash_)
{
	mvHash = mvHash_;
	mvKiller1 = sc.mvKillers[board.mvsList.size()][0];
	mvKiller2 = sc.mvKillers[board.mvsList.size()][1];
	nPhase = PHASE_HASH;
}

int SortStruct::Next()
{
	int mv;
	switch (nPhase) {
		// "nPhase"��ʾ�ŷ����������ɽ׶Σ�����Ϊ��

		// 0. �û����ŷ���������ɺ�����������һ�׶Σ�
	case PHASE_HASH:
		nPhase = PHASE_KILLER_1;
		if (mvHash != 0) {
			return mvHash;
		}
		// ���ɣ�����û��"break"����ʾ"switch"����һ��"case"ִ��������������һ��"case"����ͬ

	  // 1. ɱ���ŷ�����(��һ��ɱ���ŷ�)����ɺ�����������һ�׶Σ�
	case PHASE_KILLER_1:
		nPhase = PHASE_KILLER_2;
		if (mvKiller1 != mvHash && mvKiller1 != 0 && board.IsLegalMove(mvKiller1)) {
			return mvKiller1;
		}

		// 2. ɱ���ŷ�����(�ڶ���ɱ���ŷ�)����ɺ�����������һ�׶Σ�
	case PHASE_KILLER_2:
		nPhase = PHASE_GEN_MOVES;
		if (mvKiller2 != mvHash && mvKiller2 != 0 && board.IsLegalMove(mvKiller2)) {
			return mvKiller2;
		}

		// 3. ���������ŷ�����ɺ�����������һ�׶Σ�
	case PHASE_GEN_MOVES:
		nPhase = PHASE_REST;
		nGenMoves = board.GenerateMoves(mvs);
		sort(mvs.begin(), mvs.end(), CompareHistory);
		//qsort(mvs, nGenMoves, sizeof(int), CompareHistory);
		nIndex = 0;

		// 4. ��ʣ���ŷ�����ʷ��������
	case PHASE_REST:
		while (nIndex < nGenMoves) {
			mv = mvs[nIndex];
			nIndex++;
			if (mv != mvHash && mv != mvKiller1 && mv != mvKiller2) {
				return mv;
			}
		}

		// 5. û���ŷ��ˣ������㡣
	default:
		return 0;
	}
}
