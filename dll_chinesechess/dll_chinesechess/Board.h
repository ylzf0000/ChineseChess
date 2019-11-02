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
	int player;//0��1��
	BYTE squares[256];
	int valRed, valBlack;
	int nStep;// ������ڵ�Ĳ���

	int sqSelected;// ѡ�еĸ��ӣ���һ����
	int mvLast;
	bool isFlipped;// �Ƿ�ת����

	void Init();// ��ʼ������
	int MovePiece(int mv);
	void UndoMovePiece(int mv, int pcKilled);
	bool MakeMove(int mv, int& pcKilled);
	int GenerateMoves(int* mvs)const;
	bool IsLegalMove(int mv)const;
	bool IsChecked()const;// �ж��Ƿ񱻽���
	bool IsMate();// �ж��Ƿ�ɱ
	void UndoMakeMove(int mv, int pcKilled)
	{
		--nStep;
		ChangeSide();
		UndoMovePiece(mv, pcKilled);
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