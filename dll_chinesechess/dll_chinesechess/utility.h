#pragma once

#define DLL_EXPORT		__declspec(dllexport)
#define BEGIN_EXTERNC	extern "C"{
#define END_EXTERNC		}

// 棋盘范围
constexpr int BOUNDARY_TOP = 3;
constexpr int BOUNDARY_BOTTOM = 12;
constexpr int BOUNDARY_LEFT = 3;
constexpr int BOUNDARY_RIGHT = 11;

// 棋子编号
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

// 判断棋子是否在棋盘中的数组
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

// 判断棋子是否在九宫的数组
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

// 判断步长是否符合特定走法的数组，1=帅(将)，2=仕(士)，3=相(象)
// 1将 240, 255. 257. 272
// 2士 239, 241, 271, 273
// 3象 222, 226, 289, 290
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

// 根据步长判断马是否蹩腿的数组
// 判断马走步是否合法，若合法则返回马腿相对位置，否则返回0
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

// 帅(将)的步长
constexpr char jiangDelta[4] = { -16,-1,1,16 };
// 仕(士)的步长
constexpr char shiDelta[4] = { -17,-15,15,17 };
// 马的步长，以帅(将)的步长作为马腿（固定起点）
constexpr char maDelta[4][2] = { {-33, -31}, {-18, 14}, {-14, 18}, {31, 33} };
// 马将军的步长，以仕(士)的步长作为马腿（固定终点）
constexpr char maCheckDelta[4][2] = { {-33, -18}, {-31, -14}, {14, 31}, {18, 33} };
// 开局棋盘
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


// RC4密码流生成器
struct RC4Struct
{
	BYTE s[256];
	int x, y;
	// 用空密钥初始化密码流生成器
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
	// 生成密码流的下一个字节
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
	// 生成密码流的下四个字节
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

// Zobrist结构
struct ZobristStruct
{
	DWORD key, lock0, lock1;

	void InitZero(void) {                 // 用零填充Zobrist
		key = lock0 = lock1 = 0;
	}
	void InitRC4(RC4Struct& rc4) {        // 用密码流填充Zobrist
		key = rc4.NextLong();
		lock0 = rc4.NextLong();
		lock1 = rc4.NextLong();
	}
	void Xor(const ZobristStruct& zobr) { // 执行XOR操作
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

// Zobrist表
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

// 历史走法信息(占4字节)
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

// 判断棋子是否在棋盘中
inline bool isInBoard(int pos)
{
	return inBoard[pos];
}

// 判断棋子是否在九宫中
inline bool isInJiuGong(int pos)
{
	return inJiuGong[pos];
}

// 获得格子的横坐标
// 等价于 pos % 16
inline int getPosX(int pos)
{
	return pos & 15;
}

// 获得格子的纵坐标
inline int getPosY(int pos)
{
	return pos >> 4;
}

// 根据纵坐标和横坐标获得格子
DLL_EXPORT inline int getPos_XY(int x, int y)
{
	return x + (y << 4);
}

// 翻转格子(最后一列多余，所以是255 - 1)
inline int flipSquare(int pos)
{
	return 254 - pos;
}

// 横坐标水平镜像
inline int flipX(int x)
{
	return 14 - x;
}

// 纵坐标垂直镜像
inline int flipY(int y)
{
	return 15 - y;
}

// 格子水平镜像
inline int mirrorXSquare(int pos)
{
	return getPos_XY(flipX(getPosX(pos)), getPosY(pos));
}

// 格子向前一步(红向上，黑向下)
inline int squareForward(int pos, int sd) {
	return pos - 16 + (sd << 5);
}

// 走法是否符合帅(将)的步长
inline bool isJiangMoveLegal(int src, int dst)
{
	return legalMove[dst - src + 256] == 1;
}

// 走法是否符合仕(士)的步长
inline bool isShiMoveLegal(int src, int dst)
{
	return legalMove[dst - src + 256] == 2;
}

// 走法是否符合相(象)的步长
inline bool isXiangMoveLegal(int src, int dst)
{
	return legalMove[dst - src + 256] == 3;
}

// 相(象)眼的位置
inline int getXiangPin(int src, int dst)
{
	return (src + dst) >> 1;
}

// 马腿的位置
inline int getMaPin(int src, int dst)
{
	return src + maPin[dst - src + 256];
}

// 是否未过河 sd红0黑1
inline bool isHomeHalf(int pos, int sd)
{
	return (pos & 0x80) != (sd << 7);
}

// 是否已过河
inline bool isAwayHomeHalf(int pos, int sd)
{
	return (pos & 0x80) == (sd << 7);
}

// 是否在河的同一边
inline bool isSameHalf(int src, int dst)
{
	return ((src ^ dst) & 0x80) == 0;
}

// 是否在同一行
inline bool isSameY(int src, int dst)
{
	return ((src ^ dst) & 0xf0) == 0;
}

// 是否在同一列
inline bool isSameX(int src, int dst)
{
	return ((src ^ dst) & 0x0f) == 0;
}

// 获得红黑标记(红子是8，黑子是16)
inline int sideTag(int player)
{
	return 8 + (player << 3);
}

// 获得对方红黑标记
inline int oppSideTag(int player) {
	return 16 - (player << 3);
}

// 获得走法的起点
inline int getSrc(int mv)
{
	return mv & 255;
}

// 获得走法的终点
inline int getDst(int mv)
{
	return mv >> 8;
}

// 根据起点和终点获得走法
inline int getMove(int src, int dst)
{
	return src + (dst << 8);
}

// 走法水平镜像
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