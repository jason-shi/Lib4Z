#include "StdAfx.h"
#include "RSpecial.h"

#define min(x, y)				(x) < (y) ? (x) : (y)

CRSpecial::CRSpecial()
{
}

CRSpecial::~CRSpecial()
{
}

void CRSpecial::CallScore(TScoreParam *pScoreParam, TScoreResult *pScoreResult)
{
	//常主个数
	byte byConstMajor = 0;
	for (byte bySuit = TYPE_SPADE; bySuit != TYPE_ERROR; bySuit++)
	{
		word _bitmap = pScoreParam->bitmap[bySuit - 1];
		_bitmap &= 0xF000;
		byConstMajor += WORD_BIT_COUNT(_bitmap);
	}

	//各门花色的个数
	byte bySuitCount[4] = { 0 };
	for (byte bySuit = TYPE_SPADE; bySuit != TYPE_ERROR; bySuit++)
	{
		word _bitmap = pScoreParam->bitmap[bySuit - 1];
		_bitmap &= 0x0FFF;
		bySuitCount[bySuit - 1] = WORD_BIT_COUNT(_bitmap);
	}

	//最多花色的个数
	byte byMaxSuit = 0;
	for (byte bySuit = TYPE_SPADE; bySuit != TYPE_ERROR; bySuit++)
	{
		if (bySuitCount[bySuit - 1] > byMaxSuit)
			byMaxSuit = bySuitCount[bySuit - 1];
	}

	static byte TEnlarge[7] = { 0, 1, 2, 4, 6, 7, 9, };
	byte byMajorValue = TEnlarge[byConstMajor];

	//有大王时再增加
	if (pScoreParam->bitmap[0] & 0x8000)
		byMajorValue += 1;

	//按照主牌的数量叫分
	switch (byMajorValue + byMaxSuit)
	{
	case 6:
		pScoreResult->byBidScore = 50;
		break;
	case 7:
		pScoreResult->byBidScore = 55;
		break;
	case 8:
		pScoreResult->byBidScore = 60;
		break;
	case 9:
		pScoreResult->byBidScore = 65;
		break;
	default:
		pScoreResult->byBidScore = 0;
		break;
	}

	if (byMajorValue + byMaxSuit >= 10)
		pScoreResult->byBidScore = 70;

	//必须高于最后叫牌分数
	if (pScoreResult->byBidScore < pScoreParam->byLastScore)
		pScoreResult->byBidScore = 0;
}

void CRSpecial::CallTrump(TTrumpParam *pTrumpParam, TTrumpResult *pTrumpResult)
{
	//花色价值分析
	byte bySuitVA[4] = { 0 };
	for (byte bySuit = TYPE_SPADE; bySuit != TYPE_ERROR; bySuit++)
	{
		word _bitmap = pTrumpParam->bitmap[bySuit - 1];
		_bitmap &= 0x0FFF;
		bySuitVA[bySuit - 1] = WORD_BIT_COUNT(_bitmap); //个数

		if ((_bitmap & 0x0480) == 0x0480) //20分
			bySuitVA[bySuit - 1]++;
		if ((_bitmap & 0x0C00) == 0x0C00) //AK
			bySuitVA[bySuit - 1]--;
	}
	for (byte bySuit = TYPE_SPADE; bySuit != TYPE_ERROR; bySuit++)
	{
		if ((pTrumpParam->bitmap[bySuit - 1] & 0x1000) == 0x1000) //副2->主2
			bySuitVA[bySuit - 1]++;
	}

	//选择最大价值的花色
	byte byMaxSuitVA = 0;
	byte byTrump = 0;
	for (byte bySuit = TYPE_SPADE; bySuit != TYPE_ERROR; bySuit++)
	{
		if (bySuitVA[bySuit - 1] > byMaxSuitVA)
		{
			byMaxSuitVA = bySuitVA[bySuit - 1];
			byTrump = bySuit;
		}
	}
	pTrumpResult->byTrump = byTrump;
}

