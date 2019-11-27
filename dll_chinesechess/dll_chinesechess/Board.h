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

	std::vector<int> genMvList; // ���ɵ������߷���Ŵ�
	int nStep;					// ������ڵ�Ĳ���
	std::vector<MoveStruct> mvsList; // ��ʷ�߷���Ϣ�б�
	ZobristStruct zobr;			// ��ǰ�����Zobrist��ֵ
	void SetIrrev()			// ���(��ʼ��)��ʷ�߷���Ϣ
	{
		mvsList.clear();
		mvsList.push_back({ 0,0,(BYTE)IsChecked(),zobr.key });
	}

	void Startup();// ��ʼ������
	int MovePiece(int mv);
	void UndoMovePiece(int mv, int pcKilled);
	BOOL MakeMove(int mv, int& pcKilled);
	int PlayerMove_Unchecked(int mv);//0�߷������� 1���������Ѿ���
	int PlayerMove_Checked(int mv); //-1���Ϸ� 0�߷������� 1���������Ѿ���
	// ���������߷������"isKill"Ϊ"TRUE"��ֻ���ɳ����߷�
	//int GenerateMoves(std::vector<int>& mvList, BOOL isKill = FALSE)const;
	int GenerateMoves(BOOL isKill = FALSE);
	BOOL IsLegalMove(int mv)const;
	BOOL IsChecked()const;// �ж��Ƿ񱻽���
	BOOL IsMate(); //�ж��Ƿ񱻽���
	/*A.����0����ʾû���ظ����棻
	**B.����1����ʾ�����ظ����棬��˫�����޳���(�к�)��
	**C.����3(= 1 + 2)����ʾ�����ظ����棬���������泤��(�б�����)��
	**D.����5(= 1 + 4)����ʾ�����ظ����棬�Է������泤��(�жԷ���)��
	**E.����7(= 1 + 2 + 4)����ʾ�����ظ����棬˫������(�к�)��*/
	int RepStatus(int nRecur = 1)const;	// ����ظ�����
	int GameState();					// �ҷ�����֮����ж�
	void UndoMakeMove()
	{
		--nStep;
		ChangeSide();
		UndoMovePiece(mvsList.back().mv, mvsList.back().pcKilled);
		mvsList.pop_back();
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
		// �췽���֣��ڷ�(ע��"cucvlPiecePos"ȡֵҪ�ߵ�)�ӷ�
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
	BOOL InCheck()const// �ж��Ƿ񱻽���
	{
		return mvsList.back().checked;
	}
	BOOL IsKill()const// ��һ���Ƿ����
	{
		return mvsList.back().pcKilled != 0;
	}
};