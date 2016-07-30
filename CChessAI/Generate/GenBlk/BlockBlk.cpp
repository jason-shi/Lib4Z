#include "StdAfx.h"
#include "Generate\Generate.h"
#include "MoveList\MoveList.h"

#define AddNPinMove(src, dest) \
{ \
	if (!::IsPinnedByRed(src)) \
		::AddMoveBlk(src, dest); \
}

void GenBlockSingle(int single)
{
	int sgx = single & 0xF;
	int sgy = single >> 4;
	int px, py;
	TPiece *Piece = PieceBlkB;
	while (Piece)
	{
		int pos = Piece->pos;
		switch (Piece->kind)
		{
		case B_HORSE:
			if (TblHorseLeg[pos - single + 160] != 0 && board[pos + TblHorseLeg[pos - single + 160]] == 0)
				AddNPinMove(pos, single);
			break;
		default:
			GetPxPy(pos, px, py);
			if (px == sgx && (vert[px] & TblBitrim[(py << 4) + sgy]) == 0 || py == sgy && (horz[py] & TblBitrim[(px << 4) + sgx]) == 0)
				AddNPinMove(pos, single);
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
			px = pos & 0xF;
			if (TblBishopSphere[single] && (px - sgx == 2 || px - sgx == -2) && board[(pos + single) >> 1] == 0)
				AddNPinMove(pos, single);
			break;
		case B_GUARD:
			if (TblGuardSphere[single] && (single == 132 || pos == 132))
				AddNPinMove(pos, single);
			break;
		case B_PAWN:
			if ((pos + 1 == single || pos - 1 == single) && pos < 80 || pos - 16 == single)
				AddNPinMove(pos, single);
			break;
		}
		Piece =	Piece->next;
	}
}

void GenBlockRookH(int begin, int end)
{
	int px, py;
	TPiece *Piece = PieceBlkB;
	while (Piece)
	{
		int pos = Piece->pos;
		GetPxPy(pos, px, py);
		switch (Piece->kind)
		{
		case B_HORSE:
			if (py - ky == 2 && board[pos - 16] == 0 || py - ky == -2 && board[pos + 16] == 0)
			{
				if (px + 1 >= begin && px + 1 <= end)
					AddNPinMove(pos, px + 1 + (ky << 4));
				if (px - 1 >= begin && px - 1 <= end)
					AddNPinMove(pos, px - 1 + (ky << 4));
			}
			else if (py - ky == 1 || py - ky == -1)
			{
				if (px + 2 >= begin && px + 2 <= end && board[pos + 1] == 0)
					AddNPinMove(pos, px + 2 + (ky << 4));
				if (px - 2 >= begin && px - 2 <= end && board[pos - 1] == 0)
					AddNPinMove(pos, px - 2 + (ky << 4));
			}
			break;
		case B_ROOK:
		case B_CANNON:
			if (px >= begin && px <= end && (vert[px] & TblBitrim[(py << 4) + ky]) == 0)
				AddNPinMove(pos, px + (ky << 4));
			break;
		}
		Piece =	Piece->next;
	}
	//用士挡车
	if (ky == 8)
	{
		if (begin <= 4 && end >= 4)
		{
			if (board[115] == B_GUARD)
				AddNPinMove(115, 132);
			if (board[117] == B_GUARD)
				AddNPinMove(117, 132);
			if (board[147] == B_GUARD)
				AddNPinMove(147, 132);
			if (board[149] == B_GUARD)
				AddNPinMove(149, 132);
		}
	}
	else if (board[132] == B_GUARD)
	{
		if (begin <= 3 && end >= 3)
			AddNPinMove(132, 3 + (ky << 4));
		if (begin <= 5 && end >= 5)
			AddNPinMove(132, 5 + (ky << 4));
	}
	//用象挡车
	if (ky == 9)
	{
		if (begin <= 2)
		{
			if (board[112] == B_BISHOP && board[129] == 0)
				AddNPinMove(112, 146);
			if (board[116] == B_BISHOP && board[131] == 0)
				AddNPinMove(116, 146);
		}
		else if (end >= 6)
		{
			if (board[120] == B_BISHOP && board[135] == 0)
				AddNPinMove(120, 150);
			if (board[116] == B_BISHOP && board[133] == 0)
				AddNPinMove(116, 150);
		}
	}
	else if (ky == 7 && begin <= 4 && end >= 4)
	{
		if (board[82] == B_BISHOP && board[99] == 0)
			AddNPinMove(82, 116);
		if (board[86] == B_BISHOP && board[101] == 0)
			AddNPinMove(86, 116);
		if (board[146] == B_BISHOP && board[131] == 0)
			AddNPinMove(146, 116);
		if (board[150] == B_BISHOP && board[133] == 0)
			AddNPinMove(150, 116);
	}
}

