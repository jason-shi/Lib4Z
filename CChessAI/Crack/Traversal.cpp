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

	board[dest] = board[src];
	board[src] = 0;
	g_hPBoard[dest] = g_hPBoard[src];
	g_hPBoard[src] = NULL;
	g_hPBoard[dest]->pos = dest;

	horz[src >> 4] &= ~(1 << (src & 0xF));
	vert[src & 0xF] &= ~(1 << (src >> 4));

	horz[dest >> 4] |= 1 << (dest & 0xF);
	vert[dest & 0xF] |= 1 << (dest >> 4);

	UpdateKingFamily();

	trial[nStep++]++;
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

	board[dest] = board[src];
	board[src] = 0;
	g_hPBoard[dest] = g_hPBoard[src];
	g_hPBoard[src] = NULL;
	g_hPBoard[dest]->pos = dest;

	horz[src >> 4] &= ~(1 << (src & 0xF));
	vert[src & 0xF] &= ~(1 << (src >> 4));

	horz[dest >> 4] |= 1 << (dest & 0xF);
	vert[dest & 0xF] |= 1 << (dest >> 4);

	UpdateKingFamily();

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

	board[src] = board[dest];
	board[dest] = pMove->pDeath ? pMove->pDeath->kind : 0;
	g_hPBoard[src] = g_hPBoard[dest];
	g_hPBoard[dest] = pMove->pDeath;
	g_hPBoard[src]->pos = src;

	if (board[dest] == 0)
	{
		horz[dest >> 4] &= ~(1 << (dest & 0xF));
		vert[dest & 0xF] &= ~(1 << (dest >> 4));
	}

	horz[src >> 4] |= 1 << (src & 0xF);
	vert[src & 0xF] |= 1 << (src >> 4);

	UpdateKingFamily();
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

	board[src] = board[dest];
	board[dest] = pMove->pDeath ? pMove->pDeath->kind : 0;
	g_hPBoard[src] = g_hPBoard[dest];
	g_hPBoard[dest] = pMove->pDeath;
	g_hPBoard[src]->pos = src;

	if (board[dest] == 0)
	{
		horz[dest >> 4] &= ~(1 << (dest & 0xF));
		vert[dest & 0xF] &= ~(1 << (dest >> 4));
	}

	horz[src >> 4] |= 1 << (src & 0xF);
	vert[src & 0xF] |= 1 << (src >> 4);

	UpdateKingFamily();
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

static int BlackMove(bool bGenerate)
{
	if (bGenerate)
		::GenerateBlk(MoveList[nStep - 1][trial[nStep - 1] - 1]->check);

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

static int CallBound(int nBoundUpper)
{
	g_nBoundLower = nStep;
	nDepth = nStep + 2;

	while (nDepth <= nBoundUpper)
	{
		int nResult = Traversal();
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

int Traversal()
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
_bm:	if (::BlackMove(bGenerate) == BLKLOSE)
			return BLKLOSE;

		g_bAstable[nStep] = false;
		int check = MoveList[nStep - 2][trial[nStep - 2] - 1]->check;
		int dest = MoveList[nStep - 1][trial[nStep - 1] - 1]->dest;
		if (nStep < g_nFurtherEnd
			|| board[check] == R_CANNON && nStep < nDepth - 4
				&& TblSameLine[dest ^ check] && TblSameLine[dest ^ kpos])
		{
			g_bAstable[nStep] = true;
			ShrinkAstodeRed(tAstode.data);
			ShrinkAstodeBlk(tAstode.data);
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

			int nResult = ::CallBound(nBoundUpper);
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
