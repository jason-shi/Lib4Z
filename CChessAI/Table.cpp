#include "StdAfx.h"
#include "Table.h"

static void CreateBasicTable()
{
	int T1[32] = { 0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, 1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5 };
	int T2[32] = { 5, 4, 3, 3, 2, 2, 2, 2, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	int T3[32] = { 5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0, 4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0 };

	TblNumberOne = new byte[1024];
	TblFirstBit = new byte[1024];
	TblLastBit = new byte[1024];

	for (int i = 0; i < 1024; i++)
	{
		TblNumberOne[i] = T1[i >> 5] + T1[i & 31];
		TblFirstBit[i] = T2[i >> 5] == 5 ? 4 - T2[i & 31] : 9 - T2[i >> 5];
		TblLastBit[i] = T3[i & 31] == 5 ? T3[i >> 5] + 5 : T3[i & 31];
	}
	TblFirstBit[0] = 10; //强制转化为无效值
}

static void CreateExtendTable()
{
	TblLowMask = new int[10];
	TblHighMask = new int[10];

	for (int i = 0; i < 10; i++)
	{
		TblLowMask[i] = (1 << i) - 1;
		TblHighMask[i] = 1024 - (2 << i);
	}

	TblSecondBit = new byte[1024];
	TblLagendBit = new byte[1024];

	for (int i = 0; i < 1024; i++)
	{
		TblSecondBit[i] = TblFirstBit[i & ~(1 << TblFirstBit[i])];
		TblLagendBit[i] = TblLastBit[i & i - 1];
	}

	TblThirdBit = new byte[1024];
	TblLathidBit = new byte[1024];

	for (int i = 0; i < 1024; i++)
	{
		TblThirdBit[i] = TblSecondBit[i & ~(1 << TblFirstBit[i])];
		TblLathidBit[i] = TblLagendBit[i & i - 1];
	}
}

static void CreateTblBitrim()
{
	int T[10] = { 0, 1, 3, 7, 15, 31, 63, 127, 255, 511 };

	TblBitrim = new int[256];
	memset(TblBitrim, 0, sizeof(int [256]));

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
			TblBitrim[(i << 4) + j] = i > j ? T[i] ^ T[j + 1] : T[j] ^ T[i + 1];
	}
}

static void CreateTblSameLine()
{
	TblSameLine = new bool[256];
	memset(TblSameLine, 0, sizeof(bool [256]));

	for (int i = 0; i < 256; i++)
	{
		if ((i & 15) == 0 || (i >> 4) == 0)
			TblSameLine[i] = true;
	}
}

static void CreateTblHorseLeg()
{
	TblHorseLeg = new int[320];
	memset(TblHorseLeg, 0, sizeof(int [320]));

	TblHorseLeg[127] = TblHorseLeg[129] = 16;
	TblHorseLeg[142] = TblHorseLeg[174] = 1;
	TblHorseLeg[146] = TblHorseLeg[178] = -1;
	TblHorseLeg[191] = TblHorseLeg[193] = -16;
}

static void CreateTblHrsChkIdx()
{
	int B[34] = { 0,-17,-15, 17, 15,-64,-51,-49,-47,-45,-32,-19,-13,-4,-2, 2, 4,13,19,32,-66,-62,-36,-28,28,36,
				-33, -31, -14, 18, 33, 31, 14, -18 }; //8个间将位置

	TblHrsChkIdx = new byte[320];
	memset(TblHrsChkIdx, 0, sizeof(byte [320]));

	for (int i = 0; i < 34; i++)
		TblHrsChkIdx[B[i] + 160] = i;
}

