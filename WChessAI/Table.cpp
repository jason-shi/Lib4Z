#include "StdAfx.h"
#include "Table.h"

static void CreateTblBitIndex()
{
	TblBitIndex = new int[67];
	memset(TblBitIndex, 0, sizeof(int[67]));

	qword T[67]  = { 0 };
	for (int i = 0; i < 64; i++)
	{
		T[i] = 1;
		for (int j = 0; j < i; j++)
			T[i] = (T[i] << 1) % 67;
	}

	for (int i = 0; i < 67; i++)
	{
		if (T[i] == 0)
			continue;
		TblBitIndex[T[i]] = i;
	}
}

static void CreateTblPieceBitmap()
{
	int RHBQKMask = (1 << R_ROOK) | (1 << R_HORSE) | (1 << R_BISHOP) | (1 << R_QUEEN) | (1 << R_KING)
				  | (1 << B_ROOK) | (1 << B_HORSE) | (1 << B_BISHOP) | (1 << B_QUEEN) | (1 << B_KING);

	TblPieceBitmap = new int[64];
	memset(TblPieceBitmap, 0, sizeof(int [64]));

	for (int i = 0; i < 64; i++)
	{
		if ((i >> 3) != 0 && (i >> 3) != 7)
		{
			TblPieceBitmap[i] |= 1 << R_PAWN;
			TblPieceBitmap[i] |= 1 << B_PAWN;
		}
		TblPieceBitmap[i] |= RHBQKMask; //车马相后王
	}
}

static void CreateTblCharToPiece()
{
	TblCharToPiece = new byte[256];
	memset(TblCharToPiece, 0, sizeof(byte [256]));

	TblCharToPiece['R'] = R_ROOK;
	TblCharToPiece['N'] = R_HORSE;
	TblCharToPiece['B'] = R_BISHOP;
	TblCharToPiece['Q'] = R_QUEEN;
	TblCharToPiece['P'] = R_PAWN;
	TblCharToPiece['K'] = R_KING;
	TblCharToPiece['r'] = B_ROOK;
	TblCharToPiece['n'] = B_HORSE;
	TblCharToPiece['b'] = B_BISHOP;
	TblCharToPiece['q'] = B_QUEEN;
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
	TblPieceToChar[R_QUEEN]  = 'Q';
	TblPieceToChar[R_PAWN]	 = 'P';
	TblPieceToChar[R_KING]	 = 'K';
	TblPieceToChar[B_ROOK]	 = 'r';
	TblPieceToChar[B_HORSE]	 = 'n';
	TblPieceToChar[B_BISHOP] = 'b';
	TblPieceToChar[B_QUEEN]  = 'q';
	TblPieceToChar[B_PAWN]	 = 'p';
	TblPieceToChar[B_KING]	 = 'k';
}

static void CreateTblQueenPath()
{
	TblQueenPath = new qword[4096];
	memset(TblQueenPath, 0, sizeof(qword[4096]));

	for (int i = 0; i < 64; i++)
	{
		int x1 = i & 7;
		int y1 = i >> 3;
		for (int j = 0; j < 64; j++)
		{
			int x2 = j & 7;
			int y2 = j >> 3;

			if (x1 == x2 && y1 == y2)
				continue;
			//在同一水平线上
			else if (y1 == y2)
			{
				for (int k = min(x1, x2) + 1; k < max(x1, x2); k++)
					TblQueenPath[(i << 6) + j] |= (qword)1 << ((y1 << 3) + k);
			}
			//在同一垂直线上
			else if (x1 == x2)
			{
				for (int k = min(y1, y2) + 1; k < max(y1, y2); k++)
					TblQueenPath[(i << 6) + j] |= (qword)1 << ((k << 3) + x1);
			}
			//在同一斜线上
			else if (x1 - x2 == y1 - y2)
			{
				for (int k = min(x1, x2) + 1; k < max(x1, x2); k++)
					TblQueenPath[(i << 6) + j] |= (qword)1 << (((min(y1, y2) + k - min(x1, x2)) << 3) + k);
			}
			//在同一反斜线上
			else if (x1 - x2 == y2 - y1)
			{
				for (int k = min(x1, x2) + 1; k < max(x1, x2); k++)
					TblQueenPath[(i << 6) + j] |= (qword)1 << (((max(y1, y2) - k + min(x1, x2)) << 3) + k);
			}
			else
				TblQueenPath[(i << 6) + j] = (qword)-1;
		}
	}
}

