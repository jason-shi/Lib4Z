#include "StdAfx.h"
#include "Generate\Generate.h"
#include "MoveList\MoveList.h"

extern void RedBian(int pos, int dest, int check);

static void LeaveSForBishop(int pos, int chk);
static void LeaveBForBishop(int pos, int chk);

void GenBishopRed(int pos)
{
	int nIndex = (pos << 6) + kpos;
	if (TblBishopPath[nIndex] != (qword)-1)
	{
		qword _bitmap = Z & TblBishopPath[nIndex];
		if ((_bitmap & _bitmap - 1) == 0)
		{
			int dest = TblBitIndex[_bitmap % 67];
			if (board[dest] >= B_ROOK)
				::AddKillRed(pos, dest, dest);
			else
			{
				switch (TblDirection[nIndex])
				{
				case 3:
					::LeaveSForBishop(dest, pos);
					break;
				case 4:
					::LeaveBForBishop(dest, pos);
					break;
				}
			}
		}
	}
	else //in different line
	{
		if ((Z & TblBishopCrossPath[0][nIndex]) == 0 && !IsPinnedByRed(pos))
		{
			int dest = TblBishopCrossGrid[0][nIndex];
			::AddKillRed(pos, dest, dest);
		}
		if ((Z & TblBishopCrossPath[1][nIndex]) == 0 && !IsPinnedByRed(pos))
		{
			int dest = TblBishopCrossGrid[1][nIndex];
			::AddKillRed(pos, dest, dest);
		}
	}
}

void BishopLeaveSly(int pos, int chk)
{
	//向左下移动和向左下吃子
	int i = 9;
	while (pos - i >= 0 && board[pos - i] == 0)
	{
		::AddMoveRed(pos, pos - i, chk);
		i += 9;
	}
	if (pos - i >= 0 && board[pos - i] >= B_ROOK)
		::AddKillRed(pos, pos - i, chk);
	//向右上移动和向右上吃子
	i = 9;
	while (pos + i < 64 && board[pos + i] == 0)
	{
		::AddMoveRed(pos, pos + i, chk);
		i += 9;
	}
	if (pos + i < 64 && board[pos + i] >= B_ROOK)
		::AddKillRed(pos, pos + i, chk);
}

void BishopLeaveBly(int pos, int chk)
{
	//向右下移动和向右下吃子
	int i = 7;
	while (pos - i >= 0 && board[pos - i] == 0)
	{
		::AddMoveRed(pos, pos - i, chk);
		i += 7;
	}
	if (pos - i >= 0 && board[pos - i] >= B_ROOK)
		::AddKillRed(pos, pos - i, chk);
	//向左上移动和向左上吃子
	i = 7;
	while (pos + i < 64 && board[pos + i] == 0)
	{
		::AddMoveRed(pos, pos + i, chk);
		i += 7;
	}
	if (pos + i < 64 && board[pos + i] >= B_ROOK)
		::AddKillRed(pos, pos + i, chk);
}

void LeaveSForBishop(int pos, int chk)
{
	switch (board[pos])
	{
	case R_HORSE:
		::HorseLeaveXly(pos, chk);
		break;
	case R_ROOK:
		::RookLeaveHly(pos, chk);
		::RookLeaveVly(pos, chk);
		break;
	case R_PAWN:
		if (pos < 48)
		{
			if ((TblKingMove[pos] & 0x40) && board[pos + 7] >= B_ROOK)
				::AddKillRed(pos, pos + 7, chk);
			if ((TblKingMove[pos] & 0x80) && board[pos + 8] == 0)
				::AddMoveRed(pos, pos + 8, chk);
			if (pos < 16 && board[pos + 8] == 0 && board[pos + 16] == 0)
				::AddMoveRed(pos, pos + 16, chk);
		}
		else
		{
			//直走升变
			if ((TblKingMove[pos] & 0x80) && board[pos + 8] == 0)
				::RedBian(pos, pos + 8, pos + 8);
			//斜吃升变
			if ((TblKingMove[pos] & 0x40) && board[pos + 7] >= B_ROOK)
				::RedBian(pos, pos + 7, pos + 7);
		}
		break;
	case R_KING:
		::KingLeaveXly(pos, chk, 3);
		break;
	}
}

void LeaveBForBishop(int pos, int chk)
{
	switch (board[pos])
	{
	case R_HORSE:
		::HorseLeaveXly(pos, chk);
		break;
	case R_ROOK:
		::RookLeaveHly(pos, chk);
		::RookLeaveVly(pos, chk);
		break;
	case R_PAWN:
		if (pos < 48)
		{
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
				::RedBian(pos, pos + 8, pos + 8);
			//斜吃升变
			if ((TblKingMove[pos] & 0x01) && board[pos + 9] >= B_ROOK)
				::RedBian(pos, pos + 9, pos + 9);
		}
		break;
	case R_KING:
		::KingLeaveXly(pos, chk, 4);
		break;
	}
}
