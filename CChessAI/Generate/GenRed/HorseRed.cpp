#include "StdAfx.h"
#include "Generate\Generate.h"
#include "MoveList\MoveList.h"

static void			 LeaveXForHorse(int pos, int chk);

static int TblOrg1[5] = { 0, -16, -16, 16, 16 }; //马腿索引->马位置1
static int TblOrg2[5] = { 0,  -1,   1,  1, -1 }; //马腿索引->马位置2

void GenHorseRed(int pos)
{
	static int TOpst[8] = { 15, 17, -17, 15, -15, -17, 17, -15 }; //间将索引->马位置1
	static int THLeg[8] = { 16, 16,  -1, -1, -16, -16,  1,   1 }; //间将索引->马位置2

	int idx = TblHrsChkIdx[pos - kpos + 160];
	if (idx == 0)
		return;
	if (idx < 20) //马双直将
	{
		if (IsPinnedByRed(pos))
			return;
		if (board[pos + TblHrsChk11[idx]] == 0 && board[pos + TblHrsChk13[idx]] == 0)
			::AddKillRed(pos, pos + TblHrsChk12[idx], pos + TblHrsChk12[idx]);
		if (board[pos + TblHrsChk21[idx]] == 0 && board[pos + TblHrsChk23[idx]] == 0)
			::AddKillRed(pos, pos + TblHrsChk22[idx], pos + TblHrsChk22[idx]);
	}
	else if (idx < 26) //马单直将
	{
		if (board[pos + TblHrsChk11[idx]] == 0 && board[pos + TblHrsChk13[idx]] == 0)
			::AddKillRed(pos, pos + TblHrsChk12[idx], pos + TblHrsChk12[idx]);
	}
	else //各闪马将
	{
		idx -= 26; //切换到间将索引
		if (TOpst[idx] > 0 || board[pos + TOpst[idx]] != R_HORSE) //(--各闪大马将)
			::LeaveXForHorse(pos + THLeg[idx], pos);
	}
}

void HorseLeaveXly(int pos, int chk)
{
	//马必然不在边界，马在次边界时边界必然有子
	if (board[pos + 16] == 0)
	{
		::AddKillRed(pos, pos + 31, chk);
		::AddKillRed(pos, pos + 33, chk);
	}
	if (board[pos - 16] == 0)
	{
		::AddKillRed(pos, pos - 33, chk);
		::AddKillRed(pos, pos - 31, chk);
	}
	if (board[pos + 1] == 0)
	{
		::AddKillRed(pos, pos + 18, chk);
		::AddKillRed(pos, pos - 14, chk);
	}
	if (board[pos - 1] == 0)
	{
		::AddKillRed(pos, pos + 14, chk);
		::AddKillRed(pos, pos - 18, chk);
	}
}

void LeaveXForHorse(int pos, int chk)
{
	switch (board[pos])
	{
	case R_ROOK:
		::RookLeaveHly(pos, chk);
		::RookLeaveVly(pos, chk);
		break;
	case R_HORSE:
		::HorseLeaveXly(pos, chk);
		break;
	case R_CANNON:
		::CannonLeaveHly(pos, chk);
		::CannonLeaveVly(pos, chk);
		break;
	case R_PAWN:
		::AddKillRed(pos, pos + 1, chk);
		::AddKillRed(pos, pos - 1, chk);
		::AddKillRed(pos, pos + 16, chk);
		break;
	}
}
