#include "pch.h"
#include "alg_custom.h"
#include "Board.h"

using namespace std;
#define board  Board::Instance() 
Search sc;


// "qsort"按MVV/LVA值排序的比较函数
inline int CompareMvvLva(int lpmv1, int lpmv2) {
	return MvvLva(lpmv2) - MvvLva(lpmv1);
}

// "qsort"按历史表排序的比较函数
inline int CompareHistory(int lpmv1, int lpmv2) {
	return sc.historyTable[lpmv2] - sc.historyTable[lpmv1];
}

int ProbeHash(int vlAlpha, int vlBeta, int nDepth, int& mv)
{
	BOOL bMate; // 杀棋标志：如果是杀棋，那么不需要满足深度条件
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
			return -MATE_VALUE; // 可能导致搜索的不稳定性，立刻退出，但最佳着法可能拿到
		}
		hsh.svl -= board.mvsList.size();
		bMate = TRUE;
	}
	else if (hsh.svl < -WIN_VALUE) {
		if (hsh.svl > -BAN_VALUE) {
			return -MATE_VALUE; // 同上
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
			return; // 可能导致搜索的不稳定性，并且没有最佳着法，立刻退出
		}
		hsh.svl = vl + board.mvsList.size();
	}
	else if (vl < -WIN_VALUE) {
		if (mv == 0 && vl >= -BAN_VALUE) {
			return; // 同上
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
	// 一个静态搜索分为以下几个阶段

	// 1. 检查重复局面
	vl = board.RepStatus();
	if (vl != 0) {
		return board.RepValue(vl);
	}

	// 2. 到达极限深度就返回局面评价
	if (board.mvsList.size() == LIMIT_DEPTH) {
		return board.Evaluate();
	}

	// 3. 初始化最佳值
	vlBest = -MATE_VALUE; // 这样可以知道，是否一个走法都没走过(杀棋)

	if (board.InCheck()) {
		// 4. 如果被将军，则生成全部走法
		nGenMoves = board.GenerateMoves(mvs);
		sort(mvs.begin(), mvs.end(), CompareHistory);
	}
	else {

		// 5. 如果不被将军，先做局面评价
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

		// 6. 如果局面评价没有截断，再生成吃子走法
		nGenMoves = board.GenerateMoves(mvs, GEN_CAPTURE);
		sort(mvs.begin(), mvs.end(), CompareMvvLva);
		//qsort(mvs, nGenMoves, sizeof(int), CompareMvvLva);
	}

	// 7. 逐一走这些走法，并进行递归
	for (i = 0; i < nGenMoves; i++) {
		if (board.MakeMove(mvs[i])) {
			vl = -SearchQuiesc(-vlBeta, -vlAlpha);
			board.UndoMakeMove();

			// 8. 进行Alpha-Beta大小判断和截断
			if (vl > vlBest) {    // 找到最佳值(但不能确定是Alpha、PV还是Beta走法)
				vlBest = vl;        // "vlBest"就是目前要返回的最佳值，可能超出Alpha-Beta边界
				if (vl >= vlBeta) { // 找到一个Beta走法
					return vl;        // Beta截断
				}
				if (vl > vlAlpha) { // 找到一个PV走法
					vlAlpha = vl;     // 缩小Alpha-Beta边界
				}
			}
		}
	}

	// 9. 所有走法都搜索完了，返回最佳值
	return vlBest == -MATE_VALUE ? board.mvsList.size() - MATE_VALUE : vlBest;
}

// "SearchFull"的参数
const BOOL NO_NULL = TRUE;

// 超出边界(Fail-Soft)的Alpha-Beta搜索过程
int SearchFull(int vlAlpha, int vlBeta, int nDepth, BOOL bNoNull)
{
	vector<int> mvList;
	int nHashFlag, vl, vlBest;
	int mv, mvBest, mvHash, nNewDepth;
	SortStruct Sort;
	// 一个Alpha-Beta完全搜索分为以下几个阶段

	// 1. 到达水平线，则调用静态搜索(注意：由于空步裁剪，深度可能小于零)
	if (nDepth <= 0) {
		return SearchQuiesc(vlAlpha, vlBeta);
	}

	// 1-1. 检查重复局面(注意：不要在根节点检查，否则就没有走法了)
	vl = board.RepStatus();
	if (vl != 0) {
		return board.RepValue(vl);
	}

	// 1-2. 到达极限深度就返回局面评价
	if (board.mvsList.size() == LIMIT_DEPTH) {
		return board.Evaluate();
	}

	// 1-3. 尝试置换表裁剪，并得到置换表走法
	vl = ProbeHash(vlAlpha, vlBeta, nDepth, mvHash);
	if (vl > -MATE_VALUE) {
		return vl;
	}

	// 1-4. 尝试空步裁剪(根节点的Beta值是"MATE_VALUE"，所以不可能发生空步裁剪)
	if (!bNoNull && !board.InCheck() && board.NullOkay()) {
		board.NullMove();
		vl = -SearchFull(-vlBeta, 1 - vlBeta, nDepth - NULL_DEPTH - 1, NO_NULL);
		board.UndoNullMove();
		if (vl >= vlBeta) {
			return vl;
		}
	}

	// 2. 初始化最佳值和最佳走法
	nHashFlag = HASH_ALPHA;
	vlBest = -MATE_VALUE; // 这样可以知道，是否一个走法都没走过(杀棋)
	mvBest = 0;           // 这样可以知道，是否搜索到了Beta走法或PV走法，以便保存到历史表

	// 3. 初始化走法排序结构
	Sort.Init(mvHash);

	// 4. 逐一走这些走法，并进行递归
	while ((mv = Sort.Next()) != 0) {
		if (board.MakeMove(mv)) {
			// 将军延伸
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

			// 5. 进行Alpha-Beta大小判断和截断
			if (vl > vlBest) {    // 找到最佳值(但不能确定是Alpha、PV还是Beta走法)
				vlBest = vl;        // "vlBest"就是目前要返回的最佳值，可能超出Alpha-Beta边界
				if (vl >= vlBeta) { // 找到一个Beta走法
					nHashFlag = HASH_BETA;
					mvBest = mv;      // Beta走法要保存到历史表
					break;            // Beta截断
				}
				if (vl > vlAlpha) { // 找到一个PV走法
					nHashFlag = HASH_PV;
					mvBest = mv;      // PV走法要保存到历史表
					vlAlpha = vl;     // 缩小Alpha-Beta边界
				}
			}
		}
	}

	// 5. 所有走法都搜索完了，把最佳走法(不能是Alpha走法)保存到历史表，返回最佳值
	if (vlBest == -MATE_VALUE) {
		// 如果是杀棋，就根据杀棋步数给出评价
		return board.mvsList.size() - MATE_VALUE;
	}
	// 记录到置换表
	RecordHash(nHashFlag, vlBest, nDepth, mvBest);
	if (mvBest != 0) {
		// 如果不是Alpha走法，就将最佳走法保存到历史表
		SetBestMove(mvBest, nDepth);
	}
	return vlBest;
}

// 根节点的Alpha-Beta搜索过程
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

	// 初始化
	memset(sc.historyTable, 0, 65536 * sizeof(int));       // 清空历史表
	memset(sc.mvKillers, 0, LIMIT_DEPTH * 2 * sizeof(int)); // 清空杀手走法表
	memset(sc.HashTable, 0, HASH_SIZE * sizeof(HashItem));  // 清空置换表
	t = clock();       // 初始化定时器
	//board.mvsList.clear();

	// 搜索开局库
	//sc.mvResult = SearchBook();
	//if (sc.mvResult != 0) {
	//	board.MakeMove(sc.mvResult);
	//	if (board.RepStatus(3) == 0) {
	//		board.UndoMakeMove();
	//		return;
	//	}
	//	board.UndoMakeMove();
	//}

	// 检查是否只有唯一走法
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

	// 迭代加深过程
	for (i = 1; i <= LIMIT_DEPTH; i++) {
		vl = SearchRoot(i);
		// 搜索到杀棋，就终止搜索
		if (vl > WIN_VALUE || vl < -WIN_VALUE) {
			break;
		}
		// 超过一秒，就终止搜索
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
		// "nPhase"表示着法启发的若干阶段，依次为：

		// 0. 置换表着法启发，完成后立即进入下一阶段；
	case PHASE_HASH:
		nPhase = PHASE_KILLER_1;
		if (mvHash != 0) {
			return mvHash;
		}
		// 技巧：这里没有"break"，表示"switch"的上一个"case"执行完后紧接着做下一个"case"，下同

	  // 1. 杀手着法启发(第一个杀手着法)，完成后立即进入下一阶段；
	case PHASE_KILLER_1:
		nPhase = PHASE_KILLER_2;
		if (mvKiller1 != mvHash && mvKiller1 != 0 && board.IsLegalMove(mvKiller1)) {
			return mvKiller1;
		}

		// 2. 杀手着法启发(第二个杀手着法)，完成后立即进入下一阶段；
	case PHASE_KILLER_2:
		nPhase = PHASE_GEN_MOVES;
		if (mvKiller2 != mvHash && mvKiller2 != 0 && board.IsLegalMove(mvKiller2)) {
			return mvKiller2;
		}

		// 3. 生成所有着法，完成后立即进入下一阶段；
	case PHASE_GEN_MOVES:
		nPhase = PHASE_REST;
		nGenMoves = board.GenerateMoves(mvs);
		sort(mvs.begin(), mvs.end(), CompareHistory);
		//qsort(mvs, nGenMoves, sizeof(int), CompareHistory);
		nIndex = 0;

		// 4. 对剩余着法做历史表启发；
	case PHASE_REST:
		while (nIndex < nGenMoves) {
			mv = mvs[nIndex];
			nIndex++;
			if (mv != mvHash && mv != mvKiller1 && mv != mvKiller2) {
				return mv;
			}
		}

		// 5. 没有着法了，返回零。
	default:
		return 0;
	}
}