static void CreateTblRookPath()
{
	TblRookPath = new qword[4096];
	memset(TblRookPath, 0, sizeof(qword[4096]));

	for (int i = 0; i < 64; i++)
	{
		int x1 = i & 7;
		int y1 = i >> 3;
		for (int j = 0; j < 64; j++)
		{
			int x2 = j & 7;
			int y2 = j >> 3;

			if (x1 == x2 && y1 == y2)
				continue;
			//在同一水平线上
			else if (y1 == y2)
			{
				for (int k = min(x1, x2) + 1; k < max(x1, x2); k++)
					TblRookPath[(i << 6) + j] |= (qword)1 << ((y1 << 3) + k);
			}
			//在同一垂直线上
			else if (x1 == x2)
			{
				for (int k = min(y1, y2) + 1; k < max(y1, y2); k++)
					TblRookPath[(i << 6) + j] |= (qword)1 << ((k << 3) + x1);
			}
			else
				TblRookPath[(i << 6) + j] = (qword)-1;
		}
	}
}

static void CreateTblBishopPath()
{
	TblBishopPath = new qword[4096];
	memset(TblBishopPath, 0, sizeof(qword[4096]));

	for (int i = 0; i < 64; i++)
	{
		int x1 = i & 7;
		int y1 = i >> 3;
		for (int j = 0; j < 64; j++)
		{
			int x2 = j & 7;
			int y2 = j >> 3;

			if (x1 == x2 && y1 == y2)
				continue;
			//在同一斜线上
			else if (x1 - x2 == y1 - y2)
			{
				for (int k = min(x1, x2) + 1; k < max(x1, x2); k++)
					TblBishopPath[(i << 6) + j] |= (qword)1 << (((min(y1, y2) + k - min(x1, x2)) << 3) + k);
			}
			//在同一反斜线上
			else if (x1 - x2 == y2 - y1)
			{
				for (int k = min(x1, x2) + 1; k < max(x1, x2); k++)
					TblBishopPath[(i << 6) + j] |= (qword)1 << (((max(y1, y2) - k + min(x1, x2)) << 3) + k);
			}
			else
				TblBishopPath[(i << 6) + j] = (qword)-1;
		}
	}
}

static void CreateTblDirection()
{
	TblDirection = new int[4096];
	memset(TblDirection, 0, sizeof(int[4096]));

	for (int i = 0; i < 64; i++)
	{
		int x1 = i & 7;
		int y1 = i >> 3;
		for (int j = 0; j < 64; j++)
		{
			int x2 = j & 7;
			int y2 = j >> 3;

			if (y1 == y2)
				TblDirection[(i << 6) + j] = 1;
			if (x1 == x2)
				TblDirection[(i << 6) + j] = 2;
			if (x1 - x2 == y1 - y2)
				TblDirection[(i << 6) + j] = 3;
			if (x1 - x2 == y2 - y1)
				TblDirection[(i << 6) + j] = 4;
		}
	}
}

static void CreateTblRookCrossGrid()
{
	for (int i = 0; i < 2; i++)
	{
		TblRookCrossGrid[i] = new byte[4096];
		memset(TblRookCrossGrid[i], 0x40, sizeof(byte[4096]));
	}

	for (int i = 0; i < 64; i++)
	{
		int x1 = i & 7;
		int y1 = i >> 3;
		for (int j = 0; j < 64; j++)
		{
			int x2 = j & 7;
			int y2 = j >> 3;
			TblRookCrossGrid[0][(i << 6) + j] = (y1 << 3) + x2;
			TblRookCrossGrid[1][(i << 6) + j] = (y2 << 3) + x1;
		}
	}
}

