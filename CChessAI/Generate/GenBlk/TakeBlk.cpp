#include "StdAfx.h"
#include "Generate\Generate.h"
#include "MoveList\MoveList.h"

#define AddNPinKill(src, dest) \
{ \
	if (!::IsPinnedByRed(src)) \
		::AddKillBlk(src, dest); \
}

void GenTakeChecker(int check)
{
	int chx = check & 0xF;
	int chy = check >> 4;
	int px, py;
	TPiece *Piece = PieceBlkB;
	while (Piece)
	{
		int pos = Piece->pos;
		switch (Piece->kind)
		{
		case B_ROOK:
			GetPxPy(pos, px, py);
			if (px == chx && (vert[px] & TblBitrim[(py << 4) + chy]) == 0 || py == chy && (horz[py] & TblBitrim[(px << 4) + chx]) == 0)
				AddNPinKill(pos, check);
			break;
		case B_HORSE:
			if (TblHorseLeg[pos - check + 160] != 0 && board[pos + TblHorseLeg[pos - check + 160]] == 0)
				AddNPinKill(pos, check);
			break;
		case B_CANNON:
			GetPxPy(pos, px, py);
			if (px == chx && TblNumberOne[vert[px] & TblBitrim[(py << 4) + chy]] == 1 || py == chy && TblNumberOne[horz[py] & TblBitrim[(px << 4) + chx]] == 1)
				AddNPinKill(pos, check);
			break;
		}
		Piece =	Piece->next;
	}

	Piece = PieceBlkS;
	while (Piece)
	{
		int pos = Piece->pos;
		switch (Piece->kind)
		{
		case B_BISHOP:
			GetPxPy(pos, px, py);
			if (TblBishopSphere[check] && (px - chx == 2 || px - chx == -2) && board[(pos + check) >> 1] == 0)
				AddNPinKill(pos, check);
			break;
		case B_GUARD:
			if (TblGuardSphere[check] && (check == 132 || pos == 132))
				AddNPinKill(pos, check);
			break;
		case B_PAWN:
			if ((pos + 1 == check || pos - 1 == check) && pos < 80 || pos - 16 == check)
				AddNPinKill(pos, check);
			break;
		}
		Piece =	Piece->next;
	}
}
