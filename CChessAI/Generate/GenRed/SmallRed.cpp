#include "StdAfx.h"
#include "Generate\Generate.h"
#include "MoveList\MoveList.h"

void BishopLeaveXly(int pos, int chk)
{
	if (board[19] == 0)
		::AddKillRed(pos, 2, chk);
	if (board[21] == 0)
		::AddKillRed(pos, 6, chk);
	if (board[51] == 0)
		::AddKillRed(pos, 66, chk);
	if (board[53] == 0)
		::AddKillRed(pos, 70, chk);
}

void GuardLeaveXly(int pos, int chk)
{
	if (pos == 20)
	{
		::AddKillRed(20, 3, chk);
		::AddKillRed(20, 5, chk);
		::AddKillRed(20, 35, chk);
		::AddKillRed(20, 37, chk);
	}
	else
	{
		::AddKillRed(pos, 20, chk);
	}
}

void GenPawnRed()
{
	if (board[kpos - 2] == R_PAWN)
		::AddKillRed(kpos - 2, kpos - 1, kpos - 1);
	if (board[kpos + 2] == R_PAWN)
		::AddKillRed(kpos + 2, kpos + 1, kpos + 1);
	if (board[kpos - 32] == R_PAWN)
		::AddKillRed(kpos - 32, kpos - 16, kpos - 16);
	if (board[kpos - 17] == R_PAWN)
	{
		if (board[kpos - 18] != R_HORSE && board[kpos - 33] != R_HORSE) //(--兵闪马将)
		{
			::AddKillRed(kpos - 17, kpos - 16, kpos - 16);
			::AddKillRed(kpos - 17, kpos - 1, kpos - 1);
		}
	}
	if (board[kpos - 15] == R_PAWN)
	{
		if (board[kpos - 14] != R_HORSE && board[kpos - 31] != R_HORSE) //(--兵闪马将)
		{
			::AddKillRed(kpos - 15, kpos - 16, kpos - 16);
			::AddKillRed(kpos - 15, kpos + 1, kpos + 1);
		}
	}
}
