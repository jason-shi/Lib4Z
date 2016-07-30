#include "StdAfx.h"
#include "WChess.h"

void InitBoard()
{
	for (int i = 0; i < 8; i++)
	{
		int *grid = board + (i << 3);
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
		&PieceRedB, &PieceRedB, &PieceRedB, &PieceRedB, &PieceRedB, &PieceRedK,
		&PieceBlkB, &PieceBlkB, &PieceBlkB, &PieceBlkB, &PieceBlkB, &PieceBlkK,
	};

	PieceRedB = PieceRedK = NULL;
	PieceBlkB = PieceBlkK = NULL;
	g_pZDeposit->Reclaim();

	//根据盘面初始化Z, g_hPBoard, PL
	Z = 0;
	for (int i = 0; i < 64; i++)
	{
		if (board[i] > 0)
		{
			Z |= (qword)1 << i;
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
}

bool InCheckRed()
{
	TPiece *Piece = PieceBlkB;
	while (Piece)
	{
		switch (Piece->kind)
		{
		case B_ROOK:
			if ((Z & TblRookPath[(Piece->pos << 6) + rkpos]) == 0)
				return true;
			break;
		case B_BISHOP:
			if ((Z & TblBishopPath[(Piece->pos << 6) + rkpos]) == 0)
				return true;
			break;
		case B_QUEEN:
			if ((Z & TblQueenPath[(Piece->pos << 6) + rkpos]) == 0)
				return true;
			break;
		case B_HORSE:
			if (TblHorseChkIdx[(Piece->pos << 6) + rkpos] == 1)
				return true;
			break;
		}
		Piece = Piece->next;
	}	
	if ((TblKingMove[rkpos] & 0x40) && board[rkpos + 7] == B_PAWN)
		return true;
	if ((TblKingMove[rkpos] & 0x01) && board[rkpos + 9] == B_PAWN)
		return true;
	return TblKingChkIdx[(kpos << 6) + rkpos] == 1;
}

bool InCheckBlk()
{
	TPiece *Piece = PieceRedB;
	while (Piece)
	{
		switch (Piece->kind)
		{
		case R_ROOK:
			if ((Z & TblRookPath[(Piece->pos << 6) + kpos]) == 0)
				return true;
			break;
		case R_BISHOP:
			if ((Z & TblBishopPath[(Piece->pos << 6) + kpos]) == 0)
				return true;
			break;
		case R_QUEEN:
			if ((Z & TblQueenPath[(Piece->pos << 6) + kpos]) == 0)
				return true;
			break;
		case R_HORSE:
			if (TblHorseChkIdx[(Piece->pos << 6) + kpos] == 1)
				return true;
			break;
		}
		Piece = Piece->next;
	}
	if ((TblKingMove[kpos] & 0x10) && board[kpos - 9] == R_PAWN)
		return true;
	if ((TblKingMove[kpos] & 0x04) && board[kpos - 7] == R_PAWN)
		return true;
	return TblKingChkIdx[(kpos << 6) + rkpos] == 1;
}

bool CalcCheckPos(int &nCheckPos)
{
	TPiece *Piece = PieceRedB;
	while (Piece)
	{
		switch (Piece->kind)
		{
		case R_ROOK:
			if ((Z & TblRookPath[(Piece->pos << 6) + kpos]) == 0)
			{
				nCheckPos = Piece->pos;
				return true;
			}
			break;
		case R_BISHOP:
			if ((Z & TblBishopPath[(Piece->pos << 6) + kpos]) == 0)
			{
				nCheckPos = Piece->pos;
				return true;
			}
			break;
		case R_QUEEN:
			if ((Z & TblQueenPath[(Piece->pos << 6) + kpos]) == 0)
			{
				nCheckPos = Piece->pos;
				return true;
			}
			break;
		case R_HORSE:
			if (TblHorseChkIdx[(Piece->pos << 6) + kpos] == 1)
			{
				nCheckPos = Piece->pos;
				return true;
			}
			break;
		}
		Piece = Piece->next;
	}
	if ((TblKingMove[kpos] & 0x10) && board[kpos - 9] == R_PAWN)
	{
		nCheckPos = kpos - 9;
		return true;
	}
	if ((TblKingMove[kpos] & 0x04) && board[kpos - 7] == R_PAWN)
	{
		nCheckPos = kpos - 7;
		return true;
	}

	return false;
}
