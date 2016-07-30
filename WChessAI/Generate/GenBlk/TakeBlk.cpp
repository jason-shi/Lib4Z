#include "StdAfx.h"
#include "Generate\Generate.h"
#include "MoveList\MoveList.h"

static void BlkBian(int pos, int dest);

void GenTakeChecker(int check)
{
	TPiece *Piece = PieceBlkB;
	while (Piece)
	{
		int pos = Piece->pos;
		switch (Piece->kind)
		{
		case B_ROOK:
			if ((Z & TblRookPath[(check << 6) + pos]) == 0)
				::AddKillBlk(pos, check);
			break;
		case B_BISHOP:
			if ((Z & TblBishopPath[(check << 6) + pos]) == 0)
				::AddKillBlk(pos, check);
			break;
		case B_QUEEN:
			if ((Z & TblQueenPath[(check << 6) + pos]) == 0)
				::AddKillBlk(pos, check);
			break;
		case B_HORSE:
			if (TblHorseChkIdx[(check << 6) + pos] == 1)
				::AddKillBlk(pos, check);
			break;
		case B_PAWN:
			if (pos >= 16)
			{
				//б��
				if ((TblKingMove[pos] & 0x04) && pos - 7 == check)
					::AddKillBlk(pos, pos - 7);
				if ((TblKingMove[pos] & 0x10) && pos - 9 == check)
					::AddKillBlk(pos, pos - 9);
			}
			else
			{
				//б������
				if ((TblKingMove[pos] & 0x40) && pos - 7 == check)
					::BlkBian(pos, pos - 7);
				if ((TblKingMove[pos] & 0x10) && pos - 9 == check)
					::BlkBian(pos, pos - 9);
			}
			break;
		}
		Piece =	Piece->next;
	}
}

void BlkBian(int pos, int dest)
{
	//����Ϊ��
	::AddKillBlk(pos, dest, B_QUEEN);
	//����Ϊ��
	//::AddKillBlk(pos, dest, B_ROOK);
	//����Ϊ��
	//::AddKillBlk(pos, dest, B_BISHOP);
	//����Ϊ��
	::AddKillBlk(pos, dest, B_HORSE);
}
