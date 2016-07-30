#include "StdAfx.h"
#include "RSpecial.h"

CRSpecial::CRSpecial()
{
}

CRSpecial::~CRSpecial()
{
}

void CRSpecial::CallLiGun(TLiGunParam *pLiGunParam, TLiGunResult *pLiGunResult)
{
	word bitmap[4];
	memcpy(bitmap, pLiGunParam->bitmap, sizeof(word [4]));

	//炸弹的数量：10分
	byte byValue = 0;
	byValue += WORD_BIT_COUNT(bitmap[3]) * 10;

	//三张的数量：7分
	byValue += WORD_BIT_COUNT(bitmap[2]) * 7;

	//大小王的价值：7分 4分
	byValue += bitmap[0] & CARD_BK ? 7 : 0;
	byValue += bitmap[0] & CARD_SK ? 4 : 0;

	//2的价值：3分
	for (int i = 0; i < 4; i++)
	{
		if (bitmap[i] & CARD_2)
			byValue += (i + 1) * 3;
	}
	//A的价值：2分
	for (int i = 0; i < 4; i++)
	{
		if (bitmap[i] & CARD_A)
			byValue += (i + 1) * 2;
	}
	//K的价值：1分
	for (int i = 0; i < 4; i++)
	{
		if (bitmap[i] & CARD_K)
			byValue += (i + 1) * 1;
	}

	//不连续的单张和对子（3..A）
	word _bitmap = 0;
	_bitmap = bitmap[0] | bitmap[1] | bitmap[2] | bitmap[3];
	_bitmap &= MASK_SHUNZI;
	_bitmap ^= TblShunZi[_bitmap];

	_bitmap &= bitmap[0] | bitmap[1];

	//每独立单张扣2分，每独立对子扣2分
	if (byValue >= WORD_BIT_COUNT(_bitmap) * 2)
		byValue -= WORD_BIT_COUNT(_bitmap) * 2;
	else
		byValue -= byValue;

	//根据价值决定是否立棍
	pLiGunResult->bLiGun = byValue >= 18;
}

void CRSpecial::CallJueGun(TJueGunParam *pJueGunParam, TJueGunResult *pJueGunResult)
{
	word bitmap[4];
	memcpy(bitmap, pJueGunParam->bitmap, sizeof(word [4]));

	//炸弹的数量：10分
	byte byValue = 0;
	byValue += WORD_BIT_COUNT(bitmap[3]) * 10;

	//三张的数量：7分
	byValue += WORD_BIT_COUNT(bitmap[2]) * 7;

	//大小王的价值：7分 4分
	byValue += bitmap[0] & CARD_BK ? 7 : 0;
	byValue += bitmap[0] & CARD_SK ? 4 : 0;

	//2的价值：3分
	for (int i = 0; i < 4; i++)
	{
		if (bitmap[i] & CARD_2)
			byValue += (i + 1) * 3;
	}
	//A的价值：2分
	for (int i = 0; i < 4; i++)
	{
		if (bitmap[i] & CARD_A)
			byValue += (i + 1) * 2;
	}
	//K的价值：1分
	for (int i = 0; i < 4; i++)
	{
		if (bitmap[i] & CARD_K)
			byValue += (i + 1) * 1;
	}

	//不连续的单张和对子（3..A）
	word _bitmap = 0;
	_bitmap = bitmap[0] | bitmap[1] | bitmap[2] | bitmap[3];
	_bitmap &= MASK_SHUNZI;
	_bitmap ^= TblShunZi[_bitmap];

	_bitmap &= bitmap[0] | bitmap[1];

	//每独立单张扣2分，每独立对子扣2分
	if (byValue >= WORD_BIT_COUNT(_bitmap) * 2)
		byValue -= WORD_BIT_COUNT(_bitmap) * 2;
	else
		byValue -= byValue;

	//根据价值决定是否撅棍
	pJueGunResult->bJueGun = byValue >= 22;
}
