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
	int player;					// 0��1��
	BYTE squares[256];			// �����ϵ�����
	int valRed, valBlack;		// �졢��˫����������ֵ

	int nStep;					// ������ڵ�Ĳ���
	std::vector<MoveStruct> mvsList; // ��ʷ�߷���Ϣ�б�
	ZobristStruct zobr;			// ��ǰ�����Zobrist��ֵ
	void ClearBoard()
	{
		player = valRed = valBlack = nStep = 0;
		memset(squares, 0, sizeof squares);
		zobr.InitZero();
	}
	void SetIrrev()			// ���(��ʼ��)��ʷ�߷���Ϣ
	{
		mvsList.clear();
		mvsList.push_back({ 0,0,(BYTE)IsChecked(),zobr.key });
	}

	void Startup();// ��ʼ������
	int MovePiece(int mv);
	void UndoMovePiece(int mv, int pcKilled);
	BOOL MakeMove(int mv);
	int PlayerMove_Unchecked(int mv);//0�߷������� 1���������Ѿ���
	int PlayerMove_Checked(int mv); //-1���Ϸ� 0�߷������� 1���������Ѿ���
	// ���������߷������"isKill"Ϊ"TRUE"��ֻ���ɳ����߷�
	int GenerateMoves(std::vector<int>& mvList, BOOL isKill = FALSE)const;
	BOOL IsLegalMove(int mv)const;
	BOOL IsChecked()const;// �ж��Ƿ񱻽���
	BOOL IsMate(); //�ж��Ƿ񱻽���
	/*A.����0����ʾû���ظ����棻
	**B.����1����ʾ�����ظ����棬��˫�����޳���(�к�)��
	**C.����3(= 1 + 2)����ʾ�����ظ����棬���������泤��(�б�����)��
	**D.����5(= 1 + 4)����ʾ�����ظ����棬�Է������泤��(�жԷ���)��
	**E.����7(= 1 + 2 + 4)����ʾ�����ظ����棬˫������(�к�)��*/
	int RepStatus(int nRecur = 1)const;	// ����ظ�����
	int RepValue(int nRepStatus)const   // �ظ������ֵ
	{
		int vlReturn;
		vlReturn = ((nRepStatus & 2) == 0 ? 0 : mvsList.size() - BAN_VALUE) +
			((nRepStatus & 4) == 0 ? 0 : BAN_VALUE - mvsList.size());
		return vlReturn == 0 ? DrawValue() : vlReturn;
	}
	BOOL NullOkay()const// �ж��Ƿ�����ղ��ü�
	{
		return (player == 0 ? valRed : valBlack) > NULL_MARGIN;
	}
	void Mirror(Board& mirror)const;// �Ծ��澵��
	int GameState();					// �ҷ�����֮����ж�
	void UndoMakeMove()
	{
		--nStep;
		ChangeSide();
		UndoMovePiece(mvsList.back().mv, mvsList.back().pcKilled);
		mvsList.pop_back();
	}
	void NullMove() // ��һ���ղ�
	{
		DWORD dwKey = zobr.key;
		ChangeSide();
		mvsList.push_back({ 0, 0, FALSE, dwKey });
		++nStep;
	}
	void UndoNullMove() // �����ղ�
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
	// �������Ϸ�һö����
	void AddPiece(int pos, int pc)
	{
		squares[pos] = pc;
		// �췽�ӷ֣��ڷ�(ע��"cucvlPiecePos"ȡֵҪ�ߵ�)����
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
	// ������������һö����
	void DelPiece(int pos, int pc)
	{
		squares[pos] = 0;
		// �췽���֣��ڷ�(ע��"cucvlPiecePos"ȡֵҪ�ߵ�)�ӷ�
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
	BOOL InCheck()const// �ж��Ƿ񱻽���
	{
		return mvsList.back().checked;
	}
	BOOL IsKill()const// ��һ���Ƿ����
	{
		return mvsList.back().pcKilled != 0;
	}
};