static void CreateTblBishopCrossGrid()
{
	for (int i = 0; i < 2; i++)
	{
		TblBishopCrossGrid[i] = new byte[4096];
		memset(TblBishopCrossGrid[i], 0x40, sizeof(byte[4096]));
	}

	for (int i = 0; i < 64; i++)
	{
		int x1 = i & 7;
		int y1 = i >> 3;
		for (int j = 0; j < 64; j++)
		{
			int x2 = j & 7;
			int y2 = j >> 3;

			for (int k = 0; k < 64; k++)
			{
				int xk = k & 7;
				int yk = k >> 3;
				if (abs(x1 - xk) == abs(y1 - yk) && abs(x2 - xk) == abs(y2 - yk))
				{
					if (TblBishopCrossGrid[0][(i << 6) + j] == 0x40)
						TblBishopCrossGrid[0][(i << 6) + j] = k;
					else
						TblBishopCrossGrid[1][(i << 6) + j] = k;
				}
			}
		}
	}
}

static void CreateTblRookCrossPath()
{
	for (int i = 0; i < 2; i++)
	{
		TblRookCrossPath[i] = new qword[4096];
		memset(TblRookCrossPath[i], 0xFF, sizeof(qword[4096]));
	}

	for (int i = 0; i < 64; i++)
	{
		int x1 = i & 7;
		int y1 = i >> 3;
		for (int j = 0; j < 64; j++)
		{
			int x2 = j & 7;
			int y2 = j >> 3;
			if (x1 == x2 || y1 == y2)
				continue;
			//不在同一直线上
			int intersect1 = TblRookCrossGrid[0][(i << 6) + j];
			if (intersect1 != 0x40)
			{
				TblRookCrossPath[0][(i << 6) + j] = TblRookPath[(i << 6) + intersect1]
												  | TblRookPath[(j << 6) + intersect1];
			}
			//不在同一直线上
			int intersect2 = TblRookCrossGrid[1][(i << 6) + j];
			if (intersect2 != 0x40)
			{
				TblRookCrossPath[1][(i << 6) + j] = TblRookPath[(i << 6) + intersect2]
												  | TblRookPath[(j << 6) + intersect2];
			}
		}
	}
}

static void CreateTblBishopCrossPath()
{
	for (int i = 0; i < 2; i++)
	{
		TblBishopCrossPath[i] = new qword[4096];
		memset(TblBishopCrossPath[i], 0xFF, sizeof(qword[4096]));
	}

	for (int i = 0; i < 64; i++)
	{
		int x1 = i & 7;
		int y1 = i >> 3;
		for (int j = 0; j < 64; j++)
		{
			int x2 = j & 7;
			int y2 = j >> 3;
			if (x1 - x2 == y1 - y2 || x1 - x2 == y2 - y1)
				continue;
			//不在同一斜线上
			int intersect3 = TblBishopCrossGrid[0][(i << 6) + j];
			if (intersect3 != 0x40)
			{
				TblBishopCrossPath[0][(i << 6) + j] = TblBishopPath[(i << 6) + intersect3]
													| TblBishopPath[(j << 6) + intersect3];
			}
			//不在同一斜线上
			int intersect4 = TblBishopCrossGrid[1][(i << 6) + j];
			if (intersect4 != 0x40)
			{
				TblBishopCrossPath[1][(i << 6) + j] = TblBishopPath[(i << 6) + intersect4]
													| TblBishopPath[(j << 6) + intersect4];
			}
		}
	}
}

static void CreateTblQueenCrossGridBtmp()
{
	TblQueenCrossGridBtmp = new qword[4096];
	memset(TblQueenCrossGridBtmp, 0, sizeof(qword[4096]));

	for (int i = 0; i < 64; i++)
	{
		for (int j = 0; j < 64; j++)
		{
			for (int k = 0; k < 64; k++)
			{
				if (TblQueenPath[(i << 6) + k] != (qword)-1 && TblQueenPath[(j << 6) + k] != (qword)-1)
				{
					if (TblQueenPath[(i << 6) + j] != (qword)-1)
					{
						//同一直线上必须离开直线
						if ((TblQueenPath[(i << 6) + j] & ((qword)1 << k)) != 0
							|| (TblQueenPath[(i << 6) + k] & ((qword)1 << j)) != 0
							|| (TblQueenPath[(j << 6) + k] & ((qword)1 << i)) != 0)
							continue;
					}
					TblQueenCrossGridBtmp[(i << 6) + j] |= (qword)1 << k;
				}
			}
		}
	}
}

