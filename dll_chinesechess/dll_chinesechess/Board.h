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
	int player;					// 0��1��
	BYTE squares[256];			// �����ϵ�����
#ifdef ALG_CUSTOM
	int valRed, valBlack;		// �졢��˫����������ֵ
#endif // ALG_CUSTOM

	int nStep;					// ������ڵ�Ĳ���
	std::vector<MoveStruct> mvsList; // ��ʷ�߷���Ϣ�б�
	ZobristStruct zobr;			// Zobrist
	void SetIrrev()			// ���(��ʼ��)��ʷ�߷���Ϣ
	{
		mvsList.clear();
		mvsList.push_back({ 0,0,IsChecked(),zobr.dwKey });
	}

	int sqSelected;// ѡ�еĸ��ӣ���һ����
	int mvLast;
	BOOL isFlipped;// �Ƿ�ת����

	void Startup();// ��ʼ������
	int MovePiece(int mv);
	void UndoMovePiece(int mv, int pcKilled);
	bool MakeMove(int mv, int& pcKilled);
	int PlayerMove_Unchecked(int mv);//0�߷������� 1���������Ѿ���
	int PlayerMove_Checked(int mv); //-1���Ϸ� 0�߷������� 1���������Ѿ���
	int GenerateMoves(int* mvs)const;
	bool IsLegalMove(int mv)const;
	bool IsChecked()const;// �ж��Ƿ񱻽���
	bool IsMate(); //�ж��Ƿ񱻽���
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
	// �������Ϸ�һö����
	void AddPiece(int pos, int pc)
	{
		squares[pos] = pc;

		// �췽�ӷ֣��ڷ�(ע��"cucvlPiecePos"ȡֵҪ�ߵ�)����
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
	// ������������һö����
	void DelPiece(int pos, int pc)
	{
		squares[pos] = 0;
#ifdef ALG_CUSTOM
		// �췽���֣��ڷ�(ע��"cucvlPiecePos"ȡֵҪ�ߵ�)�ӷ�
		if (pc < 16)
			valRed -= piecePosValue[pc - 8][pos];
		else
			valBlack -= piecePosValue[pc - 16][flipSquare(pos)];
#endif // ALG_CUSTOM
	}
	int Evaluate() const
	{
		return ((player == 0) ? valRed - valBlack : valBlack - valRed)
			+ ADVANCED_VALUE;
	}
};