#include "StdAfx.h"
#include "Traversal.h"
#include "Generate\Generate.h"
#include "MoveList\MoveList.h"
#include "Astode\Astode.h"

#define GetAstode(n) (n < MaxBufferSize ? AstodeBuffer[n] : AstodeStore[n - MaxBufferSize])

void ForwardsRed()
{
	int nTrial = trial[nStep];
	TMove *pMove = MoveList[nStep][nTrial];

	int src = pMove->src;
	int dest = pMove->dest;

	if (pMove->pDeath)
		g_pZDeposit->Impawn((char *)pMove->pDeath);

	if (board[src] == R_KING)
		rkpos = dest;

	board[dest] = pMove->bian ? pMove->bian : board[src];
	board[src] = 0;
	g_hPBoard[dest] = g_hPBoard[src];
	g_hPBoard[src] = NULL;
	g_hPBoard[dest]->pos = dest;
	if (pMove->bian)
		g_hPBoard[dest]->kind = pMove->bian;

	Z &= ~((qword)1 << src);
	Z |= (qword)1 << dest;

	trial[nStep++]++;

#if 0

	if (nDepth != 4)
		goto GO;
	int t = 0;
	if (!(nStep > t && MoveList[t][trial[t] - 1]->src == 24 && MoveList[t][trial[t] - 1]->dest == 42))
		goto GO;
	t = 1;
	if (!(nStep > t && MoveList[t][trial[t] - 1]->src == 7 && MoveList[t][trial[t] - 1]->dest == 6))
		goto GO;
	t = 2;
	if (!(nStep > t && MoveList[t][trial[t] - 1]->src == 42 && MoveList[t][trial[t] - 1]->dest == 14))
		goto GO;
	t = 3;
	if (!(nStep > t && MoveList[t][trial[t] - 1]->src == 49 && MoveList[t][trial[t] - 1]->dest == 40))
		goto GO;

GO:;
#endif
}

void ForwardsBlk()
{
	int nTrial = trial[nStep];
	TMove *pMove = MoveList[nStep][nTrial];

	int src = pMove->src;
	int dest = pMove->dest;

	if (pMove->pDeath)
		g_pZDeposit->Impawn((char *)pMove->pDeath);

	if (board[src] == B_KING)
		kpos = dest;

	board[dest] = pMove->bian ? pMove->bian : board[src];
	board[src] = 0;
	g_hPBoard[dest] = g_hPBoard[src];
	g_hPBoard[src] = NULL;
	g_hPBoard[dest]->pos = dest;
	if (pMove->bian)
		g_hPBoard[dest]->kind = pMove->bian;

	Z &= ~((qword)1 << src);
	Z |= (qword)1 << dest;

	trial[nStep++]++;
}

void BackwardsRed()
{
	int nTrial = trial[--nStep] - 1;
	TMove *pMove = MoveList[nStep][nTrial];

	int src = pMove->src;
	int dest = pMove->dest;

	if (pMove->pDeath)
		g_pZDeposit->Redeem((char *)pMove->pDeath);

	if (board[dest] == R_KING)
		rkpos = src;

	board[src] = pMove->bian ? R_PAWN : board[dest];
	board[dest] = pMove->pDeath ? pMove->pDeath->kind : 0;
	g_hPBoard[src] = g_hPBoard[dest];
	g_hPBoard[dest] = pMove->pDeath;
	g_hPBoard[src]->pos = src;
	if (pMove->bian)
		g_hPBoard[src]->kind = R_PAWN;

	if (board[dest] == 0)
		Z &= ~((qword)1 << dest);

	Z |= (qword)1 << src;
}

void BackwardsBlk()
{
	int nTrial = trial[--nStep] - 1;
	TMove *pMove = MoveList[nStep][nTrial];

	int src = pMove->src;
	int dest = pMove->dest;

	if (pMove->pDeath)
		g_pZDeposit->Redeem((char *)pMove->pDeath);

	if (board[dest] == B_KING)
		kpos = src;

	board[src] = pMove->bian ? B_PAWN : board[dest];
	board[dest] = pMove->pDeath ? pMove->pDeath->kind : 0;
	g_hPBoard[src] = g_hPBoard[dest];
	g_hPBoard[dest] = pMove->pDeath;
	g_hPBoard[src]->pos = src;
	if (pMove->bian)
		g_hPBoard[src]->kind = B_PAWN;

	if (board[dest] == 0)
		Z &= ~((qword)1 << dest);

	Z |= (qword)1 << src;
}

