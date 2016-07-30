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

	//ը����������10��
	byValue += WORD_BIT_COUNT(pScoreParam->bitmap[3]) * 10;

	//���ŵ�������5��
	byValue += WORD_BIT_COUNT(pScoreParam->bitmap[2]) * 5;

	//��С���ļ�ֵ��7�� 4��
	byValue += pScoreParam->bitmap[0] & CARD_BK ? 7 : 0;
	byValue += pScoreParam->bitmap[0] & CARD_SK ? 4 : 0;

	//2�ļ�ֵ��3��
	for (int i = 0; i < 4; i++)
	{
		if (pScoreParam->bitmap[i] & CARD_2)
			byValue += (i + 1) * 3;
	}
	//A�ļ�ֵ��2��
	for (int i = 0; i < 4; i++)
	{
		if (pScoreParam->bitmap[i] & CARD_A)
			byValue += (i + 1) * 2;
	}
	//K�ļ�ֵ��1��
	for (int i = 0; i < 4; i++)
	{
		if (pScoreParam->bitmap[i] & CARD_K)
			byValue += (i + 1) * 1;
	}

	//���ݼ�ֵ�������Ʒ���
	pScoreResult->byBidScore = 0;

	if (byValue >= 20)
		pScoreResult->byBidScore = 1;
	if (byValue >= 25)
		pScoreResult->byBidScore = 2;
	if (byValue >= 30)
		pScoreResult->byBidScore = 3;

	//������������Ʒ���
	if (pScoreResult->byBidScore < pScoreParam->byLastScore)
		pScoreResult->byBidScore = 0;
}
