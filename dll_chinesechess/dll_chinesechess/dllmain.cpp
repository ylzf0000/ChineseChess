// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "pch.h"
#include "Alg.h"
#define DLL_EXPORT __declspec(dllexport)
extern "C"
{
	DLL_EXPORT void AIMove()
	{
		ResponseMove();
	}
	DLL_EXPORT void Start()
	{
		Board& board = Board::Instance();
		board.Init();
	}
	DLL_EXPORT Board GetBoard()
	{
		return Board::Instance();
	}
}


BOOL APIENTRY DllMain( HMODULE hModule,
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

