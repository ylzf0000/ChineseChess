#pragma once
#include "pch.h"
#include "utility.h"
struct Board
{
	static Board& Instance()
	{
		static Board board;
		return board;
	}
	int player;//0红1黑
	BYTE squares[256];
	int valRed, valBlack;
	int nStep;// 距离根节点的步数

	int sqSelected;// 选中的格子，上一步棋
	int mvLast;
	bool isFlipped;// 是否翻转棋盘

	void Init();// 初始化棋盘
	int MovePiece(int mv);
	void UndoMovePiece(int mv, int pcKilled);
	bool MakeMove(int mv, int& pcKilled);
	int GenerateMoves(int* mvs)const;
	bool IsLegalMove(int mv)const;
	bool IsChecked()const;// 判断是否被将军
	bool IsMate();// 判断是否被杀
	void UndoMakeMove(int mv, int pcKilled)
	{
		--nStep;
		ChangeSide();
		UndoMovePiece(mv, pcKilled);
	}
	void ChangeSide() {
		player = 1 - player;
	}
	// 在棋盘上放一枚棋子
	void AddPiece(int pos, int pc)
	{
		squares[pos] = pc;
		// 红方加分，黑方(注意"cucvlPiecePos"取值要颠倒)减分
		if (pc < 16)
			valRed += piecePosValue[pc - 8][pos];
		else
			valBlack += piecePosValue[pc - 16][flipSquare(pos)];
	}
	// 从棋盘上拿走一枚棋子
	void DelPiece(int pos, int pc)
	{
		squares[pos] = 0;
		// 红方减分，黑方(注意"cucvlPiecePos"取值要颠倒)加分
		if (pc < 16)
			valRed -= piecePosValue[pc - 8][pos];
		else
			valBlack -= piecePosValue[pc - 16][flipSquare(pos)];
	}
	int Evaluate() const
	{
		return ((player == 0) ? valRed - valBlack : valBlack - valRed)
			+ ADVANCED_VALUE;
	}
};