static void CreateTblHorseCheck()
{
	TblHorseChkGrid1 = new byte[4096];
	memset(TblHorseChkGrid1, 0x40, sizeof(byte [4096]));
	TblHorseChkGrid2 = new byte[4096];
	memset(TblHorseChkGrid2, 0x40, sizeof(byte [4096]));

	for (int i = 0; i < 64; i++)
	{
		int x1 = i & 7;
		int y1 = i >> 3;
		for (int j = 0; j < 64; j++)
		{
			int x2 = j & 7;
			int y2 = j >> 3;
			for (int pos = 0; pos < 64; pos++)
			{
				int px = pos & 7;
				int py = pos >> 3;
				if ((abs(x1 - px) == 2 && abs(y1 - py) == 1 || abs(x1 - px) == 1 && abs(y1 - py) == 2)
					&& (abs(x2 - px) == 2 && abs(y2 - py) == 1 || abs(x2 - px) == 1 && abs(y2 - py) == 2))
				{
					if (TblHorseChkGrid1[(i << 6) + j] == 0x40)
						TblHorseChkGrid1[(i << 6) + j] = pos;
					else
						TblHorseChkGrid2[(i << 6) + j] = pos;
				}
			}
		}
	}
}

static void CreateTblHorseChkIdx()
{
	TblHorseChkIdx = new byte[4096];
	memset(TblHorseChkIdx, 0, sizeof(byte [4096]));

	for (int i = 0; i < 64; i++)
	{
		int x1 = i & 7;
		int y1 = i >> 3;
		for (int j = 0; j < 64; j++)
		{
			int x2 = j & 7;
			int y2 = j >> 3;
			if (abs(x1 - x2) == 2 && abs(y1 - y2) == 1 || abs(x1 - x2) == 1 && abs(y1 - y2) == 2)
				TblHorseChkIdx[(i << 6) + j] = 1;
		}
	}
}

static void CreateTblKingChkIdx()
{
	TblKingChkIdx = new byte[4096];
	memset(TblKingChkIdx, 0, sizeof(byte[4096]));

	for (int i = 0; i < 64; i++)
	{
		int x1 = i & 7;
		int y1 = i >> 3;
		for (int j = 0; j < 64; j++)
		{
			int x2 = j & 7;
			int y2 = j >> 3;
			if (abs(x1 - x2) <= 1 && abs(y1 - y2) <= 1)
				TblKingChkIdx[(i << 6) + j] = 1;
		}
	}
}

static void CreateTblEludeChkIdx()
{
	TblEludeChkIdx = new byte[4096];
	memset(TblEludeChkIdx, 0, sizeof(byte[4096]));

	for (int i = 0; i < 64; i++)
	{
		int x1 = i & 7;
		int y1 = i >> 3;
		for (int j = 0; j < 64; j++)
		{
			int x2 = j & 7;
			int y2 = j >> 3;
			if (y1 == y2)
			{
				if (abs(x1 - x2) > 1)
					TblEludeChkIdx[(i << 6) + j] = 1;
			}
			else if (x1 == x2)
			{
				if (abs(y1 - y2) > 1)
					TblEludeChkIdx[(i << 6) + j] = 2;
			}
			else if (x1 - x2 == y1 - y2)
			{
				if (abs(x1 - x2) > 1)
					TblEludeChkIdx[(i << 6) + j] = 3;
			}
			else if (x1 - x2 == y2 - y1)
			{
				if (abs(x1 - x2) > 1)
					TblEludeChkIdx[(i << 6) + j] = 4;
			}
			if (TblKingChkIdx[(i << 6) + j] == 1)
			{
				if (x2 - x1 == 1 && y2 - y1 == 1)
					TblEludeChkIdx[(i << 6) + j] = 0 + 5;
				if (x2 - x1 == 1 && y2 - y1 == 0)
					TblEludeChkIdx[(i << 6) + j] = 1 + 5;
				if (x2 - x1 == 1 && y2 - y1 == -1)
					TblEludeChkIdx[(i << 6) + j] = 2 + 5;
				if (x2 - x1 == 0 && y2 - y1 == -1)
					TblEludeChkIdx[(i << 6) + j] = 3 + 5;
				if (x2 - x1 == -1 && y2 - y1 == -1)
					TblEludeChkIdx[(i << 6) + j] = 4 + 5;
				if (x2 - x1 == -1 && y2 - y1 == 0)
					TblEludeChkIdx[(i << 6) + j] = 5 + 5;
				if (x2 - x1 == -1 && y2 - y1 == 1)
					TblEludeChkIdx[(i << 6) + j] = 6 + 5;
				if (x2 - x1 == 0 && y2 - y1 == 1)
					TblEludeChkIdx[(i << 6) + j] = 7 + 5;
			}
		}
	}
}

