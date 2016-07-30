#include "StdAfx.h"
#include "Generate\Generate.h"
#include "MoveList\MoveList.h"

static void		 RookEscapeHly(int pos);
static void		 RookEscapeVly(int pos);
static void		 CannonEscapeHly(int pos);
static void		 CannonEscapeVly(int pos);
static void		 HorseEscapeXly(int pos);
static void		 BishopEscapeXly(int pos);
static void		 GuardEscapeXly(int pos);

void GenShellEscapeH(int pos)
{
	switch (board[pos])
	{
	case B_ROOK:
		::RookEscapeVly(pos);
		break;
	case B_HORSE:
		::HorseEscapeXly(pos);
		break;
	case B_BISHOP:
		::BishopEscapeXly(pos);
		break;
	case B_GUARD:
		::GuardEscapeXly(pos);
		break;
	case B_CANNON:
		if (pos == kl1)
		{
			if (kr1 != CLGRID)
				::AddKillBlk(pos, kr1);
			if (kl3 != CLGRID)
				::AddKillBlk(pos, kl3);
		}
		else
		{
			if (kl1 != CLGRID)
				::AddKillBlk(pos, kl1);
			if (kr3 != CLGRID)
				::AddKillBlk(pos, kr3);
		}
		::CannonEscapeVly(pos);
		break;
	}
}

void GenShellEscapeV(int pos)
{
	switch (board[pos])
	{
	case B_ROOK:
		::RookEscapeHly(pos);
		break;
	case B_HORSE:
		::HorseEscapeXly(pos);
		break;
	case B_BISHOP:
		::BishopEscapeXly(pos);
		break;
	case B_GUARD:
		::GuardEscapeXly(pos);
		break;
	case B_CANNON:
		if (pos == kb1)
		{
			if (vert[kx] & TblHighMask[ky])
				::AddKillBlk(pos, kx + (TblLastBit[vert[kx] & TblHighMask[ky]] << 4));
			if (kb3 != CLGRID)
				::AddKillBlk(pos, kb3);
		}
		else
		{
			if (kb1 != CLGRID)
				::AddKillBlk(pos, kb1);
		}
		::CannonEscapeHly(pos);
		break;
	case B_PAWN:
		if (pos < 80)
		{
			::AddKillBlk(pos, pos + 1);
			::AddKillBlk(pos, pos - 1);
		}
		break;
	}
}

void RookEscapeHly(int pos)
{
	int px = pos & 0xF;
	int py = pos >> 4;
	//向左移动和向左吃子
	int mside = horz[py] & TblLowMask[px];
	switch (mside ? px - TblFirstBit[mside] : px + 1)
	{
	case 7:
		::AddMoveBlk(pos, pos - 6);
	case 6:
		::AddMoveBlk(pos, pos - 5);
	case 5:
		::AddMoveBlk(pos, pos - 4);
	case 4:
		::AddMoveBlk(pos, pos - 3);
	case 3:
		::AddMoveBlk(pos, pos - 2);
	case 2:
		::AddMoveBlk(pos, pos - 1);
	}
	if (mside != 0)
		::AddKillBlk(pos, TblFirstBit[mside] + (py << 4));
	//向右移动和向右吃子
	mside = horz[py] & TblHighMask[px];
	switch (TblLastBit[mside] - px - !mside)
	{
	case 7:
		::AddMoveBlk(pos, pos + 6);
	case 6:
		::AddMoveBlk(pos, pos + 5);
	case 5:
		::AddMoveBlk(pos, pos + 4);
	case 4:
		::AddMoveBlk(pos, pos + 3);
	case 3:
		::AddMoveBlk(pos, pos + 2);
	case 2:
		::AddMoveBlk(pos, pos + 1);
	}
	if (mside != 0)
		::AddKillBlk(pos, TblLastBit[mside] + (py << 4));
}

void RookEscapeVly(int pos)
{
	int px = pos & 0xF;
	int py = pos >> 4;
	//向下移动和向下吃子
	int mside = vert[px] & TblLowMask[py];
	switch (mside ? py - TblFirstBit[mside] : py + 1)
	{
	case 10:
		::AddMoveBlk(pos, pos - 144);
	case 9:
		::AddMoveBlk(pos, pos - 128);
	case 8:
		::AddMoveBlk(pos, pos - 112);
	case 7:
		::AddMoveBlk(pos, pos - 96);
	case 6:
		::AddMoveBlk(pos, pos - 80);
	case 5:
		::AddMoveBlk(pos, pos - 64);
	case 4:
		::AddMoveBlk(pos, pos - 48);
	case 3:
		::AddMoveBlk(pos, pos - 32);
	case 2:
		::AddMoveBlk(pos, pos - 16);
	}
	if (mside != 0)
		::AddKillBlk(pos, px + (TblFirstBit[mside] << 4));
	//向上移动和向上吃子
	mside = vert[px] & TblHighMask[py];
	switch (TblLastBit[mside] - py)
	{
	case 4:
		::AddMoveBlk(pos, pos + 48);
	case 3:
		::AddMoveBlk(pos, pos + 32);
	case 2:
		::AddMoveBlk(pos, pos + 16);
	}
	if (mside != 0)
		::AddKillBlk(pos, px + (TblLastBit[mside] << 4));
}

