#include "StdAfx.h"
#include "Generate\Generate.h"
#include "MoveList\MoveList.h"

void GenHorseRed(int pos)
{
	int nIndex = (pos << 6) + kpos;
	if (TblHorseChkGrid1[nIndex] != 0x40 && !IsPinnedByRed(pos))
	{
		int dest = TblHorseChkGrid1[nIndex];
		::AddKillRed(pos, dest, dest);
	}
	if (TblHorseChkGrid2[nIndex] != 0x40 && !IsPinnedByRed(pos))
	{
		int dest = TblHorseChkGrid2[nIndex];
		::AddKillRed(pos, dest, dest);
	}
}

void HorseLeaveXly(int pos, int chk)
{
	if (TblHorseMove[pos] & 0x01)
		::AddKillRed(pos, pos + 17, chk);
	if (TblHorseMove[pos] & 0x02)
		::AddKillRed(pos, pos + 10, chk);
	if (TblHorseMove[pos] & 0x04)
		::AddKillRed(pos, pos - 6, chk);
	if (TblHorseMove[pos] & 0x08)
		::AddKillRed(pos, pos - 15, chk);
	if (TblHorseMove[pos] & 0x10)
		::AddKillRed(pos, pos - 17, chk);
	if (TblHorseMove[pos] & 0x20)
		::AddKillRed(pos, pos - 10, chk);
	if (TblHorseMove[pos] & 0x40)
		::AddKillRed(pos, pos + 6, chk);
	if (TblHorseMove[pos] & 0x80)
		::AddKillRed(pos, pos + 15, chk);
}
