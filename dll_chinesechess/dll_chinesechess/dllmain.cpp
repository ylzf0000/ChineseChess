// dllmain.cpp : 定义 DLL 应用程序的入口点。
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
DLL_EXPORT int aiMove()
{
	return responseMove();
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
DLL_EXPORT BOOL isMate()
{
	return board.IsMate();
}
DLL_EXPORT int getRepStatus()
{
	return board.RepStatus();
}
DLL_EXPORT int getGameState()
{
	return board.GameState();
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

