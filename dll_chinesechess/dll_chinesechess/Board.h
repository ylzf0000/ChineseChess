#pragma once
#include "pch.h"
#include "alg_custom.h"
struct Board
{
	static Board& Instance()
	{
		static Board board;
		return board;
	}
	int player;					// 0红1黑
	BYTE squares[256];			// 棋盘上的棋子
	int valRed, valBlack;		// 红、黑双方的子力价值

	int nStep;					// 距离根节点的步数
	std::vector<MoveStruct> mvsList; // 历史走法信息列表
	ZobristStruct zobr;			// 当前局面的Zobrist键值
	void ClearBoard()
	{
		player = valRed = valBlack = nStep = 0;
		memset(squares, 0, sizeof squares);
		zobr.InitZero();
	}
	void SetIrrev()			// 清空(初始化)历史走法信息
	{
		mvsList.clear();
		mvsList.push_back({ 0,0,(BYTE)IsChecked(),zobr.key });
	}

	void Startup();// 初始化棋盘
	int MovePiece(int mv);
	void UndoMovePiece(int mv, int pcKilled);
	BOOL MakeMove(int mv);
	int PlayerMove_Unchecked(int mv);//0走法被将军 1可以走且已经走
	int PlayerMove_Checked(int mv); //-1不合法 0走法被将军 1可以走且已经走
	// 生成所有走法，如果"isKill"为"TRUE"则只生成吃子走法
	int GenerateMoves(std::vector<int>& mvList, BOOL isKill = FALSE)const;
	BOOL IsLegalMove(int mv)const;
	BOOL IsChecked()const;// 判断是否被将军
	BOOL IsMate(); //判断是否被将死
	/*A.返回0，表示没有重复局面；
	**B.返回1，表示存在重复局面，但双方都无长将(判和)；
	**C.返回3(= 1 + 2)，表示存在重复局面，本方单方面长将(判本方负)；
	**D.返回5(= 1 + 4)，表示存在重复局面，对方单方面长将(判对方负)；
	**E.返回7(= 1 + 2 + 4)，表示存在重复局面，双方长将(判和)。*/
	int RepStatus(int nRecur = 1)const;	// 检测重复局面
	int RepValue(int nRepStatus)const   // 重复局面分值
	{
		int vlReturn;
		vlReturn = ((nRepStatus & 2) == 0 ? 0 : mvsList.size() - BAN_VALUE) +
			((nRepStatus & 4) == 0 ? 0 : BAN_VALUE - mvsList.size());
		return vlReturn == 0 ? DrawValue() : vlReturn;
	}
	BOOL NullOkay()const// 判断是否允许空步裁剪
	{
		return (player == 0 ? valRed : valBlack) > NULL_MARGIN;
	}
	void Mirror(Board& mirror)const;// 对局面镜像
	int GameState();					// 我方落子之后的判断
	void UndoMakeMove()
	{
		--nStep;
		ChangeSide();
		UndoMovePiece(mvsList.back().mv, mvsList.back().pcKilled);
		mvsList.pop_back();
	}
	void NullMove() // 走一步空步
	{
		DWORD dwKey = zobr.key;
		ChangeSide();
		mvsList.push_back({ 0, 0, FALSE, dwKey });
		++nStep;
	}
	void UndoNullMove() // 撤销空步
	{
		--nStep;
		mvsList.pop_back();
		ChangeSide();
	}
	int DrawValue()const
	{
		return (mvsList.size() & 1) == 0 ? -DRAW_VALUE : DRAW_VALUE;
	}
	void ChangeSide() {
		player = 1 - player;
		zobr.Xor(Zobrist::Instance().Player);
	}
	// 在棋盘上放一枚棋子
	void AddPiece(int pos, int pc)
	{
		squares[pos] = pc;
		// 红方加分，黑方(注意"cucvlPiecePos"取值要颠倒)减分
		if (pc < 16)
		{
			zobr.Xor(Zobrist::Instance().Table[pc - 8][pos]);
			valRed += piecePosValue[pc - 8][pos];
		}
		else
		{
			zobr.Xor(Zobrist::Instance().Table[pc - 9][pos]);
			valBlack += piecePosValue[pc - 16][flipSquare(pos)];
		}
	}
	// 从棋盘上拿走一枚棋子
	void DelPiece(int pos, int pc)
	{
		squares[pos] = 0;
		// 红方减分，黑方(注意"cucvlPiecePos"取值要颠倒)加分
		if (pc < 16)
		{
			zobr.Xor(Zobrist::Instance().Table[pc - 8][pos]);
			valRed -= piecePosValue[pc - 8][pos];
		}
		else
		{
			zobr.Xor(Zobrist::Instance().Table[pc - 9][pos]);
			valBlack -= piecePosValue[pc - 16][flipSquare(pos)];
		}
	}
	int Evaluate() const
	{
		return ((player == 0) ? valRed - valBlack : valBlack - valRed)
			+ ADVANCED_VALUE;
	}
	BOOL InCheck()const// 判断是否被将军
	{
		return mvsList.back().checked;
	}
	BOOL IsKill()const// 上一步是否吃子
	{
		return mvsList.back().pcKilled != 0;
	}
};