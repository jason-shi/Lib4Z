#include "StdAfx.h"
#include "Generate\Generate.h"
#include "MoveList\MoveList.h"

static void BlockByRookBlk(int pos, int check)
{
	byte block1 = TblRookBlock1[(((check << 6) + kpos) << 6) + pos];
	if (block1 != 0x40)
	{
		if ((Z & TblRookPath[(block1 << 6) + pos]) == 0)
			::AddMoveBlk(pos, block1);
	}
	byte block2 = TblRookBlock2[(((check << 6) + kpos) << 6) + pos];
	if (block2 != 0x40)
	{
		if ((Z & TblRookPath[(block2 << 6) + pos]) == 0)
			::AddMoveBlk(pos, block2);
	}
}

static void BlockByHorseBlk(int pos, int check)
{
	byte block1 = TblHorseBlock1[(((check << 6) + kpos) << 6) + pos];
	if (block1 != 0x40)
		::AddMoveBlk(pos, block1);

	byte block2 = TblHorseBlock2[(((check << 6) + kpos) << 6) + pos];
	if (block2 != 0x40)
		::AddMoveBlk(pos, block2);
}

static void BlockByBishopBlk(int pos, int check)
{
	byte block1 = TblBishopBlock1[(((check << 6) + kpos) << 6) + pos];
	if (block1 != 0x40)
	{
		if ((Z & TblBishopPath[(block1 << 6) + pos]) == 0)
			::AddMoveBlk(pos, block1);
	}
	byte block2 = TblBishopBlock2[(((check << 6) + kpos) << 6) + pos];
	if (block2 != 0x40)
	{
		if ((Z & TblBishopPath[(block2 << 6) + pos]) == 0)
			::AddMoveBlk(pos, block2);
	}
}

static void BlockByPawnBlk(int pos, int check)
{
	byte block = TblBlkPawnBlock[(((check << 6) + kpos) << 6) + pos];
	if (block != 0x40)
	{
		if (check >= 8 || kpos >= 8)
			::AddMoveBlk(pos, block);
		else
		{
			//Ö±×ßÉý±ä
			::BlkBian(pos, block);
		}
	}
	byte block2 = TblBlkPawnBlock2[(((check << 6) + kpos) << 6) + pos];
	if (block != 0x40)
	{
		if (board[pos - 8] == 0)
			::AddMoveBlk(pos, block);
	}
}

void GenBlockLine(int check)
{
	TPiece *Piece = PieceBlkB;
	while (Piece)
	{
		int pos = Piece->pos;
		switch (Piece->kind)
		{
		case B_ROOK:
			::BlockByRookBlk(pos, check);
			break;
		case B_HORSE:
			::BlockByHorseBlk(pos, check);
			break;
		case B_BISHOP:
			::BlockByBishopBlk(pos, check);
			break;
		case B_QUEEN:
			::BlockByRookBlk(pos, check);
			::BlockByBishopBlk(pos, check);
			break;
		case B_PAWN:
			::BlockByPawnBlk(pos, check);
			break;
		}
		Piece =	Piece->next;
	}
}
