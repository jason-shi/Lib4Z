#include "StdAfx.h"
#include "Generate.h"
#include "MoveList\MoveList.h"

static void CalcPinMask()
{
	pinmask = 0;
	//����˧��˩��
	if (board[kl2] == R_ROOK || board[kl3] == R_CANNON)
		pinmask |= 16;
	if (board[kr2] == R_ROOK || board[kr3] == R_CANNON)
		pinmask |= 32;
	if (board[kb2] == R_ROOK || board[kb3] == R_CANNON || board[kb2] == STONE)
		pinmask |= 64;
	//�����˩��
	if (PieceRedH)
	{
		int dpos = PieceRedH->pos - kpos + 160;
		if (TblHorseLeg[dpos])
			pinmask |= TblPinByRed[dpos + TblHorseLeg[dpos]];
		if (PieceRedH->next)
		{
			int dpos = PieceRedH->next->pos - kpos + 160;
			if (TblHorseLeg[dpos])
				pinmask |= TblPinByRed[dpos + TblHorseLeg[dpos]];
		}
	}
}

bool IsPinnedByRed(int pos)
{
	int idx = TblPinByRed[pos - kpos + 160];
	switch (idx)
	{
	case 0: //�������
		return false;
	case 16:
		return (pinmask & idx) && pos >= kl2;
	case 32:
		return (pinmask & idx) && pos <= kr2;
	case 64:
		return (pinmask & idx) && pos >= kb2;
	default:
		return (pinmask & idx) !=0;
	}
}

static bool InTheWay(TMove *move, int src, int dest, int kind)
{
	switch (kind)
	{
	case B_ROOK:
	case B_CANNON:
		kind = move->src; //����Ϊ���
		return TblSameLine[kind ^ src] && TblSameLine[kind ^ dest] && (src < kind) == (kind < dest);
	case B_HORSE:
		kind = src + TblHorseLeg[src - dest + 160]; //����Ϊ����
		return board[kind] != 0 && move->src != kind || move->dest == kind;
	case B_BISHOP:
		return src + dest == move->dest << 1;
	}
	return false;
}

static void RejectCycleRed(int nStep)
{
	if (nStep >= 2 && Taken[nStep] != Trial[nStep])
	{
		TMove *mb1 = MoveList[nStep - 1][trial[nStep - 1] - 1];
		TMove *mr1 = MoveList[nStep - 2][trial[nStep - 2] - 1];
		if (mr1->pDeath == NULL && mb1->pDeath == NULL) //ǰ����û�г���
		{
			TMove *move = MoveList[nStep][Taken[nStep]];
			TMove *last = MoveList[nStep][Trial[nStep] - 1];
			//���������ӵ��ŷ�
			while (move <= last)
			{
				if (move->src == mr1->dest && move->dest == mr1->src)
				{
					//�����û���嵲ס�ڷ���ԭ��·��
					if (!::InTheWay(mr1, mb1->dest, mb1->src, board[mb1->dest]))
						move->bValid = false;
					break;
				}
				move++;
			}
		}
	}
}

void GenerateRed(bool bRejectCycle)
{
	Trial[nStep] = trial[nStep] = Taken[nStep] = 0;

	::CalcPinMask();
	//�����������ŷ�����ǰ��
	if (PieceRedR)
	{
		::GenRookRed(PieceRedR->pos);
		if (PieceRedR->next)
			::GenRookRed(PieceRedR->next->pos);
	}
	if (PieceRedH)
	{
		::GenHorseRed(PieceRedH->pos);
		if (PieceRedH->next)
			::GenHorseRed(PieceRedH->next->pos);
	}
	if (PieceRedC)
	{
		::GenCannonRed(PieceRedC->pos);
		if (PieceRedC->next)
			::GenCannonRed(PieceRedC->next->pos);
	}
	::GenPawnRed();

	if (bRejectCycle)
		::RejectCycleRed(nStep);
}

void GenerateBlk(int check)
{
	Trial[nStep] = trial[nStep] = Taken[nStep] = 0;

	::CalcPinMask();
	//���ڽ���Խ�������
	::GenTakeChecker(check);
	switch (board[check])
	{
	case R_ROOK:
		::GenRookBlk(check);
		break;
	case R_HORSE:
		::GenBlockSingle(check + TblHorseLeg[check - kpos + 160]);
		::GenEludeUnlimited();
		break;
	case R_CANNON:
		::GenCannonBlk(check);
		break;
	default:
		::GenEludeUnlimited();
		break;
	}
}