void GenBlockRookV(int begin, int end)
{
	int px, py;
	TPiece *Piece = PieceBlkB;
	while (Piece)
	{
		int pos = Piece->pos;
		GetPxPy(pos, px, py);
		switch (Piece->kind)
		{
		case B_HORSE:
			if (px - kx == 2 && board[pos - 1] == 0 || px - kx == -2 && board[pos + 1] == 0)
			{
				if (py + 1 >= begin && py + 1 <= end)
					AddNPinMove(pos, kx + ((py + 1) << 4));
				if (py - 1 >= begin && py - 1 <= end)
					AddNPinMove(pos, kx + ((py - 1) << 4));
			}
			else if (px - kx == 1 || px - kx == -1)
			{
				if (py + 2 >= begin && py + 2 <= end && board[pos + 16] == 0)
					AddNPinMove(pos, kx + ((py + 2) << 4));
				if (py - 2 >= begin && py - 2 <= end && board[pos - 16] == 0)
					AddNPinMove(pos, kx + ((py - 2) << 4));
			}
			break;
		case B_ROOK:
		case B_CANNON:
			if (py >= begin && py <= end && (horz[py] & TblBitrim[(px << 4) + kx]) == 0)
				AddNPinMove(pos, kx + (py << 4));
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
			if ((kpos == 132 || kpos == 148) && (pos >> 4) != 7 && board[(pos + 116) >> 1] == 0)
				AddNPinMove(pos, 116);
			break;
		case B_GUARD:
			if (kpos == 148)
				AddNPinMove(pos, 132);
			if (pos == 132 && ky != 7)
				AddNPinMove(132, kx + 112);
			break;
		case B_PAWN:
			GetPxPy(pos, px, py);
			if ((px == kx + 1 || px == kx - 1) && py >= begin && py <= end && pos < 80)
				AddNPinMove(pos, kx + (py << 4));
			break;
		}
		Piece =	Piece->next;
	}
}