static void CreateTblHorseCheck()
{
	//0，4个尖，15个双，6个单，8个间
	int B[26] = { 0,-17,-15,17,15,-64,-51,-49,-47,-45,-32,-19,-13,-4,-2, 2, 4,13,19,32,-66,-62,-36,-28,28,36 };
	int E[26] = { 0, 14, 18,31,33,-31,-33,-31,-33,-31,-14,-33,-31,14,31,33,18,31,33,14,-33,-31,-18,-14,14,18 };

	TblHrsChk11 = new int[26];
	TblHrsChk12 = new int[26];
	TblHrsChk13 = new int[26];
	TblHrsChk21 = new int[26];
	TblHrsChk22 = new int[26];
	TblHrsChk23 = new int[26];

	for (int i = 0; i < 26; i++)
	{
		int b = B[i], e = E[i];
		TblHrsChk11[i] = TblHorseLeg[b - e + 160];
		TblHrsChk12[i] = e - b;
		TblHrsChk13[i] = e - b + TblHorseLeg[e + 160];
		TblHrsChk21[i] = TblHorseLeg[e + 160];
		TblHrsChk22[i] = - e;
		TblHrsChk23[i] = TblHorseLeg[b - e + 160] - e;
	}
}

static void CreateTblBishopSphere()
{
	TblBishopSphere = new bool[160];
	memset(TblBishopSphere, 0, sizeof(bool[160]));

	TblBishopSphere[112] = true;
	TblBishopSphere[116] = true;
	TblBishopSphere[120] = true;
	TblBishopSphere[82] = true;
	TblBishopSphere[86] = true;
	TblBishopSphere[146] = true;
	TblBishopSphere[150] = true;
}

static void CreateTblGuardSphere()
{
	TblGuardSphere = new bool[160];
	memset(TblGuardSphere, 0, sizeof(bool [160]));

	TblGuardSphere[132] = true;
	TblGuardSphere[115] = true;
	TblGuardSphere[117] = true;
	TblGuardSphere[147] = true;
	TblGuardSphere[149] = true;
}

static void CreateTblPieceBitmap()
{
	int RHCMask = (1 << R_ROOK) | (1 << R_HORSE) | (1 << R_CANNON) | (1 << B_ROOK) | (1 << B_HORSE) | (1 << B_CANNON);

	TblPieceBitmap = new int[160];
	memset(TblPieceBitmap, 0, sizeof(int [160]));

	for (int i = 0; i < 160; i++)
	{
		int x = i & 0xF;
		int y = i >> 4;
		if (x < 9)
		{
			TblPieceBitmap[i] |= RHCMask; //车马炮

			if (x >= 3 && x <= 5)
			{
				if (y >= 0 && y <= 2)
					TblPieceBitmap[i] |= 1 << R_KING;
				if (y >= 7 && y <= 9)
					TblPieceBitmap[i] |= 1 << B_KING;
			}
			if (y >= 5 || y >= 3 && !(x & 1))
				TblPieceBitmap[i] |= 1 << R_PAWN;
			if (y <= 4 || y <= 6 && !(x & 1))
				TblPieceBitmap[i] |= 1 << B_PAWN;
			//关于中心对称
			if (TblBishopSphere[152 - i])
				TblPieceBitmap[i] |= 1 << R_BISHOP;
			if (TblBishopSphere[i])
				TblPieceBitmap[i] |= 1 << B_BISHOP;
			if (TblGuardSphere[152 - i])
				TblPieceBitmap[i] |= 1 << R_GUARD;
			if (TblGuardSphere[i])
				TblPieceBitmap[i] |= 1 << B_GUARD;
		}
	}
}

static void CreateTblPinByRed()
{
	TblPinByRed = new int[320];
	memset(TblPinByRed, 0, sizeof(int [320]));

	TblPinByRed[143] = 1;
	TblPinByRed[145] = 2;
	TblPinByRed[177] = 4;
	TblPinByRed[175] = 8;
	for (int i = -5; i <= -1; i++)
		TblPinByRed[i + 160] = 16;
	for (int i = 1; i <= 5; i++)
		TblPinByRed[i + 160] = 32;
	for (int i = -144; i <= -16; i += 16)
		TblPinByRed[i + 160] = 64;
}

