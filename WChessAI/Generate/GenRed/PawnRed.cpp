#include "StdAfx.h"
#include "Generate\Generate.h"
#include "MoveList\MoveList.h"

static void RedBian(int pos, int dest, int check);

void GenPawnRed(int pos)
{
	if (IsPinnedByRed(pos))
		return;

	if (pos < 48)
	{
		//ֱ��
		if ((pos & 7) != 0 && pos + 15 == kpos || (pos & 7) != 7 && pos + 17 == kpos)
		{
			if (board[pos + 8] == 0)
				::AddMoveRed(pos, pos + 8, pos + 8);
		}
		//б��
		if ((TblKingMove[pos] & 0x40) && board[pos + 7] >= B_ROOK && (pos + 16 == kpos || pos + 14 == kpos))
			::AddKillRed(pos, pos + 7, pos + 7);
		if ((TblKingMove[pos] & 0x01) && board[pos + 9] >= B_ROOK && (pos + 16 == kpos || pos + 18 == kpos))
			::AddKillRed(pos, pos + 9, pos + 9);
		if (pos < 16 && board[pos + 8] == 0 && board[pos + 16] == 0)
		{
			if ((pos & 7) != 0 && pos + 23 == kpos || (pos & 7) != 7 && pos + 25 == kpos)
				::AddMoveRed(pos, pos + 16, pos + 16);
		}
	}
	else
	{
		//ֱ������
		if ((TblKingMove[pos] & 0x80) && board[pos + 8] == 0)
			::RedBian(pos, pos + 8, pos + 8);
		//б������
		if ((TblKingMove[pos] & 0x40) && board[pos + 7] >= B_ROOK)
			::RedBian(pos, pos + 7, pos + 7);
		if ((TblKingMove[pos] & 0x01) && board[pos + 9] >= B_ROOK)
			::RedBian(pos, pos + 9, pos + 9);
	}
}

void RedBian(int pos, int dest, int check)
{
	qword _Z = Z ^ ((qword)1 << pos) | ((qword)1 << dest);

	//����Ϊ��
	if (check != dest || (_Z & TblQueenPath[(kpos << 6) + dest]) == 0)
		::AddKillRed(pos, dest, check, R_QUEEN);
	//����Ϊ��
	//if ((_Z & TblRookPath[(kpos << 6) + dest]) == 0)
	//	::AddKillRed(pos, dest, dest, R_ROOK);
	//����Ϊ��
	//if ((_Z & TblBishopPath[(kpos << 6) + dest]) == 0)
	//	::AddKillRed(pos, dest, dest, R_BISHOP);
	//����Ϊ��
	if (check != dest || TblHorseChkIdx[(kpos << 6) + dest] == 1)
		::AddKillRed(pos, dest, check, R_HORSE);
}
