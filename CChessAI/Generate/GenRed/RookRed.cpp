#include "StdAfx.h"
#include "Generate\Generate.h"
#include "MoveList\MoveList.h"

static void LeaveVForRook(int pos, int chk);
static void LeaveHForRook(int pos, int chk);

void GenRookRed(int pos)
{
	int px = pos & 0xF;
	int py = pos >> 4;
	if (px == kx)
	{
		if (pos == kb2)
			board[kb1] >= B_ROOK ? ::AddKillRed(pos, kb1, kb1) : ::LeaveVForRook(kb1, pos);
		else if (pos > kpos)
			board[pos - 16] >= B_ROOK ? ::AddKillRed(pos, pos - 16, pos - 16) : ::LeaveVForRook(pos - 16, pos);
	}
	else if (py == ky)
	{
		if (pos == kl2)
			board[kl1] >= B_ROOK ? ::AddKillRed(pos, kl1, kl1) : ::LeaveHForRook(kl1, pos);
		else if (pos == kr2)
			board[kr1] >= B_ROOK ? ::AddKillRed(pos, kr1, kr1) : ::LeaveHForRook(kr1, pos);
	}
	else //in different line
	{
		int htrim = TblBitrim[(px << 4) + kx];
		int vtrim = TblBitrim[(py << 4) + ky];
		if (htrim || vtrim || board[pos + px - kx] != R_HORSE && board[pos + ((py - ky) << 4)] != R_HORSE) //(--车闪马将)
		{
			if ((horz[ky] & htrim) == 0 && (vert[px] & vtrim) == 0) //move vertical
				::AddKillRed(pos, px + (ky << 4), px + (ky << 4));
			if ((vert[kx] & vtrim) == 0 && (horz[py] & htrim) == 0) //move horizontal
				::AddKillRed(pos, kx + (py << 4), kx + (py << 4));
		}
	}
}

void RookLeaveHly(int pos, int chk)
{
	int px = pos & 0xF;
	int py = pos >> 4;
	//向左移动和向左吃子
	int mside = horz[py] & TblLowMask[px];
	switch (mside ? px - TblFirstBit[mside] : px + 1)
	{
	case 7:
		AddMoveRed(pos, pos - 6, chk);
	case 6:
		AddMoveRed(pos, pos - 5, chk);
	case 5:
		AddMoveRed(pos, pos - 4, chk);
	case 4:
		AddMoveRed(pos, pos - 3, chk);
	case 3:
		AddMoveRed(pos, pos - 2, chk);
	case 2:
		AddMoveRed(pos, pos - 1, chk);
	}
	if (mside != 0)
		::AddKillRed(pos, TblFirstBit[mside] + (py << 4), chk);
	//向右移动和向右吃子
	mside = horz[py] & TblHighMask[px];
	switch (TblLastBit[mside] - px - !mside)
	{
	case 7:
		::AddMoveRed(pos, pos + 6, chk);
	case 6:
		::AddMoveRed(pos, pos + 5, chk);
	case 5:
		::AddMoveRed(pos, pos + 4, chk);
	case 4:
		::AddMoveRed(pos, pos + 3, chk);
	case 3:
		::AddMoveRed(pos, pos + 2, chk);
	case 2:
		::AddMoveRed(pos, pos + 1, chk);
	}
	if (mside != 0)
		::AddKillRed(pos, TblLastBit[mside] + (py << 4), chk);
}

void RookLeaveVly(int pos, int chk)
{
	int px = pos & 0xF;
	int py = pos >> 4;
	//向下移动和向下吃子
	int mside = vert[px] & TblLowMask[py];
	switch (mside ? py - TblFirstBit[mside] : py + 1)
	{
	case 10:
		::AddMoveRed(pos, pos - 144, chk);
	case 9:
		::AddMoveRed(pos, pos - 128, chk);
	case 8:
		::AddMoveRed(pos, pos - 112, chk);
	case 7:
		::AddMoveRed(pos, pos - 96, chk);
	case 6:
		::AddMoveRed(pos, pos - 80, chk);
	case 5:
		::AddMoveRed(pos, pos - 64, chk);
	case 4:
		::AddMoveRed(pos, pos - 48, chk);
	case 3:
		::AddMoveRed(pos, pos - 32, chk);
	case 2:
		::AddMoveRed(pos, pos - 16, chk);
	}
	if (mside != 0)
		::AddKillRed(pos, px + (TblFirstBit[mside] << 4), chk);
	//向上移动和向上吃子
	mside = vert[px] & TblHighMask[py];
	switch (TblLastBit[mside] - py)
	{
	case 4:
		::AddMoveRed(pos, pos + 48, chk);
	case 3:
		::AddMoveRed(pos, pos + 32, chk);
	case 2:
		::AddMoveRed(pos, pos + 16, chk);
	}
	if (mside != 0)
		::AddKillRed(pos, px + (TblLastBit[mside] << 4), chk);
}

void LeaveVForRook(int pos, int chk)
{
	switch (board[pos])
	{
	case R_HORSE:
		::HorseLeaveXly(pos, chk);
		break;
	case R_BISHOP:
		::BishopLeaveXly(pos, chk);
		break;
	case R_GUARD:
		::GuardLeaveXly(pos, chk);
		break;
	case R_CANNON:
		::CannonLeaveHly(pos, chk);
		break;
	case R_PAWN:
		if (pos >= 80)
		{
			::AddKillRed(pos, pos + 1, chk);
			::AddKillRed(pos, pos - 1, chk);
		}
		break;
	}
}

void LeaveHForRook(int pos, int chk)
{
	switch (board[pos])
	{
	case R_HORSE:
		::HorseLeaveXly(pos, chk);
		break;
	case R_CANNON:
		::CannonLeaveVly(pos, chk);
		break;
	case R_PAWN:
		::AddKillRed(pos, pos + 16, chk);
		break;
	}
}