static void CreateTblCharToPiece()
{
	TblCharToPiece = new byte[256];
	memset(TblCharToPiece, 0, sizeof(byte [256]));

	TblCharToPiece['R'] = R_ROOK;
	TblCharToPiece['N'] = R_HORSE;
	TblCharToPiece['B'] = R_BISHOP;
	TblCharToPiece['A'] = R_GUARD;
	TblCharToPiece['C'] = R_CANNON;
	TblCharToPiece['P'] = R_PAWN;
	TblCharToPiece['K'] = R_KING;
	TblCharToPiece['r'] = B_ROOK;
	TblCharToPiece['n'] = B_HORSE;
	TblCharToPiece['b'] = B_BISHOP;
	TblCharToPiece['a'] = B_GUARD;
	TblCharToPiece['c'] = B_CANNON;
	TblCharToPiece['p'] = B_PAWN;
	TblCharToPiece['k'] = B_KING;
}

static void CreateTblPieceToChar()
{
	TblPieceToChar = new char[15];

	TblPieceToChar[0] = 0;
	TblPieceToChar[R_ROOK]	 = 'R';
	TblPieceToChar[R_HORSE]	 = 'N';
	TblPieceToChar[R_BISHOP] = 'B';
	TblPieceToChar[R_GUARD]	 = 'A';
	TblPieceToChar[R_CANNON] = 'C';
	TblPieceToChar[R_PAWN]	 = 'P';
	TblPieceToChar[R_KING]	 = 'K';
	TblPieceToChar[B_ROOK]	 = 'r';
	TblPieceToChar[B_HORSE]	 = 'n';
	TblPieceToChar[B_BISHOP] = 'b';
	TblPieceToChar[B_GUARD]	 = 'a';
	TblPieceToChar[B_CANNON] = 'c';
	TblPieceToChar[B_PAWN]	 = 'p';
	TblPieceToChar[B_KING]	 = 'k';
}

static void CreateTblExclusion()
{
	TblExclusion = new int[32];

	for (int i = 0; i < 32; i++)
		TblExclusion[i] = ~(1 << i);
}

static void CreateTblConflictPawn()
{
	TblConflictPawn = new byte[160];
	memset(TblConflictPawn, 0, sizeof(byte[160]));

	for (int i = 0; i < 9; i += 2)
	{
		TblConflictPawn[i + (3 << 4)] = i + (4 << 4);
		TblConflictPawn[i + (4 << 4)] = i + (3 << 4);
		TblConflictPawn[i + (5 << 4)] = i + (6 << 4);
		TblConflictPawn[i + (6 << 4)] = i + (5 << 4);
	}
}

void CreateAllTable()
{
	::CreateBasicTable();
	::CreateExtendTable();
	::CreateTblBitrim();
	::CreateTblSameLine();
	::CreateTblHorseLeg();
	::CreateTblHrsChkIdx();
	::CreateTblHorseCheck();
	::CreateTblBishopSphere();
	::CreateTblGuardSphere();
	::CreateTblPieceBitmap();
	::CreateTblPinByRed();
	::CreateTblCharToPiece();
	::CreateTblPieceToChar();
	::CreateTblExclusion();
	::CreateTblConflictPawn();
}

void DeleteAllTable()
{
	delete TblNumberOne;
	delete TblFirstBit;
	delete TblSecondBit;
	delete TblThirdBit;
	delete TblLastBit;
	delete TblLagendBit;
	delete TblLathidBit;
	delete TblHighMask;
	delete TblLowMask;
	delete TblBitrim;
	delete TblSameLine;
	delete TblHorseLeg;
	delete TblHrsChkIdx;
	delete TblHrsChk11;
	delete TblHrsChk12;
	delete TblHrsChk13;
	delete TblHrsChk21;
	delete TblHrsChk22;
	delete TblHrsChk23;
	delete TblBishopSphere;
	delete TblGuardSphere;
	delete TblPieceBitmap;
	delete TblPinByRed;
	delete TblCharToPiece;
	delete TblPieceToChar;
	delete TblExclusion;
	delete TblConflictPawn;
}
