#include "pch.h"
#include "RC4Struct.h"

// �ÿ���Կ��ʼ��������������
void RC4Struct::InitZero()
{
	int i, j;
	BYTE uc;

	x = y = j = 0;
	for (i = 0; i < 256; i++) {
		s[i] = i;
	}
	for (i = 0; i < 256; i++) {
		j = (j + s[i]) & 255;
		uc = s[i];
		s[i] = s[j];
		s[j] = uc;
	}
}