void GenBlockCannonH(int begin, int end, int shell)
{
	int px, py;
	TPiece *Piece = PieceBlkB;
	while (Piece)
	{
		int pos = Piece->pos;
		GetPxPy(pos, px, py);
		switch (Piece->kind)
		{
		case B_HORSE:
			if (py - ky == 2 && board[pos - 16] == 0 || py - ky == -2 && board[pos + 16] == 0)
			{
				if (px + 1 >= begin && px + 1 <= end && px + 1 != shell)
					AddNPinMove(pos, px + 1 + (ky << 4));
				if (px - 1 >= begin && px - 1 <= end && px - 1 != shell)
					AddNPinMove(pos, px - 1 + (ky << 4));
			}
			else if (py - ky == 1 || py - ky == -1)
			{
				if (px + 2 >= begin && px + 2 <= end && board[pos + 1] == 0 && px + 2 != shell)
					AddNPinMove(pos, px + 2 + (ky << 4));
				if (px - 2 >= begin && px - 2 <= end && board[pos - 1] == 0 && px - 2 != shell)
					AddNPinMove(pos, px - 2 + (ky << 4));
			}
			break;
		case B_ROOK:
		case B_CANNON:
			if (px >= begin && px <= end && (vert[px] & TblBitrim[(py << 4) + ky]) == 0 && px != shell)
				AddNPinMove(pos, px + (ky << 4));
			break;
		}
		Piece =	Piece->next;
	}
	//用士挡炮
	if (ky == 8)
	{
		if (begin <= 4 && end >= 4 && shell != 4)
		{
			if (board[115] == B_GUARD)
				AddNPinMove(115, 132);
			if (board[117] == B_GUARD)
				AddNPinMove(117, 132);
			if (board[147] == B_GUARD)
				AddNPinMove(147, 132);
			if (board[149] == B_GUARD)
				AddNPinMove(149, 132);
		}
	}
	else if (board[132] == B_GUARD)
	{
		if (begin <= 3 && end >= 3 && shell != 3)
			AddNPinMove(132, 3 + (ky << 4));
		if (begin <= 5 && end >= 5 && shell != 5)
			AddNPinMove(132, 5 + (ky << 4));
	}
	//用象挡炮
	if (ky == 9)
	{
		if (begin <= 2 && shell != 2)
		{
			if (board[112] == B_BISHOP && board[129] == 0)
				AddNPinMove(112, 146);
			if (board[116] == B_BISHOP && board[131] == 0)
				AddNPinMove(116, 146);
		}
		else if (end >= 6 && shell != 6)
		{
			if (board[120] == B_BISHOP && board[135] == 0)
				AddNPinMove(120, 150);
			if (board[116] == B_BISHOP && board[133] == 0)
				AddNPinMove(116, 150);
		}
	}
	else if (ky == 7 && begin <= 4 && end >= 4 && shell != 4)
	{
		if (board[82] == B_BISHOP && board[99] == 0)
			AddNPinMove(82, 116);
		if (board[86] == B_BISHOP && board[101] == 0)
			AddNPinMove(86, 116);
		if (board[146] == B_BISHOP && board[131] == 0)
			AddNPinMove(146, 116);
		if (board[150] == B_BISHOP && board[133] == 0)
			AddNPinMove(150, 116);
	}
}

void GenBlockCannonV(int begin, int end, int shell)
{
	int px, py;
	TPiece *Piece = PieceBlkB;
	while (Piece)
	{
		int pos = Piece->pos;
		GetPxPy(pos, px, py);
		switch (Piece->kind)
		{
		case B_HORSE:
			if (px - kx == 2 && board[pos - 1] == 0 || px - kx == -2 && board[pos + 1] == 0)
			{
				if (py + 1 >= begin && py + 1 <= end && py + 1 != shell)
					AddNPinMove(pos, kx + ((py + 1) << 4));
				if (py - 1 >= begin && py - 1 <= end && py - 1 != shell)
					AddNPinMove(pos, kx + ((py - 1) << 4));
			}
			else if (px - kx == 1 || px - kx == -1)
			{
				if (py + 2 >= begin && py + 2 <= end && board[pos + 16] == 0 && py + 2 != shell)
					AddNPinMove(pos, kx + ((py + 2) << 4));
				if (py - 2 >= begin && py - 2 <= end && board[pos - 16] == 0 && py - 2 != shell)
					AddNPinMove(pos, kx + ((py - 2) << 4));
			}
			break;
		case B_ROOK:
		case B_CANNON:
			if (py >= begin && py <= end && (horz[py] & TblBitrim[(px << 4) + kx]) == 0 && py != shell)
				AddNPinMove(pos, kx + (py << 4));
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
		case B_BISHOP: //将,象,象腿,炮架
			if ((kpos == 132 || kpos == 148) && (pos >> 4) != 7 && board[(pos + 116) >> 1] == 0 && shell != 7)
				AddNPinMove(pos, 116);
			break;
		case B_GUARD:
			if (kpos == 148)
			{
				if (shell != 8)
					AddNPinMove(pos, 132);
			}
			else if (pos == 132 && ky != 7 && shell != 7)
				AddNPinMove(132, kx + 112);
			break;
		case B_PAWN:
			GetPxPy(pos, px, py);
			if ((px == kx + 1 || px == kx - 1) && py >= begin && py <= end && pos < 80 && py != shell)
				AddNPinMove(pos, kx + (py << 4));
			break;
		}
		Piece =	Piece->next;
	}
}
