#include "StdAfx.h"
#include "CChess.h"

void InitBoard()
{
	//效率要求：中低
	for (int i = 0; i < 10; i++)
	{
		int *grid = board + (i << 4);
		*grid++ = 0;
		*grid++ = 0;
		*grid++ = 0;
		*grid++ = 0;
		*grid++ = 0;
		*grid++ = 0;
		*grid++ = 0;
		*grid++ = 0;
		*grid++ = 0;
	}
}

void UpdateKingFamily()
{
	int mkside;
	int old = kx + (ky << 4);
	//Update independently and conditionally: kx, ky, kbtm, klft, krgt, kb1, kb2, kb3, kl1, kl2, kl3, kr1, kr2, kr3
	kx = kpos & 0xF;
	ky = kpos >> 4;
	if (old != kpos || kbtm != (vert[kx] & TblLowMask[ky]))
	{
		mkside = kbtm = vert[kx] & TblLowMask[ky];
		if (mkside == 0)
			kb1 = kb2 = kb3 = CLGRID;
		else
		{
			kb1 = kx + (TblFirstBit[mkside] << 4);
			kb2 = TblSecondBit[mkside] == 10 ? CLGRID : kx + (TblSecondBit[mkside] << 4);
			kb3 = TblThirdBit[mkside] == 10 ? CLGRID : kx + (TblThirdBit[mkside] << 4);
		}
	}
	if (old != kpos || klft != (horz[ky] & TblLowMask[kx]))
	{
		mkside = klft = horz[ky] & TblLowMask[kx];
		if (mkside == 0)
			kl1 = kl2 = kl3 = CLGRID;
		else
		{
			kl1 = TblFirstBit[mkside] + (ky << 4);
			kl2 = TblSecondBit[mkside] == 10 ? CLGRID : TblSecondBit[mkside] + (ky << 4);
			kl3 = TblThirdBit[mkside] == 10 ? CLGRID : TblThirdBit[mkside] + (ky << 4);
		}
	}
	if (old != kpos || krgt != (horz[ky] & TblHighMask[kx]))
	{
		mkside = krgt = horz[ky] & TblHighMask[kx];
		if (mkside == 0)
			kr1 = kr2 = kr3 = CLGRID;
		else
		{
			kr1 = TblLastBit[mkside] + (ky << 4);
			kr2 = TblLagendBit[mkside] == 10 ? CLGRID : TblLagendBit[mkside] + (ky << 4);
			kr3 = TblLathidBit[mkside] == 10 ? CLGRID : TblLathidBit[mkside] + (ky << 4);
		}
	}
}

static void PushFront_PL(TPiece **pList, TPiece *pPiece)
{
	pPiece->next = *pList;
	if (*pList)
		(*pList)->prev = pPiece;
	*pList = pPiece;
	(*pList)->prev = (TPiece *)pList;
}

void GetBoardDynData()
{
	//效率要求：中低
	static TPiece **TblPieceList[] = { NULL,
		&PieceRedR, &PieceRedH, &PieceRedS, &PieceRedS, &PieceRedC, &PieceRedS, &PieceRedK,
		&PieceBlkB, &PieceBlkB, &PieceBlkS, &PieceBlkS, &PieceBlkB, &PieceBlkS, &PieceBlkK,
	};

	PieceRedR = PieceRedH = PieceRedC = PieceRedS = PieceRedK = NULL;
	PieceBlkB = PieceBlkS = PieceBlkK = NULL;
	g_pZDeposit->Reclaim();

	memset(horz, 0, sizeof(horz));
	memset(vert, 0, sizeof(vert));
	//根据盘面初始化horz, vert, g_hPBoard, PL
	for (int i = 0; i < CLGRID; i++)
	{
		if (board[i] > 0)
		{
			horz[i >> 4] |= 1 << (i & 0xF);
			vert[i & 0xF] |= 1 << (i >> 4);

			TPiece *pPiece = (TPiece *)g_pZDeposit->GetNode();
			pPiece->kind = board[i];
			pPiece->pos = i;
			g_hPBoard[i] = pPiece;
			::PushFront_PL(TblPieceList[board[i]], pPiece);
		}
	}
	//根据盘面初始化将族
	rkpos = PieceRedK->pos;
	kpos = PieceBlkK->pos;
	::UpdateKingFamily();
}