static void CreateTblRookBlock()
{
	TblRookBlock1 = new byte[262144];
	memset(TblRookBlock1, 0x40, sizeof(byte[262144]));
	TblRookBlock2 = new byte[262144];
	memset(TblRookBlock2, 0x40, sizeof(byte[262144]));

	for (int i = 0; i < 64; i++)
	{
		for (int j = 0; j < 64; j++)
		{
			if (TblRookPath[(i << 6) + j] != (qword)-1)
			{
				for(int k = 0; k < 64; k++)
				{
					if ((TblRookPath[(i << 6) + j] & ((qword)1 << k)) == 0)
						continue;
					for (int pos = 0; pos < 64; pos++)
					{
						if (TblRookPath[(k << 6) + pos] == (qword)-1)
							continue;
						TblRookBlock1[(((i << 6) + j) << 6) + pos] = k;
					}
				}
			}
			if (TblBishopPath[(i << 6) + j] != (qword)-1)
			{
				for(int k = 0; k < 64; k++)
				{
					if ((TblBishopPath[(i << 6) + j] & ((qword)1 << k)) == 0)
						continue;
					for (int pos = 0; pos < 64; pos++)
					{
						if (TblRookPath[(k << 6) + pos] == (qword)-1)
							continue;
						if (TblRookBlock1[(((i << 6) + j) << 6) + pos] == 0x40)
							TblRookBlock1[(((i << 6) + j) << 6) + pos] = k;
						else
							TblRookBlock2[(((i << 6) + j) << 6) + pos] = k;
					}
				}
			}
		}
	}
}

static void CreateTblHorseBlock()
{
	TblHorseBlock1 = new byte[262144];
	memset(TblHorseBlock1, 0x40, sizeof(byte[262144]));
	TblHorseBlock2 = new byte[262144];
	memset(TblHorseBlock2, 0x40, sizeof(byte[262144]));

	for (int i = 0; i < 64; i++)
	{
		for (int j = 0; j < 64; j++)
		{
			if (TblRookPath[(i << 6) + j] != (qword)-1)
			{
				for(int k = 0; k < 64; k++)
				{
					if ((TblRookPath[(i << 6) + j] & ((qword)1 << k)) == 0)
						continue;
					for (int pos = 0; pos < 64; pos++)
					{
						if (TblHorseChkIdx[(pos << 6) + k] == 0)
							continue;
						if (TblHorseBlock1[(((i << 6) + j) << 6) + pos] == 0x40)
							TblHorseBlock1[(((i << 6) + j) << 6) + pos] = k;
						else
							TblHorseBlock2[(((i << 6) + j) << 6) + pos] = k;
					}
				}
			}
			if (TblBishopPath[(i << 6) + j] != (qword)-1)
			{
				for(int k = 0; k < 64; k++)
				{
					if ((TblBishopPath[(i << 6) + j] & ((qword)1 << k)) == 0)
						continue;
					for (int pos = 0; pos < 64; pos++)
					{
						if (TblHorseChkIdx[(pos << 6) + k] == 0)
							continue;
						if (TblHorseBlock1[(((i << 6) + j) << 6) + pos] == 0x40)
							TblHorseBlock1[(((i << 6) + j) << 6) + pos] = k;
						else
							TblHorseBlock2[(((i << 6) + j) << 6) + pos] = k;
					}
				}
			}
		}
	}
}

