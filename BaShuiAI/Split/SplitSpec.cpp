#include "StdAfx.h"
#include "Split.h"

dword CSplit::SplitSpecType(byte byCard[8], word wFlushBitmap)
{
	dword v = 0;

	v = _SplitDblBomb(byCard, wFlushBitmap);
	if (v != 0) return v;

	v = _SplitOneBomb(byCard, wFlushBitmap);
	if (v != 0) return v;

	v = _SplitFourPair(byCard, wFlushBitmap);
	if (v != 0) return v;

	v = _SplitOneShunZi(byCard, wFlushBitmap);
	if (v != 0) return v;

	v = _SplitTriShunZi(byCard, wFlushBitmap);
	if (v != 0) return v;

	return 0;
}

dword CSplit::_SplitDblBomb(byte byCard[8], word wFlushBitmap)
{
	for (int i = 1; i < 4; i++)
	{
		if ((byCard[i] & 0x0F) != (byCard[0] & 0x0F))
			return 0;
	}
	for (int i = 5; i < 8; i++)
	{
		if ((byCard[i] & 0x0F) != (byCard[4] & 0x0F))
			return 0;
	}
	dword v = 076543210;
	return v | (1 << 31) | (TYPE_DBL_BOMB << 24);
}

dword CSplit::_SplitOneBomb(byte byCard[8], word wFlushBitmap)
{
	byte byBomb = 0xFF;
	for (int i = 0; i < 5; i++)
	{
		if ((byCard[i + 1] & 0x0F) == (byCard[i] & 0x0F)
			&& (byCard[i + 2] & 0x0F) == (byCard[i] & 0x0F)
			&& (byCard[i + 3] & 0x0F) == (byCard[i] & 0x0F))
		{
			byBomb = i;
			break;
		}
	}
	if (byBomb == 0xFF)
		return 0;

	//将炸弹摆在最前面
	dword v = byBomb | (byBomb + 1) << 3 | (byBomb + 2) << 6 | (byBomb + 3) << 9;
	for (int i = 0; i < byBomb; i++)
		v |= i << (12 + 3 * i);
	for (int i = byBomb + 4; i < 8; i++)
		v |= i << (3 * i);
	return v | (1 << 31) | (TYPE_ONE_BOMB << 24);
}

dword CSplit::_SplitFourPair(byte byCard[8], word wFlushBitmap)
{
	for (int i = 0; i < 8; i += 2)
	{
		if ((byCard[i] & 0x0F) != (byCard[i + 1] & 0x0F))
			return 0;
	}
	dword v = 076543210;
	return v | (1 << 31) | (TYPE_FOUR_PAIR << 24);
}

dword CSplit::_SplitOneShunZi(byte byCard[8], word wFlushBitmap)
{
	//前七张牌必须连续
	for (int i = 0; i < 7; i++)
	{
		if ((byCard[i + 1] & 0x0F) != (byCard[i] & 0x0F) + 1)
			return 0;
	}

	//一把顺子的两种不同的类型
	bool bOneShunZi = false;
	dword v;
	if ((byCard[7] & 0x0F) == (byCard[6] & 0x0F) + 1)
	{
		v = 076543210;
		bOneShunZi = true;
	}
	if ((byCard[7] & 0x0F) == (byCard[0] & 0x0F) + 12)
	{
		v = 065432107;
		bOneShunZi = true;
	}
	if (!bOneShunZi)
		return 0;

	//判断是否一把同花顺
	byte bySpecType = WORD_BIT_COUNT(wFlushBitmap) == 8 ? TYPE_S1_SHUNZI
														: TYPE_ONE_SHUNZI;
	return v | (1 << 31) | (bySpecType << 24);
}

dword CSplit::_SplitTriShunZi(byte byCard[8], word wFlushBitmap)
{
	word _bitmap = 0;
	for (int i = 0; i < 8; i++)
		_bitmap |= 1 << (byCard[i] & 0x0F);

	//剔除构成顺子、A2、KA的标记
	word wA23 = _bitmap & MASK_A23;
	if (wA23 != MASK_A23)
		wA23 = 0;
	word wQKA = _bitmap & MASK_QKA;
	if (wQKA != MASK_QKA)
		wQKA = 0;
	word wA2 = _bitmap & MASK_A2;
	if (wA2 != MASK_A2)
		wA2 = 0;
	word wKA = _bitmap & MASK_KA;
	if (wKA != MASK_KA)
		wKA = 0;
	_bitmap ^= TblSanShun[_bitmap & MASK_SHUNZI] | wA23 | wQKA | wA2 | wKA;

	if (WORD_BIT_COUNT(_bitmap) >= 3) //例：346，3478，357
		return 0;

	while (_bitmap)
	{
		if ((_bitmap & TblSister[_bitmap & MASK_SHUNZI]) == 0)
			return 0;
		_bitmap ^= TblSister[_bitmap];
	}

	VECPILE vecPile;
	dword v = SplitCards_TriShunZi(byCard, wFlushBitmap, vecPile);
	if (v == 0)
		return 0;

	//三把同花顺
	if (WORD_BIT_COUNT(wFlushBitmap) == 8)
		return v | (1 << 31) | (TYPE_S3_SHUNZI << 24);

	//特殊牌型三顺子：含有一个同花顺，两个同花顺
	byte bySpecType = GetSpecType_TriShunZi(vecPile);
	return v | (1 << 31) | (bySpecType << 24);
}
