#pragma once
#include <wtypes.h>
// RC4������������
struct RC4Struct
{
	BYTE s[256];
	int x, y;
	// �ÿ���Կ��ʼ��������������
	void InitZero();
	// ��������������һ���ֽ�
	BYTE NextByte()
	{
		BYTE uc;
		x = (x + 1) & 255;
		y = (y + s[x]) & 255;
		uc = s[x];
		s[x] = s[y];
		s[y] = uc;
		return s[(s[x] + s[y]) & 255];
	}
	// ���������������ĸ��ֽ�
	DWORD NextLong()
	{
		BYTE uc0, uc1, uc2, uc3;
		uc0 = NextByte();
		uc1 = NextByte();
		uc2 = NextByte();
		uc3 = NextByte();
		return uc0 + (uc1 << 8) + (uc2 << 16) + (uc3 << 24);
	}
};