static int RedMove(bool bGenerate)
{
	if (bGenerate)
	{
		::GenerateRed(!g_bAstable[nStep]);
		//非稳定节点排除红必负着法
		if (g_bAstable[nStep])
		{
			TAstode *pAstode = GetAstode(g_nBSIndex[nStep]);
			if (Trial[nStep] > 32)
				nStep = nStep;
			pAstode->tag2 &= (1 << Trial[nStep]) - 1;
			if (pAstode->tag2 == 0)
				pAstode->tag1 = NOFURTHER;
		}
	}
_a:	while (trial[nStep] < Trial[nStep] && !MoveList[nStep][trial[nStep]]->bValid)
		trial[nStep]++;
	if (trial[nStep] == Trial[nStep])
	{
		if (nStep == g_nBoundLower)
			return REDLOSE;
		::BackwardsBlk();
		::BackwardsRed();
		goto _a;
	}
	//专为小倒推使用
	if (nStep == 0)
	{
		TMove *move = MoveList[nStep][trial[nStep]];
		if (move->src == g_tCrackParam.src && move->dest == g_tCrackParam.dest)
		{
			trial[nStep]++;
			goto _a;
		}
	}
	if (g_bAstable[nStep])
	{
		if ((GetAstode(g_nBSIndex[nStep])->tag2 & (1 << trial[nStep])) == 0)
		{
			trial[nStep]++;
			goto _a;
		}
	}
	::ForwardsRed();
	if (InCheckRed())
	{
		::BackwardsRed();
		goto _a;
	}

	return NOLOSE;
}

static int BlackMove(bool bGenerate, int away)
{
	if (bGenerate)
		::GenerateBlk(MoveList[nStep - 1][trial[nStep - 1] - 1]->check, away);

_a:	if (trial[nStep] == Trial[nStep])
	{
		if (nStep == g_nBoundLower + 1)
		{
			::BackwardsRed();
			return BLKLOSE;
		}
		::BackwardsRed();
		//Astode相关处理
		if (g_bAstable[nStep])
		{
			TAstode *pAstode = GetAstode(g_nBSIndex[nStep]);
			pAstode->nWinD = pAstode->nLoseD;
			pAstode->nLoseD = pAstode->nVagueD;
		}
		::BackwardsBlk();
		goto _a;
	}
	::ForwardsBlk();
	if (InCheckBlk())
	{
		::BackwardsBlk();
		goto _a;
	}
	return NOLOSE;
}

static int CallBound(int nBoundUpper, int away)
{
	g_nBoundLower = nStep;
	nDepth = nStep + 2;

	while (nDepth <= nBoundUpper)
	{
		int nResult = Traversal(away);
		if (nResult == INTERRUPT)
			return INTERRUPT;
		if (nResult == FULLASTODE)
			return FULLASTODE;
		if (nResult == REDLOSE)
			nDepth += 2;
		else
		{
			//红赢
			if (g_bAstable[nStep])
			{
				TAstode *pAstode = GetAstode(g_nBSIndex[nStep]);
				pAstode->nWinD = pAstode->nLoseD;
				pAstode->nLoseD = pAstode->nVagueD;
			}
			return BLKLOSE;
		}
	}
	return NOLOSE;
}

int Traversal(int away)
{
	static TAstode tAstode = { 0 };
	while (!g_bInterrupt)
	{
		bool bGenerate = nStep < nDepth;
		if (nStep >= nDepth)
		{
			::BackwardsBlk();
			::BackwardsRed();
		}
		//到达思考最大深度导致的回退，此时红方已经生成过着法
_rm:	if (::RedMove(bGenerate) == REDLOSE)
			return REDLOSE;
		bGenerate = true;
_bm:	if (::BlackMove(bGenerate, away) == BLKLOSE)
			return BLKLOSE;

		g_bAstable[nStep] = false;
		int check = MoveList[nStep - 2][trial[nStep - 2] - 1]->check;
		int dest = MoveList[nStep - 1][trial[nStep - 1] - 1]->dest;
		if (nStep < g_nFurtherEnd)
		{
			g_bAstable[nStep] = true;
			ShrinkAstode(tAstode.data);
			int nResult = CallAstode(tAstode, nStep, nDepth - nStep);
			if (nResult == REDLOSE)
			{
				::BackwardsBlk();
				::BackwardsRed();
				bGenerate = false;
				goto _rm;
			}
			else if (nResult == NOFURTHER)
			{
				if (nStep >= 4 && g_bAstable[nStep - 2])
					GetAstode(g_nBSIndex[nStep - 2])->tag2 &= ~(1 << (trial[nStep - 2] - 1));
				::BackwardsBlk();
				::BackwardsRed();
				bGenerate = false;
				goto _rm;
			}
			else if (nResult == BLKLOSE)
			{
				::BackwardsBlk();
				bGenerate = false;
				goto _bm;
			}
			else if (nResult == FULLASTODE)
			{
				g_bAstable[nStep] = false;
				if (g_bFullAstode == false)
				{
					g_bFullAstode = true;
					return FULLASTODE;
				}
			}
		}
		if (g_bAddAstode && nStep < g_nFurtherEnd && nStep < nDepth)
		{
			//记录上下限
			int nBoundUpper = nDepth;
			int nLastLower = g_nBoundLower;

			int nResult = ::CallBound(nBoundUpper, away);
			if (nResult == INTERRUPT)
				return INTERRUPT;

			nDepth = nBoundUpper;
			g_nBoundLower = nLastLower;

			if (nResult == FULLASTODE)
				return FULLASTODE;

			if (nResult == BLKLOSE)
			{
				::BackwardsBlk();
				bGenerate = false;
				goto _bm;
			}
		}
	}
	return INTERRUPT;
}
