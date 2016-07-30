#include "StdAfx.h"
#include "MoveList.h"

using namespace std;

bool CreateMoveList()
{
	for (int i = 0; i < MaxCrackDepth; i++)
	{
		*MoveList[i] = new TMove[MaxMove];
		if (*MoveList[i] == NULL)
			return false;
		for (int j = 0; j < MaxMove; j++)
			MoveList[i][j] = *MoveList[i] + j;
	}
	return true;
}

void DeleteMoveList()
{
	for (int i = 0; i < MaxCrackDepth; i++)
	{
		delete *MoveList[i];
		*MoveList[i] = NULL;
	}
}

void AddMoveRed(int src, int dest, int check, int bian)
{
	TMove *move = (TMove *)(*MoveList[nStep]);
	move += Trial[nStep]++;
	move->src = src;
	move->dest = dest;
	move->check = check;
	move->bian = bian;
	move->pDeath = NULL;
	move->bValid = true;
}

void AddKillRed(int src, int dest, int check, int bian)
{
	TMove *move = (TMove *)(*MoveList[nStep]);
	if (board[dest] == 0)
	{
		move += Trial[nStep]++;
		move->src = src;
		move->dest = dest;
		move->check = check;
		move->bian = bian;
		move->pDeath = NULL;
		move->bValid = true;
	}
	else if (board[dest] >= B_ROOK)
	{
		move += Taken[nStep];
		if (Trial[nStep]++ != Taken[nStep]++)
		{
			//保持着法生成顺序有更好的性能
			TMove *last = move + Trial[nStep] - Taken[nStep];
			while (last > move)
			{
				TMove *temp = last - 1;
				last->src = temp->src;
				last->dest = temp->dest;
				last->check = temp->check;
				last->bian = temp->bian;
				last->pDeath = temp->pDeath;
				move->bValid = temp->bValid;
				last = temp;
			}
		}
		move->src = src;
		move->dest = dest;
		move->check = check;
		move->bian = bian;
		move->pDeath = g_hPBoard[dest];
		move->bValid = true;
	}
}

void AddMoveBlk(int src, int dest, int bian)
{
	TMove *move = (TMove *)(*MoveList[nStep]);
	move += Trial[nStep]++;
	move->src = src;
	move->dest = dest;
	move->bian = bian;
	move->pDeath = NULL;
	move->bValid = true;
}

void AddKillBlk(int src, int dest, int bian)
{
	TMove *move = (TMove *)(*MoveList[nStep]);
	if (board[dest] == 0)
	{
		move += Trial[nStep]++;
		move->src = src;
		move->dest = dest;
		move->bian = bian;
		move->pDeath = NULL;
		move->bValid = true;
	}
	else if (board[dest] < B_ROOK)
	{
		move += Taken[nStep];
		if (Trial[nStep]++ != Taken[nStep]++)
		{
			//保持着法生成顺序有更好的性能
			TMove *last = move + Trial[nStep] - Taken[nStep];
			while (last > move)
			{
				TMove *temp = last - 1;
				last->src = temp->src;
				last->dest = temp->dest;
				last->bian = temp->bian;
				last->pDeath = temp->pDeath;
				move->bValid = temp->bValid;
				last = temp;
			}
		}
		move->src = src;
		move->dest = dest;
		move->bian = bian;
		move->pDeath = g_hPBoard[dest];
		move->bValid = true;
	}
}
