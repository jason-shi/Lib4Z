#include "StdAfx.h"
#include "RSpecial.h"

CRSpecial::CRSpecial()
{
}

CRSpecial::~CRSpecial()
{
}

void CRSpecial::CallScore(TScoreParam *pScoreParam, TScoreResult *pScoreResult)
{
	byte byValue = 0;
	word _bitmap = 0;

	//炸弹的数量：10分
	byValue += WORD_BIT_COUNT(pScoreParam->bitmap[3]) * 10;

	//三张的数量：5分
	byValue += WORD_BIT_COUNT(pScoreParam->bitmap[2]) * 5;

	//大小王的价值：7分 4分
	byValue += pScoreParam->bitmap[0] & CARD_BK ? 7 : 0;
	byValue += pScoreParam->bitmap[0] & CARD_SK ? 4 : 0;

	//2的价值：3分
	for (int i = 0; i < 4; i++)
	{
		if (pScoreParam->bitmap[i] & CARD_2)
			byValue += (i + 1) * 3;
	}
	//A的价值：2分
	for (int i = 0; i < 4; i++)
	{
		if (pScoreParam->bitmap[i] & CARD_A)
			byValue += (i + 1) * 2;
	}
	//K的价值：1分
	for (int i = 0; i < 4; i++)
	{
		if (pScoreParam->bitmap[i] & CARD_K)
			byValue += (i + 1) * 1;
	}

	//根据价值决定叫牌分数
	pScoreResult->byBidScore = 0;

	if (byValue >= 20)
		pScoreResult->byBidScore = 1;
	if (byValue >= 25)
		pScoreResult->byBidScore = 2;
	if (byValue >= 30)
		pScoreResult->byBidScore = 3;

	//必须高于最后叫牌分数
	if (pScoreResult->byBidScore < pScoreParam->byLastScore)
		pScoreResult->byBidScore = 0;
}
