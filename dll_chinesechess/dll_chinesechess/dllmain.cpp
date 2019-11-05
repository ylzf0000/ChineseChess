﻿// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include "alg_custom.h"
#include "utility.h"
#include "Board.h"
#define DLL_EXPORT __declspec(dllexport)
Board& board = Board::Instance();

BEGIN_EXTERNC
DLL_EXPORT void start()
{
	board.Startup();
}
DLL_EXPORT void aiMove()
{
	responseMove();
}
DLL_EXPORT int playerMove(int src, int dst)
{
	return board.PlayerMove_Checked(getMove(src, dst));
}
DLL_EXPORT int getPlayer()
{
	return board.player;
}
DLL_EXPORT BYTE* getSquares()
{
	return board.squares;
}
DLL_EXPORT int getValRed()
{
	return board.valRed;
}
DLL_EXPORT int getValBlack()
{
	return board.valBlack;
}
DLL_EXPORT int getNStep()
{
	return board.nStep;
}
DLL_EXPORT int getSqSelected()
{
	return board.sqSelected;
}
DLL_EXPORT int setSqselected(int pos)
{
	int ret = isSelfChess(board.player, board.squares[pos]);
	if (ret)
		board.sqSelected = pos;
	return ret;
}
DLL_EXPORT int getMvLast()
{
	return board.mvLast;
}
DLL_EXPORT int isFlipped()
{
	return board.isFlipped;
}
DLL_EXPORT BOOL isMate()
{
	return board.IsMate();
}
END_EXTERNC


BOOL APIENTRY DllMain(HMODULE hModule,
	DWORD  ul_reason_for_call,
	LPVOID lpReserved
)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