bool InCheckRed()
{
	int rkx = rkpos & 0xF;
	int rky = rkpos >> 4;
	TPiece *Piece = PieceBlkB;
	while (Piece)
	{
		int px = Piece->pos & 0xF;
		int py = Piece->pos >> 4;
		switch (Piece->kind)
		{
		case B_ROOK:
			if (rkx == px && (vert[rkx] & TblBitrim[(rky << 4) + py]) == 0
				|| rky == py && (horz[rky] & TblBitrim[(rkx << 4) + px]) == 0)
				return true;
			break;
		case B_HORSE:
			if (TblHorseLeg[Piece->pos - rkpos + 160] != 0
				&& board[Piece->pos + TblHorseLeg[Piece->pos - rkpos + 160]] == 0)
				return true;
			break;
		default:
			if (rkx == px && TblNumberOne[vert[rkx] & TblBitrim[(rky << 4) + py]] == 1
				|| rky == py && TblNumberOne[horz[rky] & TblBitrim[(rkx << 4) + px]] == 1)
				return true;
			break;
		}
		Piece = Piece->next;
	}
	if (rkx == kx && (vert[rkx] & TblBitrim[(kpos & 0xF0) + rky]) == 0)
		return true;
	return board[rkpos + 1] == B_PAWN || board[rkpos - 1] == B_PAWN || board[rkpos + 16] == B_PAWN;
}

bool InCheckBlk()
{
	//效率要求：高
	int rkx = rkpos & 0xF;
	int rky = rkpos >> 4;
	if (PieceRedR)
	{
		int px = PieceRedR->pos & 0xF;
		int py = PieceRedR->pos >> 4;
		if (kx == px && (vert[kx] & TblBitrim[(ky << 4) + py]) == 0
			|| ky == py && (horz[ky] & TblBitrim[(kx << 4) + px]) == 0)
			return true;
		if (PieceRedR->next)
		{
			px = PieceRedR->next->pos & 0xF;
			py = PieceRedR->next->pos >> 4;
			if (kx == px && (vert[kx] & TblBitrim[(ky << 4) + py]) == 0
				|| ky == py && (horz[ky] & TblBitrim[(kx << 4) + px]) == 0)
				return true;
		}
	}
	if (PieceRedC)
	{
		int px = PieceRedC->pos & 0xF;
		int py = PieceRedC->pos >> 4;
		if (kx == px && TblNumberOne[vert[kx] & TblBitrim[(ky << 4) + py]] == 1
			|| ky == py && TblNumberOne[horz[ky] & TblBitrim[(kx << 4) + px]] == 1)
			return true;
		if (PieceRedC->next)
		{
			px = PieceRedC->next->pos & 0xF;
			py = PieceRedC->next->pos >> 4;
			if (kx == px && TblNumberOne[vert[kx] & TblBitrim[(ky << 4) + py]] == 1
				|| ky == py && TblNumberOne[horz[ky] & TblBitrim[(kx << 4) + px]] == 1)
				return true;
		}
	}
	if (PieceRedH)
	{
		if (TblHorseLeg[PieceRedH->pos - kpos + 160] != 0
			&& board[PieceRedH->pos + TblHorseLeg[PieceRedH->pos - kpos + 160]] == 0)
			return true;
		if (PieceRedH->next)
		{
			if (TblHorseLeg[PieceRedH->next->pos - kpos + 160] != 0
				&& board[PieceRedH->next->pos + TblHorseLeg[PieceRedH->next->pos - kpos + 160]] == 0)
				return true;
		}
	}
	if (rkx == kx && (vert[rkx] & TblBitrim[(kpos & 0xF0) + rky]) == 0)
		return true;
	return board[kpos + 1] == R_PAWN || board[kpos - 1] == R_PAWN || board[kpos - 16] == R_PAWN;
}

bool CalcCheckPos(int &nCheckPos)
{
	//注：必须先算兵，车，再算炮
	if (board[kpos + 1] == R_PAWN)
	{
		nCheckPos = kpos + 1;
		return true;
	}
	if (board[kpos - 1] == R_PAWN)
	{
		nCheckPos = kpos - 1;
		return true;
	}
	if (board[kpos - 16] == R_PAWN)
	{
		nCheckPos = kpos - 16;
		return true;
	}

	TPiece *pPiece = PieceRedR;
	while (pPiece)
	{
		int px = pPiece->pos & 0xF;
		int py = pPiece->pos >> 4;
		if (kx == px && (vert[kx] & TblBitrim[(ky << 4) + py]) == 0
			|| ky == py && (horz[ky] & TblBitrim[(kx << 4) + px]) == 0)
		{
			nCheckPos = pPiece->pos;
			return true;
		}
		pPiece = pPiece->next;
	}
	pPiece = PieceRedC;
	while (pPiece)
	{
		int px = pPiece->pos & 0xF;
		int py = pPiece->pos >> 4;
		if (kx == px && TblNumberOne[vert[kx] & TblBitrim[(ky << 4) + py]] == 1
			|| ky == py && TblNumberOne[horz[ky] & TblBitrim[(kx << 4) + px]] == 1)
		{
			nCheckPos = pPiece->pos;
			return true;
		}
		pPiece = pPiece->next;
	}
	pPiece = PieceRedH;
	while (pPiece)
	{
		int pos = pPiece->pos;
		if (TblHorseLeg[pos - kpos + 160] != 0 && board[pos + TblHorseLeg[pos - kpos + 160]] == 0)
		{
			nCheckPos = pPiece->pos;
			return true;
		}
		pPiece = pPiece->next;
	}

	return false;
}
