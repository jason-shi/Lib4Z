#include "StdAfx.h"
#include "Generate\Generate.h"
#include "MoveList\MoveList.h"

extern void RedBian(int pos, int dest, int check);

static void LeaveHForRook(int pos, int chk);
static void LeaveVForRook(int pos, int chk);

void GenRookRed(int pos)
{
	int nIndex = (pos << 6) + kpos;
	if (TblRookPath[nIndex] != (qword)-1)
	{
		qword _bitmap = Z & TblRookPath[nIndex];
		if ((_bitmap & _bitmap - 1) == 0)
		{
			int dest = TblBitIndex[_bitmap % 67];
			if (board[dest] >= B_ROOK)
				::AddKillRed(pos, dest, dest);
			else
			{
				switch (TblDirection[nIndex])
				{
				case 1:
					::LeaveHForRook(dest, pos);
					break;
				case 2:
					::LeaveVForRook(dest, pos);
					break;
				}
			}
		}
	}
	else //in different line
	{
		if ((Z & TblRookCrossPath[0][nIndex]) == 0 && !IsPinnedByRed(pos))
		{
			int dest = TblRookCrossGrid[0][nIndex];
			::AddKillRed(pos, dest, dest);
		}
		if ((Z & TblRookCrossPath[1][nIndex]) == 0 && !IsPinnedByRed(pos))
		{
			int dest = TblRookCrossGrid[1][nIndex];
			::AddKillRed(pos, dest, dest);
		}
	}
}

void RookLeaveHly(int pos, int chk)
{
	//向左移动和向左吃子
	int i = 1;
	while (((pos - i) & 7) != 7 && board[pos - i] == 0)
	{
		::AddMoveRed(pos, pos - i, chk);
		i += 1;
	}
	if (((pos - i) & 7) != 7 && board[pos - i] >= B_ROOK)
		::AddKillRed(pos, pos - i, chk);
	//向右移动和向右吃子
	i = 1;
	while (((pos + i) & 7) != 0 && board[pos + i] == 0)
	{
		::AddMoveRed(pos, pos + i, chk);
		i += 1;
	}
	if (((pos + i) & 7) != 0 && board[pos + i] >= B_ROOK)
		::AddKillRed(pos, pos + i, chk);
}

void RookLeaveVly(int pos, int chk)
{
	//向下移动和向下吃子
	int i = 8;
	while (pos - i >= 0 && board[pos - i] == 0)
	{
		::AddMoveRed(pos, pos - i, chk);
		i += 8;
	}
	if (pos - i >= 0 && board[pos - i] >= B_ROOK)
		::AddKillRed(pos, pos - i, chk);
	//向上移动和向上吃子
	i = 8;
	while (pos + i < 64 && board[pos + i] == 0)
	{
		::AddMoveRed(pos, pos + i, chk);
		i += 8;
	}
	if (pos + i < 64 && board[pos + i] >= B_ROOK)
		::AddKillRed(pos, pos + i, chk);
}

void LeaveHForRook(int pos, int chk)
{
	switch (board[pos])
	{
	case R_HORSE:
		::HorseLeaveXly(pos, chk);
		break;
	case R_BISHOP:
		::BishopLeaveSly(pos, chk);
		::BishopLeaveBly(pos, chk);
		break;
	case R_PAWN:
		if (pos < 48)
		{
			if ((TblKingMove[pos] & 0x40) && board[pos + 7] >= B_ROOK)
				::AddKillRed(pos, pos + 7, chk);
			if ((TblKingMove[pos] & 0x01) && board[pos + 9] >= B_ROOK)
				::AddKillRed(pos, pos + 9, chk);
			if ((TblKingMove[pos] & 0x80) && board[pos + 8] == 0)
				::AddMoveRed(pos, pos + 8, chk);
			if (pos < 16 && board[pos + 8] == 0 && board[pos + 16] == 0)
				::AddMoveRed(pos, pos + 16, chk);
		}
		else
		{
			//直走升变
			if ((TblKingMove[pos] & 0x80) && board[pos + 8] == 0)
				::RedBian(pos, pos + 8, chk);
			//斜吃升变
			if ((TblKingMove[pos] & 0x40) && board[pos + 7] >= B_ROOK)
				::RedBian(pos, pos + 7, chk);
			if ((TblKingMove[pos] & 0x01) && board[pos + 9] >= B_ROOK)
				::RedBian(pos, pos + 9, chk);
		}
		break;
	case R_KING:
		::KingLeaveXly(pos, chk, 1);
		break;
	}
}

void LeaveVForRook(int pos, int chk)
{
	switch (board[pos])
	{
	case R_HORSE:
		::HorseLeaveXly(pos, chk);
		break;
	case R_BISHOP:
		::BishopLeaveSly(pos, chk);
		::BishopLeaveBly(pos, chk);
		break;
	case R_PAWN:
		if (pos < 48)
		{
			if ((TblKingMove[pos] & 0x40) && board[pos + 7] >= B_ROOK)
				::AddKillRed(pos, pos + 7, chk);
			if ((TblKingMove[pos] & 0x01) && board[pos + 9] >= B_ROOK)
				::AddKillRed(pos, pos + 9, chk);
		}
		else
		{
			//斜吃升变
			if ((TblKingMove[pos] & 0x40) && board[pos + 7] >= B_ROOK)
				::RedBian(pos, pos + 7, chk);
			if ((TblKingMove[pos] & 0x01) && board[pos + 9] >= B_ROOK)
				::RedBian(pos, pos + 9, chk);
		}
		break;
	case R_KING:
		::KingLeaveXly(pos, chk, 2);
		break;
	}
}
