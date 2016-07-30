#include "StdAfx.h"
#include "Generate\Generate.h"
#include "MoveList\MoveList.h"

void GenEludeLimited(int check, int away)
{
	static int T[13 * 8] = {
		 0, 1, 2, 3, 4, 5, 6, 7,
		 0, 2, 3, 4, 6, 7, 0, 0,
		 0, 1, 2, 4, 5, 6, 0, 0,
		 1, 2, 3, 5, 6, 7, 0, 0,
		 1, 3, 4, 5, 7, 0, 0, 0,
		 0, 3, 5, 0, 0, 0, 0, 0,
		 1, 4, 6, 0, 0, 0, 0, 0,
		 2, 5, 7, 0, 0, 0, 0, 0,
		 3, 6, 0, 0, 0, 0, 0, 0,
		 4, 7, 1, 0, 0, 0, 0, 0,
		 5, 0, 2, 0, 0, 0, 0, 0,
		 6, 1, 3, 0, 0, 0, 0, 0,
		 7, 2, 4, 0, 0, 0, 0, 0,
	};
	static int TOffset[8] = { +9, +1, -7, -8, -9, -1, +7, +8, };


	//_kpos = check;
#if 0
	int nIndex = TblEludeChkIdx[(check << 6) + kpos];
	if (nIndex > 0)
	{
		for (byte i = 0; i < away; i++)
		{
			for (int j = 0; j < 6; j++)
			{
				int dir = T[(nIndex << 3) + j];
				if (TblKingMove[kpos] & (1 << dir))
				{
					if (TblKingAway[(_kpos << 6) + (kpos + TOffset[dir])] == i)
						::AddKillBlk(kpos, kpos + TOffset[dir]);
				}
			}
		}
	}
	else
	{
		for (byte i = 0; i < away; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				int dir = T[(nIndex << 3) + j];
				if (TblKingMove[kpos] & (1 << dir))
				{
					if (TblKingAway[(_kpos << 6) + (kpos + TOffset[dir])] == i)
						::AddKillBlk(kpos, kpos + TOffset[dir]);
				}
			}
		}
	}
#endif
	int nCount = 0;
	int nIndex = TblEludeChkIdx[(kpos << 6) + check];
	if (nIndex >= 5)
	{
		if (board[check] == R_QUEEN)
			nCount = 3;
		else
		{
			nIndex = 0;
			nCount = 8;
		}
	}
	else if (nIndex > 0)
		nCount = 6;
	else
		nCount = 8;
		
	for (byte i = 7; i >= 4; i--)
	{
		for (int j = 0; j < nCount; j++)
		{
			int dir = T[(nIndex << 3) + j];
			if (TblKingMove[kpos] & (1 << dir))
			{
				if (TblMaxKingAway[kpos + TOffset[dir]] == i)
				{
					if (TblKingAway[(_kpos << 6) + (kpos + TOffset[dir])] < away)
						::AddKillBlk(kpos, kpos + TOffset[dir]);
				}
			}
		}
	}
}
