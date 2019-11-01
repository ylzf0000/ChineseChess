#pragma once
#include "pch.h"
#include "utility.h"
struct Board
{
	int player;//0��1��
	BYTE squares[256];
	int valRed, valBlack;
	int dist;

	void Init();// ��ʼ������
	int MovePiece(int mv);
	void UndoMovePiece(int mv, int pcKilled);
	bool MakeMove(int mv, int& pcKilled);
	int GenerateMoves(int* mvs)const;
	bool IsLegalMove(int mv)const;
	bool IsChecked()const;
	bool IsMate();
	void UndoMakeMove(int mv, int pcKilled)
	{
		--dist;
		ChangeSide();
		UndoMakeMove(mv, pcKilled);
	}
	void ChangeSide() {
		player = 1 - player;
	}
	// �������Ϸ�һö����
	void AddPiece(int pos, int pc)
	{
		squares[pos] = pc;
		// �췽�ӷ֣��ڷ�(ע��"cucvlPiecePos"ȡֵҪ�ߵ�)����
		if (pc < 16)
			valRed += piecePosValue[pc - 8][pos];
		else
			valBlack += piecePosValue[pc - 16][flipSquare(pos)];
	}
	// ������������һö����
	void DelPiece(int pos, int pc)
	{
		squares[pos] = 0;
		// �췽���֣��ڷ�(ע��"cucvlPiecePos"ȡֵҪ�ߵ�)�ӷ�
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