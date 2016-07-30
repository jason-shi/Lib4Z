#include "StdAfx.h"
#include "Generate.h"
#include "MoveList\MoveList.h"

static void CalcPinMask()
{
	pinmask = 0;

	TPiece *Piece = PieceRedB;
	while (Piece)
	{
		int pos = Piece->pos;
		qword _Z;
		switch (Piece->kind)
		{
		case R_ROOK:
			_Z = Z & TblRookPath[(pos << 6) + kpos];
			if ((_Z & _Z - 1) == 0)
				pinmask |= _Z;
			break;
		case R_BISHOP:
			_Z = Z & TblBishopPath[(pos << 6) + kpos];
			if ((_Z & _Z - 1) == 0)
				pinmask |= _Z;
			break;
		case R_QUEEN:
			_Z = Z & TblQueenPath[(pos << 6) + kpos];
			if ((_Z & _Z - 1) == 0)
				pinmask |= _Z;
			break;
		}
		Piece = Piece->next;
	}
}

bool IsPinnedByRed(int pos)
{
	if (pinmask & ((qword)1 << pos))
		return true;

	return false;
}

static bool InTheWay(TMove *move, int src, int dest, int kind)
{
	qword block = (qword)1 << move->dest;
	switch (kind)
	{
	case B_ROOK:
		return (TblRookPath[(src << 6) + dest] & block) != 0;
	case B_BISHOP:
		return (TblBishopPath[(src << 6) + dest] & block) != 0;
	case B_QUEEN:
		return (TblQueenPath[(src << 6) + dest] & block) != 0;
	}
	return false;
}

#define Move(n)					MoveList[n][trial[n] - 1]
#define MoveAB					Move(nStep - i - 4)
#define MoveBC					Move(nStep - i - 2)
#define MoveXY					Move(nStep - i - 3)
#define MoveYZ					Move(nStep - i - 1)

//A->B 0  X->Y 1
//B->C 2  Y->Z 3
//C->A 4  Check Z->X
//now nStep is 4
static bool IsQueenKingCycle(int nStep, int src, int dest)
{
	if (board[src] != R_QUEEN || board[Move(nStep - 1)->dest] != B_KING)
		return false;
	if (src != Move(nStep - 2)->dest) //C
		return false;
	if (Move(nStep - 1)->pDeath != NULL)
		return false;
	if (Move(nStep - 2)->pDeath != NULL)
		return false;
	for (int i = 0; i < nStep - 2; i += 2)
	{
		if (MoveAB->pDeath != NULL)
			return false;
		if (MoveXY->pDeath != NULL)
			return false;
		if (MoveBC->src != MoveAB->dest) //B
			return false;
		if (MoveYZ->src != MoveXY->dest) //Y
			return false;
		if (dest != MoveAB->src) //A
			continue;
		if (TblKingAway[(MoveYZ->dest << 6) + MoveXY->src] == 1) //Z -> X
			return true;
	}
	return false;
}

//A->B 0  X->Y 1
//B->C 2  Y->Z 3
//C->D 4
//X is Z  Check A->D
//now nStep is 4
static bool IsShorttest(int nStep, int src, int dest)
{
	if (board[src] != R_QUEEN || board[Move(nStep - 1)->dest] != B_KING)
		return false;
	if (src != Move(nStep - 2)->dest) //C
		return false;
	if (Move(nStep - 1)->pDeath != NULL)
		return false;
	if (Move(nStep - 2)->pDeath != NULL)
		return false;
	for (int i = 0; i < nStep - 2; i += 2)
	{
		if (MoveAB->pDeath != NULL)
			return false;
		if (MoveXY->pDeath != NULL)
			return false;
		if (MoveBC->src != MoveAB->dest) //B
			return false;
		if (MoveYZ->src != MoveXY->dest) //Y
			return false;
		if (MoveXY->src == Move(nStep - 1)->dest) //X is Z
		{
			if (((Z ^ ((qword)1 << src)) & TblQueenPath[(dest << 6) + MoveAB->src]) == 0) //A -> D
				return true;
		}
	}
	return false;
}

static void RejectCycleRed(int nStep)
{
	if (nStep >= 2 && Taken[nStep] != Trial[nStep])
	{
		TMove *mb1 = MoveList[nStep - 1][trial[nStep - 1] - 1];
		TMove *mr1 = MoveList[nStep - 2][trial[nStep - 2] - 1];
		if (mr1->pDeath == NULL && mb1->pDeath == NULL) //前两步没有吃子
		{
			if (mr1->bian == 0 && mb1->bian == 0) //前两步没有升变
			{
				TMove *move = MoveList[nStep][Taken[nStep]];
				TMove *last = MoveList[nStep][Trial[nStep] - 1];
				//遍历不吃子的着法
				while (move <= last)
				{
					if (move->src == mr1->dest && move->dest == mr1->src)
					{
						//检测有没有棋挡住黑方还原的路线
						if (!::InTheWay(mr1, mb1->dest, mb1->src, board[mb1->dest]))
							move->bValid = false;
						break;
					}
					move++;
				}
			}
		}
	}
	if (nStep >= 4 && Taken[nStep] != Trial[nStep])
	{
		TMove *move = MoveList[nStep][Taken[nStep]];
		TMove *last = MoveList[nStep][Trial[nStep] - 1];
		while (move <= last)
		{
			if (/*::IsQueenKingCycle(nStep, move->src, move->dest)
				|| */::IsShorttest(nStep, move->src, move->dest))
			{
				move->bValid = false;
				break;
			}
			move++;
		}
	}
}

void GenerateRed(bool bRejectCycle)
{
	Trial[nStep] = trial[nStep] = Taken[nStep] = 0;

	::CalcPinMask();

	TPiece *Piece = PieceRedB;
	while (Piece)
	{
		switch (Piece->kind)
		{
		case R_ROOK:
			::GenRookRed(Piece->pos);
			break;
		case R_BISHOP:
			::GenBishopRed(Piece->pos);
			break;
		case R_QUEEN:
			::GenQueenRed(Piece->pos);
			break;
		case R_HORSE:
			::GenHorseRed(Piece->pos);
			break;
		case R_PAWN:
			::GenPawnRed(Piece->pos);
			break;
		}
		Piece = Piece->next;
	}

	if (bRejectCycle)
		::RejectCycleRed(nStep);
}

void GenerateBlk(int check, int away)
{
	Trial[nStep] = trial[nStep] = Taken[nStep] = 0;

	::CalcPinMask();

	//除黑将外吃将军的子
	::GenTakeChecker(check);

	//对车、相、后挡将
	switch (board[check])
	{
	case R_ROOK:
	case R_BISHOP:
	case R_QUEEN:
		::GenBlockLine(check);
		break;
	}

	//黑将限制性逃跑
	::GenEludeLimited(check, away);
}
