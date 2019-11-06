#pragma once
#include "pch.h"
#include "alg_custom.h"
#define ALG_CUSTOM
struct Board
{
	static Board& Instance()
	{
		static Board board;
		return board;
	}
	int player;					// 0红1黑
	BYTE squares[256];			// 棋盘上的棋子
#ifdef ALG_CUSTOM
	int valRed, valBlack;		// 红、黑双方的子力价值
#endif // ALG_CUSTOM

	int nStep;					// 距离根节点的步数
	std::vector<MoveStruct> mvsList; // 历史走法信息列表
	ZobristStruct zobr;			// Zobrist
	void SetIrrev()			// 清空(初始化)历史走法信息
	{
		mvsList.clear();
		mvsList.push_back({ 0,0,(BYTE)IsChecked(),zobr.key });
	}

	int sqSelected;// 选中的格子，上一步棋
	int mvLast;
	BOOL isFlipped;// 是否翻转棋盘

	void Startup();// 初始化棋盘
	int MovePiece(int mv);
	void UndoMovePiece(int mv, int pcKilled);
	BOOL MakeMove(int mv, int& pcKilled);
	int PlayerMove_Unchecked(int mv);//0走法被将军 1可以走且已经走
	int PlayerMove_Checked(int mv); //-1不合法 0走法被将军 1可以走且已经走
	// 生成所有走法，如果"isKill"为"TRUE"则只生成吃子走法
	int GenerateMoves(int* mvs, BOOL isKill)const;
	BOOL IsLegalMove(int mv)const;
	BOOL IsChecked()const;// 判断是否被将军
	BOOL IsMate(); //判断是否被将死
	int RepStatus(int nRecur = 1)const;// 检测重复局面
	void UndoMakeMove(int mv, int pcKilled)
	{
		--nStep;
		ChangeSide();
		UndoMovePiece(mv, pcKilled);
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
#ifdef ALG_CUSTOM
			valRed += piecePosValue[pc - 8][pos];
#endif // ALG_CUSTOM
		}
		else
		{
			zobr.Xor(Zobrist::Instance().Table[pc - 9][pos]);
#ifdef ALG_CUSTOM
			valBlack += piecePosValue[pc - 16][flipSquare(pos)];
#endif // ALG_CUSTOM
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
#ifdef ALG_CUSTOM
			valRed -= piecePosValue[pc - 8][pos];
#endif // ALG_CUSTOM
		}
		else
		{
			zobr.Xor(Zobrist::Instance().Table[pc - 9][pos]);
#ifdef ALG_CUSTOM
			valBlack -= piecePosValue[pc - 16][flipSquare(pos)];
#endif // ALG_CUSTOM
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