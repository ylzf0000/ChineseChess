#pragma once

#define DLL_EXPORT		__declspec(dllexport)
#define BEGIN_EXTERNC	extern "C"{
#define END_EXTERNC		}

// ���̷�Χ
constexpr int BOUNDARY_TOP = 3;
constexpr int BOUNDARY_BOTTOM = 12;
constexpr int BOUNDARY_LEFT = 3;
constexpr int BOUNDARY_RIGHT = 11;

// ���ӱ��
enum ENUM_PIECE
{
	PIECE_JIANG = 0,
	PIECE_SHI = 1,
	PIECE_XIANG = 2,
	PIECE_MA = 3,
	PIECE_JU = 4,
	PIECE_PAO = 5,
	PIECE_BING = 6,
};

// �ж������Ƿ��������е�����
constexpr char inBoard[256] = {
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,//51-59
	  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
	  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
	  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
	  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
	  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
	  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
	  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
	  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,
	  0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0,//195-203
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

// �ж������Ƿ��ھŹ�������
constexpr char inJiuGong[256] = {
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

// �жϲ����Ƿ�����ض��߷������飬1=˧(��)��2=��(ʿ)��3=��(��)
// 1�� 240, 255. 257. 272
// 2ʿ 239, 241, 271, 273
// 3�� 222, 226, 289, 290
constexpr char legalMove[512] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//0
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//16
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//32
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//48
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//64
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//80
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//96
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//0
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//0
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//0
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//160
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//0
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//0
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0,//0
	0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,//224
	1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,//240
	0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,//256
	1, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0,//272
	0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//288
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//0
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//0
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//0
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//0
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//0
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//0
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//0
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//0
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//0
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//0
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//0
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//0
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,//0
};

// ���ݲ����ж����Ƿ����ȵ�����
// �ж����߲��Ƿ�Ϸ������Ϸ��򷵻��������λ�ã����򷵻�0
constexpr char maPin[512] = {
								  0,  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,-16,  0,-16,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0, -1,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0, -1,  0,  0,  0,  1,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0, 16,  0, 16,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0
};

// ˧(��)�Ĳ���
constexpr char jiangDelta[4] = { -16,-1,1,16 };
// ��(ʿ)�Ĳ���
constexpr char shiDelta[4] = { -17,-15,15,17 };
// ��Ĳ�������˧(��)�Ĳ�����Ϊ���ȣ��̶���㣩
constexpr char maDelta[4][2] = { {-33, -31}, {-18, 14}, {-14, 18}, {31, 33} };
// �����Ĳ���������(ʿ)�Ĳ�����Ϊ���ȣ��̶��յ㣩
constexpr char maCheckDelta[4][2] = { {-33, -18}, {-31, -14}, {14, 31}, {18, 33} };
// ��������
constexpr BYTE initBoard[256] = {
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0, 20, 19, 18, 17, 16, 17, 18, 19, 20,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0, 21,  0,  0,  0,  0,  0, 21,  0,  0,  0,  0,  0,
	  0,  0,  0, 22,  0, 22,  0, 22,  0, 22,  0, 22,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0, 14,  0, 14,  0, 14,  0, 14,  0, 14,  0,  0,  0,  0,
	  0,  0,  0,  0, 13,  0,  0,  0,  0,  0, 13,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0, 12, 11, 10,  9,  8,  9, 10, 11, 12,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0
};


// RC4������������
struct RC4Struct
{
	BYTE s[256];
	int x, y;
	// �ÿ���Կ��ʼ��������������
	void InitZero()
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

// Zobrist�ṹ
struct ZobristStruct
{
	DWORD key, lock0, lock1;

	void InitZero(void) {                 // �������Zobrist
		key = lock0 = lock1 = 0;
	}
	void InitRC4(RC4Struct& rc4) {        // �����������Zobrist
		key = rc4.NextLong();
		lock0 = rc4.NextLong();
		lock1 = rc4.NextLong();
	}
	void Xor(const ZobristStruct& zobr) { // ִ��XOR����
		key ^= zobr.key;
		lock0 ^= zobr.lock0;
		lock1 ^= zobr.lock1;
	}
	void Xor(const ZobristStruct& zobr1, const ZobristStruct& zobr2) {
		key ^= zobr1.key ^ zobr2.key;
		lock0 ^= zobr1.lock0 ^ zobr2.lock0;
		lock1 ^= zobr1.lock1 ^ zobr2.lock1;
	}
};

// Zobrist��
struct Zobrist
{
	ZobristStruct Player;
	ZobristStruct Table[14][256];
	static Zobrist& Instance()
	{
		static Zobrist instance;
		return instance;
	}
	//void InitZobrist()
	//{
	//	int i, j;
	//	RC4Struct rc4;
	//	rc4.InitZero();
	//	Player.InitRC4(rc4);
	//	for (i = 0; i < 14; i++) {
	//		for (j = 0; j < 256; j++) {
	//			Table[i][j].InitRC4(rc4);
	//		}
	//	}
	//}
private:
	Zobrist()
	{
		int i, j;
		RC4Struct rc4;
		rc4.InitZero();
		Player.InitRC4(rc4);
		for (i = 0; i < 14; i++) {
			for (j = 0; j < 256; j++) {
				Table[i][j].InitRC4(rc4);
			}
		}
	}
};

// ��ʷ�߷���Ϣ(ռ4�ֽ�)
struct MoveStruct
{
	WORD mv;
	BYTE pcKilled, checked;
	DWORD key;
	//void Set(int mv, int pcCaptured, BOOL bCheck, DWORD dwKey_)
	//{
	//	wmv = mv;
	//	ucpcCaptured = pcCaptured;
	//	ucbCheck = bCheck;
	//	dwKey = dwKey_;
	//}
}; // mvs

BEGIN_EXTERNC

// �ж������Ƿ���������
inline bool isInBoard(int pos)
{
	return inBoard[pos];
}

// �ж������Ƿ��ھŹ���
inline bool isInJiuGong(int pos)
{
	return inJiuGong[pos];
}

// ��ø��ӵĺ�����
// �ȼ��� pos % 16
inline int getPosX(int pos)
{
	return pos & 15;
}

// ��ø��ӵ�������
inline int getPosY(int pos)
{
	return pos >> 4;
}

// ����������ͺ������ø���
DLL_EXPORT inline int getPos_XY(int x, int y)
{
	return x + (y << 4);
}

// ��ת����(���һ�ж��࣬������255 - 1)
inline int flipSquare(int pos)
{
	return 254 - pos;
}

// ������ˮƽ����
inline int flipX(int x)
{
	return 14 - x;
}

// �����괹ֱ����
inline int flipY(int y)
{
	return 15 - y;
}

// ����ˮƽ����
inline int mirrorXSquare(int pos)
{
	return getPos_XY(flipX(getPosX(pos)), getPosY(pos));
}

// ������ǰһ��(�����ϣ�������)
inline int squareForward(int pos, int sd) {
	return pos - 16 + (sd << 5);
}

// �߷��Ƿ����˧(��)�Ĳ���
inline bool isJiangMoveLegal(int src, int dst)
{
	return legalMove[dst - src + 256] == 1;
}

// �߷��Ƿ������(ʿ)�Ĳ���
inline bool isShiMoveLegal(int src, int dst)
{
	return legalMove[dst - src + 256] == 2;
}

// �߷��Ƿ������(��)�Ĳ���
inline bool isXiangMoveLegal(int src, int dst)
{
	return legalMove[dst - src + 256] == 3;
}

// ��(��)�۵�λ��
inline int getXiangPin(int src, int dst)
{
	return (src + dst) >> 1;
}

// ���ȵ�λ��
inline int getMaPin(int src, int dst)
{
	return src + maPin[dst - src + 256];
}

// �Ƿ�δ���� sd��0��1
inline bool isHomeHalf(int pos, int sd)
{
	return (pos & 0x80) != (sd << 7);
}

// �Ƿ��ѹ���
inline bool isAwayHomeHalf(int pos, int sd)
{
	return (pos & 0x80) == (sd << 7);
}

// �Ƿ��ںӵ�ͬһ��
inline bool isSameHalf(int src, int dst)
{
	return ((src ^ dst) & 0x80) == 0;
}

// �Ƿ���ͬһ��
inline bool isSameY(int src, int dst)
{
	return ((src ^ dst) & 0xf0) == 0;
}

// �Ƿ���ͬһ��
inline bool isSameX(int src, int dst)
{
	return ((src ^ dst) & 0x0f) == 0;
}

// ��ú�ڱ��(������8��������16)
inline int sideTag(int player)
{
	return 8 + (player << 3);
}

// ��öԷ���ڱ��
inline int oppSideTag(int player) {
	return 16 - (player << 3);
}

// ����߷������
inline int getSrc(int mv)
{
	return mv & 255;
}

// ����߷����յ�
inline int getDst(int mv)
{
	return mv >> 8;
}

// ���������յ����߷�
inline int getMove(int src, int dst)
{
	return src + (dst << 8);
}

// �߷�ˮƽ����
inline int mirrorXMove(int mv)
{
	return getMove(mirrorXSquare(getSrc(mv)), mirrorXSquare(getDst(mv)));
}

DLL_EXPORT inline BOOL isSelfChess(int player, int pc)
{
	return (sideTag(player) & pc) != 0;
}

DLL_EXPORT inline BOOL isOppChess(int player, int pc)
{
	return (oppSideTag(player) & pc) != 0;
}
END_EXTERNC