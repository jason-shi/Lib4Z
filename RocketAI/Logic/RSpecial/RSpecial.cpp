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

	//ը����������10��
	byte byValue = 0;
	byValue += WORD_BIT_COUNT(bitmap[3]) * 10;

	//���ŵ�������7��
	byValue += WORD_BIT_COUNT(bitmap[2]) * 7;

	//��С���ļ�ֵ��7�� 4��
	byValue += bitmap[0] & CARD_BK ? 7 : 0;
	byValue += bitmap[0] & CARD_SK ? 4 : 0;

	//2�ļ�ֵ��3��
	for (int i = 0; i < 4; i++)
	{
		if (bitmap[i] & CARD_2)
			byValue += (i + 1) * 3;
	}
	//A�ļ�ֵ��2��
	for (int i = 0; i < 4; i++)
	{
		if (bitmap[i] & CARD_A)
			byValue += (i + 1) * 2;
	}
	//K�ļ�ֵ��1��
	for (int i = 0; i < 4; i++)
	{
		if (bitmap[i] & CARD_K)
			byValue += (i + 1) * 1;
	}

	//�������ĵ��źͶ��ӣ�3..A��
	word _bitmap = 0;
	_bitmap = bitmap[0] | bitmap[1] | bitmap[2] | bitmap[3];
	_bitmap &= MASK_SHUNZI;
	_bitmap ^= TblShunZi[_bitmap];

	_bitmap &= bitmap[0] | bitmap[1];

	//ÿ�������ſ�2�֣�ÿ�������ӿ�2��
	if (byValue >= WORD_BIT_COUNT(_bitmap) * 2)
		byValue -= WORD_BIT_COUNT(_bitmap) * 2;
	else
		byValue -= byValue;

	//���ݼ�ֵ�����Ƿ�����
	pLiGunResult->bLiGun = byValue >= 18;
}

void CRSpecial::CallJueGun(TJueGunParam *pJueGunParam, TJueGunResult *pJueGunResult)
{
	word bitmap[4];
	memcpy(bitmap, pJueGunParam->bitmap, sizeof(word [4]));

	//ը����������10��
	byte byValue = 0;
	byValue += WORD_BIT_COUNT(bitmap[3]) * 10;

	//���ŵ�������7��
	byValue += WORD_BIT_COUNT(bitmap[2]) * 7;

	//��С���ļ�ֵ��7�� 4��
	byValue += bitmap[0] & CARD_BK ? 7 : 0;
	byValue += bitmap[0] & CARD_SK ? 4 : 0;

	//2�ļ�ֵ��3��
	for (int i = 0; i < 4; i++)
	{
		if (bitmap[i] & CARD_2)
			byValue += (i + 1) * 3;
	}
	//A�ļ�ֵ��2��
	for (int i = 0; i < 4; i++)
	{
		if (bitmap[i] & CARD_A)
			byValue += (i + 1) * 2;
	}
	//K�ļ�ֵ��1��
	for (int i = 0; i < 4; i++)
	{
		if (bitmap[i] & CARD_K)
			byValue += (i + 1) * 1;
	}

	//�������ĵ��źͶ��ӣ�3..A��
	word _bitmap = 0;
	_bitmap = bitmap[0] | bitmap[1] | bitmap[2] | bitmap[3];
	_bitmap &= MASK_SHUNZI;
	_bitmap ^= TblShunZi[_bitmap];

	_bitmap &= bitmap[0] | bitmap[1];

	//ÿ�������ſ�2�֣�ÿ�������ӿ�2��
	if (byValue >= WORD_BIT_COUNT(_bitmap) * 2)
		byValue -= WORD_BIT_COUNT(_bitmap) * 2;
	else
		byValue -= byValue;

	//���ݼ�ֵ�����Ƿ���
	pJueGunResult->bJueGun = byValue >= 22;
}