void CannonEscapeHly(int pos)
{
	int px = pos & 0xF;
	int py = pos >> 4;
	//向左移动和向左打吃
	int mside = horz[py] & TblLowMask[px];
	switch (mside ? px - TblFirstBit[mside] : px + 1)
	{
	case 7:
		::AddMoveBlk(pos, pos - 6);
	case 6:
		::AddMoveBlk(pos, pos - 5);
	case 5:
		::AddMoveBlk(pos, pos - 4);
	case 4:
		::AddMoveBlk(pos, pos - 3);
	case 3:
		::AddMoveBlk(pos, pos - 2);
	case 2:
		::AddMoveBlk(pos, pos - 1);
	}
	if (TblSecondBit[mside] != 10)
		::AddKillBlk(pos, TblSecondBit[mside] + (py << 4));
	//向右移动和向右打吃
	mside = horz[py] & TblHighMask[px];
	switch (TblLastBit[mside] - px - !mside)
	{
	case 7:
		::AddMoveBlk(pos, pos + 6);
	case 6:
		::AddMoveBlk(pos, pos + 5);
	case 5:
		::AddMoveBlk(pos, pos + 4);
	case 4:
		::AddMoveBlk(pos, pos + 3);
	case 3:
		::AddMoveBlk(pos, pos + 2);
	case 2:
		::AddMoveBlk(pos, pos + 1);
	}
	if (TblLagendBit[mside] != 10)
		::AddKillBlk(pos, TblLagendBit[mside] + (py << 4));
}

void CannonEscapeVly(int pos)
{
	int px = pos & 0xF;
	int py = pos >> 4;
	//向下移动和向下打吃
	int mside = vert[px] & TblLowMask[py];
	switch (mside ? py - TblFirstBit[mside] : py + 1)
	{
	case 10:
		::AddMoveBlk(pos, pos - 144);
	case 9:
		::AddMoveBlk(pos, pos - 128);
	case 8:
		::AddMoveBlk(pos, pos - 112);
	case 7:
		::AddMoveBlk(pos, pos - 96);
	case 6:
		::AddMoveBlk(pos, pos - 80);
	case 5:
		::AddMoveBlk(pos, pos - 64);
	case 4:
		::AddMoveBlk(pos, pos - 48);
	case 3:
		::AddMoveBlk(pos, pos - 32);
	case 2:
		::AddMoveBlk(pos, pos - 16);
	}
	if (TblSecondBit[mside] != 10)
		::AddKillBlk(pos, px + (TblSecondBit[mside] << 4));
	//向上移动和向上打吃
	mside = vert[px] & TblHighMask[py];
	switch (TblLastBit[mside] - py)
	{
	case 4:
		::AddMoveBlk(pos, pos + 48);
	case 3:
		::AddMoveBlk(pos, pos + 32);
	case 2:
		::AddMoveBlk(pos, pos + 16);
	}
	if (TblLagendBit[mside] != 10)
		::AddKillBlk(pos, px + (TblLagendBit[mside] << 4));
}

void HorseEscapeXly(int pos)
{
	if (board[pos + 16] == 0)
	{
		//上边界越界
		if (pos < 128)
		{
			::AddKillBlk(pos, pos + 31);
			::AddKillBlk(pos, pos + 33);
		}
	}
	if (board[pos - 16] == 0)
	{
		::AddKillBlk(pos, pos - 33);
		::AddKillBlk(pos, pos - 31);
	}
	if (board[pos + 1] == 0)
	{
		//上边界越界
		if (pos < 144)
			::AddKillBlk(pos, pos + 18);
		::AddKillBlk(pos, pos - 14);
	}
	if (board[pos - 1] == 0)
	{
		//上边界越界
		if (pos < 144)
			::AddKillBlk(pos, pos + 14);
		::AddKillBlk(pos, pos - 18);
	}
}

void BishopEscapeXly(int pos)
{
	switch (pos)
	{
	case 116:
		if (board[99]  == 0) ::AddKillBlk(pos, 82);
		if (board[101] == 0) ::AddKillBlk(pos, 86);
		if (board[131] == 0) ::AddKillBlk(pos, 146);
		if (board[133] == 0) ::AddKillBlk(pos, 150);
		break;
	case 146:
		if (board[129] == 0) ::AddKillBlk(pos, 112);
		if (board[131] == 0) ::AddKillBlk(pos, 116);
		break;
	case 150:
		if (board[135] == 0) ::AddKillBlk(pos, 120);
		if (board[133] == 0) ::AddKillBlk(pos, 116);
		break;
	}
}

void GuardEscapeXly(int pos)
{
	switch (pos)
	{
	case 132:
		::AddKillBlk(pos, 115);
		::AddKillBlk(pos, 117);
		::AddKillBlk(pos, 147);
		::AddKillBlk(pos, 149);
		break;
	default:
		::AddKillBlk(pos, 132);
		break;
	}
}