void CRSpecial::CallMieDi(TMieDiParam *pMieDiParam, TMieDiResult *pMieDiResult)
{
	pMieDiResult->byFuTwo = 0;
	memset(pMieDiResult->bitmap, 0, sizeof(word [4]));

	//各门花色的个数
	byte bySuitCount[4] = { 0 };
	for (byte bySuit = TYPE_SPADE; bySuit != TYPE_ERROR; bySuit++)
	{
		word _bitmap = pMieDiParam->bitmap[bySuit - 1];
		_bitmap &= 0x0FFF;
		bySuitCount[bySuit - 1] = WORD_BIT_COUNT(_bitmap);
		if (_bitmap & 0x0800) //A
		{
			bySuitCount[bySuit - 1]--;
			if (_bitmap & 0x0400) //K
			{
				bySuitCount[bySuit - 1]--;
				if (_bitmap & 0x0300) //Q or J
					bySuitCount[bySuit - 1] = 0;
			}
		}
	}

	//副牌里最短的灭掉
	byte byRemainCount = 6;
	for (byte byCardCount = 1; byCardCount <= 3; byCardCount++)
	{
		if (byRemainCount == 0)
			break;
		for (byte bySuit = TYPE_HEART; bySuit != TYPE_ERROR; bySuit++)
		{
			word _bitmap = pMieDiParam->bitmap[bySuit - 1];

			if (bySuitCount[bySuit - 1] <= byCardCount && bySuitCount[bySuit - 1] != 0)
			{
				byte byCount = min(byRemainCount, bySuitCount[bySuit - 1]);
				if (byCount == 0)
					continue;

				//从最小的牌开始灭底
				for (byte i = 0; i < byCount; i++)
				{
					pMieDiResult->bitmap[bySuit - 1] |= _bitmap & -_bitmap;
					_bitmap &= _bitmap - 1;
				}
				byRemainCount -= byCount;
				bySuitCount[bySuit - 1] -= byCount;
				pMieDiParam->bitmap[bySuit - 1] = _bitmap; //回写
			}
		}
	}

	//副牌里有分的灭掉
	if (byRemainCount == 0)
		return;
	for (byte byCardCount = 4; byCardCount <= 12; byCardCount++)
	{
		if (byRemainCount == 0)
			break;
		for (byte bySuit = TYPE_HEART; bySuit != TYPE_ERROR; bySuit++)
		{
			word _bitmap = pMieDiParam->bitmap[bySuit - 1];

			if (bySuitCount[bySuit - 1] <= byCardCount && bySuitCount[bySuit - 1] != 0)
			{
				if (_bitmap & MASK_T)
				{
					pMieDiResult->bitmap[bySuit - 1] |= MASK_T;
					_bitmap ^= MASK_T;
					byRemainCount--;
					bySuitCount[bySuit - 1]--;
					pMieDiParam->bitmap[bySuit - 1] = _bitmap; //回写
				}
				if (byRemainCount == 0)
					break;
				if (_bitmap & MASK_5)
				{
					pMieDiResult->bitmap[bySuit - 1] |= MASK_5;
					_bitmap ^= MASK_5;
					byRemainCount--;
					bySuitCount[bySuit - 1]--;
					pMieDiParam->bitmap[bySuit - 1] = _bitmap; //回写
				}
				if (byRemainCount == 0)
					break;
			}
		}
	}

	//副牌里最短的灭掉
	if (byRemainCount == 0)
		return;
	for (byte byCardCount = 1; byCardCount <= 12; byCardCount++)
	{
		if (byRemainCount == 0)
			break;
		for (byte bySuit = TYPE_HEART; bySuit != TYPE_ERROR; bySuit++)
		{
			word _bitmap = pMieDiParam->bitmap[bySuit - 1];

			if (bySuitCount[bySuit - 1] <= byCardCount && bySuitCount[bySuit - 1] != 0)
			{
				byte byCount = min(byRemainCount, bySuitCount[bySuit - 1]);
				if (byCount == 0)
					continue;

				//从最小的牌开始灭底
				for (byte i = 0; i < byCount; i++)
				{
					pMieDiResult->bitmap[bySuit - 1] |= _bitmap & -_bitmap;
					_bitmap &= _bitmap - 1;
				}
				byRemainCount -= byCount;
				bySuitCount[bySuit - 1] -= byCount;
				pMieDiParam->bitmap[bySuit - 1] = _bitmap; //回写
			}
		}
	}

	//保证灭底的张数：副牌
	if (byRemainCount == 0)
		return;
	for (byte bySuit = TYPE_HEART; bySuit != TYPE_ERROR; bySuit++)
	{
		word _bitmap = pMieDiParam->bitmap[bySuit - 1];
		_bitmap &= 0x0FFF;
		byte byCount = min(byRemainCount, WORD_BIT_COUNT(_bitmap));
		if (byCount == 0)
			continue;

		//从最小的牌开始灭底
		for (byte i = 0; i < byCount; i++)
		{
			pMieDiResult->bitmap[bySuit - 1] |= _bitmap & -_bitmap;
			_bitmap &= _bitmap - 1;
		}
		byRemainCount -= byCount;
		bySuitCount[bySuit - 1] -= byCount;
		pMieDiParam->bitmap[bySuit - 1] = _bitmap; //回写
	}

	//保证灭底的张数：主牌
	if (byRemainCount == 0)
		return;
	for (byte bySuit = TYPE_SPADE; bySuit != TYPE_HEART; bySuit++)
	{
		word _bitmap = pMieDiParam->bitmap[bySuit - 1];
		_bitmap &= 0x0FFF;
		byte byCount = min(byRemainCount, WORD_BIT_COUNT(_bitmap));
		if (byCount == 0)
			continue;

		//从最小的牌开始灭底
		for (byte i = 0; i < byCount; i++)
		{
			pMieDiResult->bitmap[bySuit - 1] |= _bitmap & -_bitmap;
			_bitmap &= _bitmap - 1;
		}
		byRemainCount -= byCount;
		bySuitCount[bySuit - 1] -= byCount;
		pMieDiParam->bitmap[bySuit - 1] = _bitmap; //回写
	}
}
