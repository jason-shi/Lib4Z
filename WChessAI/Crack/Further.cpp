#include "StdAfx.h"
#include "Traversal.h"
#include "Further.h"
#include "MoveList\MoveList.h"
#include "Generate\Generate.h"
#include "Astode\Astode.h"

static int RedMove_F(bool bGenerate)
{
	if (bGenerate)
		::GenerateRed(true);

_a:	while (trial[nStep] < Trial[nStep] && !MoveList[nStep][trial[nStep]]->bValid)
		trial[nStep]++;
	if (trial[nStep] == Trial[nStep])
	{
		if (nStep == 0)
			return NOFURTHER;
		::BackwardsBlk();
		::BackwardsRed();
		goto _a;
	}
	::ForwardsRed();
	if (::InCheckRed())
	{
		::BackwardsRed();
		goto _a;
	}
	return NOLOSE;
}

static int BlackMove_F(bool bGenerate, int away)
{
	if (bGenerate)
		::GenerateBlk(MoveList[nStep - 1][trial[nStep - 1] - 1]->check, away);

_a:	if (trial[nStep] == Trial[nStep])
	{
		if (nStep == 1)
			return FURTHER;
		::BackwardsRed();
		::BackwardsBlk();
		goto _a;
	}
	::ForwardsBlk();
	if (::InCheckBlk())
	{
		::BackwardsBlk();
		goto _a;
	}
	return NOLOSE;
}

static int Traversal_F(int away)
{
	nStep = 0; //归零
	while (true)
	{
		bool bGenerate = nStep < g_nFurther;
		if (!bGenerate)
		{
			::GenerateRed(true);
			if (Trial[nStep] != 0)
			{
				::BackwardsBlk();
				goto _bm;
			}
			::BackwardsBlk();
			::BackwardsRed();
		}
		//到达思考最大深度导致的回退，此时红方已经生成过着法
		if (::RedMove_F(bGenerate) == NOFURTHER)
			return NOFURTHER;
		bGenerate = true;
_bm:	if (::BlackMove_F(bGenerate, away) == FURTHER)
			return FURTHER;
	}
}

static bool HasRQueen()
{
	for (int i = 0; i < 64; i++)
	{
		if (board[i] == R_QUEEN)
			return true;
	}
	return false;
}

int CallFurther(int nFurther, int away)
{
	away = 8;

	g_nFurther = nFurther;
	if (g_nFurther == 0)
		g_nFurther = DefaultFurther;

	TAstode *pAstode;
	for (int i = 0; i < g_nStoreSize; i++)
	{
		pAstode = AstodeStore[i];
		if (pAstode->tag1 == NOANSWER && pAstode->nWinD == MaxCrackDepth)
		{
			::ExpandAstode(pAstode->data);
			if (!::HasRQueen())
			{
				::GetBoardDynData();
				pAstode->tag1 = ::Traversal_F(away);
			}
		}
		if (g_bInterrupt)
			return INTERRUPT;
	}
	for (int i = 0; i < g_nBufferSize; i++)
	{
		pAstode = AstodeBuffer[i];
		if (pAstode->tag1 == NOANSWER && pAstode->nWinD == MaxCrackDepth)
		{
			::ExpandAstode(pAstode->data);
			if (!::HasRQueen())
			{
				::GetBoardDynData();
				pAstode->tag1 = ::Traversal_F(away);
			}
		}
		if (g_bInterrupt)
			return INTERRUPT;
	}
	return NOLOSE;
}