static void CreateTblBishopBlock()
{
	TblBishopBlock1 = new byte[262144];
	memset(TblBishopBlock1, 0x40, sizeof(byte[262144]));
	TblBishopBlock2 = new byte[262144];
	memset(TblBishopBlock2, 0x40, sizeof(byte[262144]));

	for (int i = 0; i < 64; i++)
	{
		for (int j = 0; j < 64; j++)
		{
			if (TblRookPath[(i << 6) + j] != (qword)-1)
			{
				for(int k = 0; k < 64; k++)
				{
					if ((TblRookPath[(i << 6) + j] & ((qword)1 << k)) == 0)
						continue;
					for (int pos = 0; pos < 64; pos++)
					{
						if (TblBishopPath[(k << 6) + pos] == (qword)-1)
							continue;
						if (TblBishopBlock1[(((i << 6) + j) << 6) + pos] == 0x40)
							TblBishopBlock1[(((i << 6) + j) << 6) + pos] = k;
						else
							TblBishopBlock2[(((i << 6) + j) << 6) + pos] = k;
					}
				}
			}
			if (TblBishopPath[(i << 6) + j] != (qword)-1)
			{
				for(int k = 0; k < 64; k++)
				{
					if ((TblBishopPath[(i << 6) + j] & ((qword)1 << k)) == 0)
						continue;
					for (int pos = 0; pos < 64; pos++)
					{
						if (TblBishopPath[(k << 6) + pos] == (qword)-1)
							continue;
						TblBishopBlock1[(((i << 6) + j) << 6) + pos] = k;
					}
				}
			}
		}
	}
}

static void CreateTblBlkPawnBlock()
{
	TblBlkPawnBlock = new byte[262144];
	memset(TblBlkPawnBlock, 0x40, sizeof(byte[262144]));

	for (int i = 0; i < 64; i++)
	{
		for (int j = 0; j < 64; j++)
		{
			if (TblRookPath[(i << 6) + j] != (qword)-1)
			{
				for(int k = 0; k < 64; k++)
				{
					if ((TblRookPath[(i << 6) + j] & ((qword)1 << k)) == 0)
						continue;
					for (int pos = 0; pos < 64; pos++)
					{
						if (pos - 8 != k)
							continue;
						TblBlkPawnBlock[(((i << 6) + j) << 6) + pos] = k;
					}
				}
			}
			if (TblBishopPath[(i << 6) + j] != (qword)-1)
			{
				for(int k = 0; k < 64; k++)
				{
					if ((TblBishopPath[(i << 6) + j] & ((qword)1 << k)) == 0)
						continue;
					for (int pos = 0; pos < 64; pos++)
					{
						if (pos - 8 != k)
							continue;
						TblBlkPawnBlock[(((i << 6) + j) << 6) + pos] = k;
					}
				}
			}
		}
	}
}

static void CreateTblBlkPawnBlock2()
{
	TblBlkPawnBlock2 = new byte[262144];
	memset(TblBlkPawnBlock2, 0x40, sizeof(byte[262144]));

	for (int i = 0; i < 64; i++)
	{
		for (int j = 0; j < 64; j++)
		{
			if (TblRookPath[(i << 6) + j] != (qword)-1)
			{
				for(int k = 0; k < 64; k++)
				{
					if ((TblRookPath[(i << 6) + j] & ((qword)1 << k)) == 0)
						continue;
					for (int pos = 0; pos < 64; pos++)
					{
						if (pos < 48 || pos - 16 != k)
							continue;
						TblBlkPawnBlock2[(((i << 6) + j) << 6) + pos] = k;
					}
				}
			}
			if (TblBishopPath[(i << 6) + j] != (qword)-1)
			{
				for(int k = 0; k < 64; k++)
				{
					if ((TblBishopPath[(i << 6) + j] & ((qword)1 << k)) == 0)
						continue;
					for (int pos = 0; pos < 64; pos++)
					{
						if (pos < 48 || pos - 16 != k)
							continue;
						TblBlkPawnBlock2[(((i << 6) + j) << 6) + pos] = k;
					}
				}
			}
		}
	}
}

static void CreateTblHorseMove()
{
	TblHorseMove = new byte[64];
	memset(TblHorseMove, 0, sizeof(byte[64]));

	int T[8] = { +17, +10, -6, -15, -17, -10, +6, +15, };
	for (int i = 0; i < 64; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (i + T[j] >= 0 && i + T[j] < 64)
			{
				int dest = i + T[j];
				if (abs((dest & 7) - (i & 7)) <= 2 && abs((dest >> 3) - (i >> 3)) <= 2)
					TblHorseMove[i] |= 1 << j;
			}
		}
	}
}

