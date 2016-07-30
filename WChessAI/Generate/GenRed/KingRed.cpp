#include "StdAfx.h"
#include "Generate\Generate.h"
#include "MoveList\MoveList.h"

void KingLeaveXly(int pos, int chk, int nIndex)
{
	static int T[5 * 8] = {
		 0, 1, 2, 3, 4, 5, 6, 7,
		 0, 2, 3, 4, 6, 7, 0, 0,
		 0, 1, 2, 4, 5, 6, 0, 0,
		 1, 2, 3, 5, 6, 7, 0, 0,
		 1, 3, 4, 5, 7, 0, 0, 0,
	};
	static int TOffset[8] = { +9, +1, -7, -8, -9, -1, +7, +8, };

	if (nIndex > 0)
	{
		for (int i = 0; i < 6; i++)
		{
			if (TblKingMove[pos] & (1 << T[(nIndex << 3) + i]))
				::AddKillRed(pos, pos + TOffset[T[(nIndex << 3) + i]], chk);
		}
	}
	else
	{
		for (int i = 0; i < 8; i++)
		{
			if (TblKingMove[pos] & (1 << T[(nIndex << 3) + i]))
				::AddKillRed(pos, pos + TOffset[T[(nIndex << 3) + i]], chk);
		}
	}
}