static void CreateTblKingMove()
{
	TblKingMove = new byte[64];
	memset(TblKingMove, 0, sizeof(byte[64]));

	int T[8] = { +9, +1, -7, -8, -9, -1, +7, +8, };
	for (int i = 0; i < 64; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (i + T[j] >= 0 && i + T[j] < 64)
			{
				int dest = i + T[j];
				if (abs((dest & 7) - (i & 7)) <= 1 && abs((dest >> 3) - (i >> 3)) <= 1)
					TblKingMove[i] |= 1 << j;
			}
		}
	}
}

static void CreateTblKingAway()
{
	TblKingAway = new byte[4096];
	memset(TblKingAway, 0, sizeof(byte[4096]));

	for (int i = 0; i < 64; i++)
	{
		int x1 = i & 7;
		int y1 = i >> 3;
		for (int j = 0; j < 64; j++)
		{
			int x2 = j & 7;
			int y2 = j >> 3;
			TblKingAway[(i << 6) + j] = max(abs(x1 - x2), abs(y1 - y2));
		}
	}
}

static void CreateTblMaxKingAway()
{
	TblMaxKingAway = new byte[64];
	memset(TblMaxKingAway, 0, sizeof(byte[64]));

	for (int i = 0; i < 64; i++)
	{
		TblMaxKingAway[i] = max(TblMaxKingAway[i], TblKingAway[(i << 6) + 0]);
		TblMaxKingAway[i] = max(TblMaxKingAway[i], TblKingAway[(i << 6) + 7]);
		TblMaxKingAway[i] = max(TblMaxKingAway[i], TblKingAway[(i << 6) + 54]);
		TblMaxKingAway[i] = max(TblMaxKingAway[i], TblKingAway[(i << 6) + 63]);
	}
}

void CreateAllTable()
{
	::CreateTblBitIndex();
	::CreateTblPieceBitmap();
	::CreateTblCharToPiece();
	::CreateTblPieceToChar();
	::CreateTblQueenPath();
	::CreateTblRookPath();
	::CreateTblBishopPath();
	::CreateTblDirection();
	::CreateTblRookCrossGrid();
	::CreateTblBishopCrossGrid();
	::CreateTblRookCrossPath();
	::CreateTblBishopCrossPath();
	::CreateTblQueenCrossGridBtmp();
	::CreateTblHorseChkIdx();
	::CreateTblKingChkIdx();
	::CreateTblEludeChkIdx();
	::CreateTblHorseCheck();
	::CreateTblRookBlock();
	::CreateTblHorseBlock();
	::CreateTblBishopBlock();
	::CreateTblBlkPawnBlock();
	::CreateTblBlkPawnBlock2();
	::CreateTblHorseMove();
	::CreateTblKingMove();
	::CreateTblKingAway();
	::CreateTblMaxKingAway();
}

void DeleteAllTable()
{
	delete TblBitIndex;
	delete TblPieceBitmap;
	delete TblCharToPiece;
	delete TblPieceToChar;
	delete TblQueenPath;
	delete TblRookPath;
	delete TblBishopPath;
	delete TblDirection;
	for (int i = 0; i < 2; i++)
	{
		delete TblRookCrossGrid[i];
		delete TblBishopCrossGrid[i];
		delete TblRookCrossPath[i];
		delete TblBishopCrossPath[i];
	}
	delete TblQueenCrossGridBtmp;
	delete TblHorseChkGrid1;
	delete TblHorseChkGrid2;
	delete TblHorseChkIdx;
	delete TblKingChkIdx;
	delete TblEludeChkIdx;
	delete TblRookBlock1;
	delete TblRookBlock2;
	delete TblHorseBlock1;
	delete TblHorseBlock2;
	delete TblBishopBlock1;
	delete TblBishopBlock2;
	delete TblBlkPawnBlock;
	delete TblBlkPawnBlock2;
	delete TblHorseMove;
	delete TblKingMove;
	delete TblKingAway;
	delete TblMaxKingAway;
}
