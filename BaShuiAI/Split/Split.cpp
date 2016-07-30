#include "StdAfx.h"
#include "Split.h"

using namespace std;

CSplit::CSplit()
{
}

CSplit::~CSplit()
{
}

dword CSplit::ForceSplit(byte byCard[8], word wFlushBitmap)
{
	//特殊牌型
	dword v = SplitSpecType(byCard, wFlushBitmap);
	if (v != 0)
		return v;

	//普通牌型
	return SplitCards(byCard, wFlushBitmap);
}

void CSplit::PushPile(VECPILE &vecPile, byte byType, byte byGrade)
{
	int nVA, nAttachVA = 0;
	switch (byType)
	{
	case INNER_TYPE_SISTER:
		nVA = V_SISTER;
		break;
	case INNER_TYPE_A2:
		nVA = V_A2;
		break;
	case INNER_TYPE_KA:
		nVA = V_KA;
		break;
	case INNER_TYPE_PAIR:
		nVA = V_PAIR;
		break;
	case INNER_TYPE_SHUNZI:
		nVA = V_SHUNZI;
		break;
	case INNER_TYPE_A23:
		nVA = V_A23;
		break;
	case INNER_TYPE_QKA:
		nVA = V_QKA;
		break;
	case INNER_TYPE_BOMB:
		nVA = V_BOMB;
		break;
	case INNER_TYPE_S_SHUNZI:
		nVA = V_S_SHUNZI;
		break;
	case INNER_TYPE_S_A23:
		nVA = V_S_A23;
		break;
	case INNER_TYPE_S_QKA:
		nVA = V_S_QKA;
		break;
	}

	//第一堆牌是对子的附加值
	if (vecPile.size() == 1)
	{
		if (vecPile[0].byType == INNER_TYPE_S_SHUNZI)
			nAttachVA = V_ATTACH_2ND_S_SHUNZI;
		else if (vecPile[0].byType == INNER_TYPE_BOMB)
			nAttachVA = V_ATTACH_2ND_BOMB;
	}
	else if (vecPile.size() == 2)
	{
		if (vecPile[0].byType == INNER_TYPE_PAIR)
			nAttachVA = V_ATTACH_1ST_PAIR;
		if (vecPile[1].byType == INNER_TYPE_S_SHUNZI)
			nAttachVA = V_ATTACH_2ND_S_SHUNZI;
		else if (vecPile[1].byType == INNER_TYPE_BOMB)
			nAttachVA = V_ATTACH_2ND_BOMB;
	}

	VSum += nVA;
	VSum += nAttachVA;
	vecPile.push_back(TPile(nVA, nAttachVA, byType, byGrade));
}

void CSplit::PopPile(VECPILE &vecPile, byte &byType, byte &byGrade)
{
	TPile &tSplit = vecPile.back();
	VSum -= tSplit.nVA;
	VSum -= tSplit.nAttachVA;
	byType = tSplit.byType;
	byGrade = tSplit.byGrade;
	vecPile.pop_back();

	InCards(byType, byGrade);
}

byte CSplit::GetMaxSingle()
{
	word _bitmap = bitmap[0] | bitmap[1] | bitmap[2] | bitmap[3];
	byte byGrade = _bitmap & 0xFF00 ? TblMaxGrade[_bitmap >> 8] + 8
									: TblMaxGrade[_bitmap];
	_OutSingle(1 << byGrade); //移除单张

	return byGrade;
}

void CSplit::PlaceCards_1(const VECPILE &vecPile, byte byPlacedCard[8])
{
	int nPileSingle = 0;
	for (int i = 0; i < (int)vecPile.size(); i++)
	{
		if (vecPile[i].byType == INNER_TYPE_SINGLE)
			nPileSingle++;
	}

	switch (nPileSingle)
	{
	case 3:
		byPlacedCard[7] = GetMaxSingle();
		byPlacedCard[4] = GetMaxSingle();
		byPlacedCard[1] = GetMaxSingle();
		byPlacedCard[0] = GetMaxSingle();
		byPlacedCard[2] = GetMaxSingle();
		byPlacedCard[3] = GetMaxSingle();
		byPlacedCard[5] = GetMaxSingle();
		byPlacedCard[6] = GetMaxSingle();
		break;
	case 2:
		byPlacedCard[4] = GetMaxSingle();
		byPlacedCard[1] = GetMaxSingle();
		byPlacedCard[0] = GetMaxSingle();
		byPlacedCard[2] = GetMaxSingle();
		byPlacedCard[3] = GetMaxSingle();
		if (vecPile[2].byType == INNER_TYPE_PAIR)
			byPlacedCard[7] = GetMaxSingle();
		break;
	case 1: //坠落
		byPlacedCard[1] = GetMaxSingle();
		byPlacedCard[0] = GetMaxSingle();
	case 0:
		if (vecPile[1].byType == INNER_TYPE_PAIR)
			byPlacedCard[4] = GetMaxSingle();
		if (vecPile[2].byType == INNER_TYPE_PAIR)
			byPlacedCard[7] = GetMaxSingle();
		break;
	}
}

void CSplit::PlaceCards_2(const VECPILE &vecPile, byte byPlacedCard[8])
{
	int nIndex = 0;
	for (int i = 0; i < 3; i++)
	{
		switch (vecPile[i].byType)
		{
		case INNER_TYPE_SISTER:
			byPlacedCard[nIndex] = vecPile[i].byGrade;
			byPlacedCard[nIndex + 1] = vecPile[i].byGrade + 1;
			break;
		case INNER_TYPE_A2:
			byPlacedCard[nIndex] = 12;
			byPlacedCard[nIndex + 1] = 0;
			break;
		case INNER_TYPE_KA:
			byPlacedCard[nIndex] = 11;
			byPlacedCard[nIndex + 1] = 12;
			break;
		case INNER_TYPE_PAIR:
			byPlacedCard[nIndex] = vecPile[i].byGrade;
			byPlacedCard[nIndex + 1] = vecPile[i].byGrade;
			break;
		case INNER_TYPE_SHUNZI:
		case INNER_TYPE_S_SHUNZI:
			byPlacedCard[nIndex] = vecPile[i].byGrade;
			byPlacedCard[nIndex + 1] = vecPile[i].byGrade + 1;
			byPlacedCard[nIndex + 2] = vecPile[i].byGrade + 2;
			break;
		case INNER_TYPE_A23:
		case INNER_TYPE_S_A23:
			byPlacedCard[nIndex] = 12;
			byPlacedCard[nIndex + 1] = 0;
			byPlacedCard[nIndex + 2] = 1;
			break;
		case INNER_TYPE_QKA:
		case INNER_TYPE_S_QKA:
			byPlacedCard[nIndex] = 10;
			byPlacedCard[nIndex + 1] = 11;
			byPlacedCard[nIndex + 2] = 12;
			break;
		case INNER_TYPE_BOMB:
			byPlacedCard[nIndex] = vecPile[i].byGrade;
			byPlacedCard[nIndex + 1] = vecPile[i].byGrade;
			byPlacedCard[nIndex + 2] = vecPile[i].byGrade;
			break;
		}
		nIndex += i == 0 ? 2 : 3;
	}
}

void CSplit::TransToBitmap(byte byCard[8])
{
	memset(bitmap, 0, sizeof(word [4]));
	byte byCount = 0;
	for (int i = 0; i < 8; i += byCount)
	{
		//计算具有相同牌点的牌数
		byCount = 1;
		for (int j = i + 1; j < 8; j++)
		{
			if (byCard[i] != byCard[j])
				break;
			byCount++;
		}
		bitmap[byCount - 1] |= 1 << byCard[i];
	}
}

dword CSplit::GetCardBitmap(byte byCard[8], byte byPlacedCard[8])
{
	bool bHasPlaced[8];
	memset (bHasPlaced, 0, sizeof(bHasPlaced));

	dword dwResult = 0;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (bHasPlaced[j])
				continue;

			if (byCard[j] == byPlacedCard[i])
			{
				//标记卡牌已经使用
				bHasPlaced[j] = true;

				dwResult |= j << (i * 3);
				break;
			}
		}
	}
	return dwResult;
}

dword CSplit::GetTypeBitmap(VECPILE &vecPile)
{
	byte dwType = 0;

	if (vecPile[0].byType == INNER_TYPE_PAIR)
		dwType |= 1 << 0;

	switch (vecPile[1].byType)
	{
	case INNER_TYPE_SINGLE:
		dwType |= TYPE_SINGLE << 1;
		break;
	case INNER_TYPE_PAIR:
		dwType |= TYPE_PAIR << 1;
		break;
	case INNER_TYPE_SHUNZI:
	case INNER_TYPE_A23:
	case INNER_TYPE_QKA:
		dwType |= TYPE_SHUNZI << 1;
		break;
	case INNER_TYPE_BOMB:
		dwType |= TYPE_BOMB << 1;
		break;
	case INNER_TYPE_S_SHUNZI:
	case INNER_TYPE_S_A23:
	case INNER_TYPE_S_QKA:
		dwType |= TYPE_S_SHUNZI << 1;
		break;
	}
	switch (vecPile[2].byType)
	{
	case INNER_TYPE_SINGLE:
		dwType |= TYPE_SINGLE << 4;
		break;
	case INNER_TYPE_PAIR:
		dwType |= TYPE_PAIR << 4;
		break;
	case INNER_TYPE_SHUNZI:
	case INNER_TYPE_A23:
	case INNER_TYPE_QKA:
		dwType |= TYPE_SHUNZI << 4;
		break;
	case INNER_TYPE_BOMB:
		dwType |= TYPE_BOMB << 4;
		break;
	case INNER_TYPE_S_SHUNZI:
	case INNER_TYPE_S_A23:
	case INNER_TYPE_S_QKA:
		dwType |= TYPE_S_SHUNZI << 4;
		break;
	}
	return dwType;
}

dword CSplit::SplitCards(byte byCard[8], word wFlushBitmap)
{
	VECPILE vecPileMax;

	VSum = 0;
	VSumMax = 0;
	m_vecPile.clear();

	//转换成位图
	TransToBitmap(byCard);
	m_wFlushBitmap = wFlushBitmap;

	byte byType = INNER_TYPE_GVP;
	byte byGrade = GRADE_INIT;
	while (true)
	{
		//尝试拆牌
		if (m_vecPile.size() < 3)
		{
			byte byLastType = byType;
			byte byLastGrade = byGrade;
			if ((byType = SplitlPile(byType, byGrade)) != INNER_TYPE_GVP)
			{
				PushPile(m_vecPile, byType, byGrade);
				if (VSum > VSumMax)
				{
					VSumMax = VSum;
					vecPileMax = m_vecPile;
				}

				//重新从这个等级拆牌
				byType = byLastType;
				byGrade = byLastGrade;
				continue;
			}
		}
		if (m_vecPile.empty())
			break;
		PopPile(m_vecPile, byType, byGrade);
	}

	//打出拆分的牌
	for (int i = 0; i < (int)vecPileMax.size(); i++)
		OutCards(vecPileMax[i].byType, vecPileMax[i].byGrade);

	//补充成为三堆
	while (vecPileMax.size() < 3)
		vecPileMax.insert(vecPileMax.begin(), TPile(0, 0, INNER_TYPE_SINGLE, 0));

	//先摆单张
	byte byPlacedCard[8];
	PlaceCards_1(vecPileMax, byPlacedCard);
	PlaceCards_2(vecPileMax, byPlacedCard);

	dword dwTypeBitmap = GetTypeBitmap(vecPileMax);
	dword dwCardBitmap = GetCardBitmap(byCard, byPlacedCard);

	//类型和卡牌位图
	return (dwTypeBitmap << 24) | dwCardBitmap;
}

byte CSplit::GetSpecType_TriShunZi(VECPILE &vecPile)
{
	switch (vecPile[1].byType)
	{
	case INNER_TYPE_S_SHUNZI:
	case INNER_TYPE_S_A23:
	case INNER_TYPE_S_QKA:
		return TYPE_TRI_SHUNZI_2S;
	}
	switch (vecPile[2].byType)
	{
	case INNER_TYPE_S_SHUNZI:
	case INNER_TYPE_S_A23:
	case INNER_TYPE_S_QKA:
		return TYPE_TRI_SHUNZI_1S;
	}
	return TYPE_TRI_SHUNZI;
}

dword CSplit::SplitCards_TriShunZi(byte byCard[8], word wFlushBitmap, VECPILE &vecPile)
{
	VECPILE vecPileMax;

	VSum = 0;
	VSumMax = 0;
	m_vecPile.clear();

	//转换成位图
	TransToBitmap(byCard);
	m_wFlushBitmap = wFlushBitmap;

	byte byType = INNER_TYPE_GVP;
	byte byGrade = GRADE_INIT;
	while (true)
	{
		if (m_vecPile.size() == 0)
		{
			byType = SplitlPile_Sister(byType, byGrade);
			if (byType == INNER_TYPE_GVP)
				break;

			//压入第一堆姐妹牌
			PushPile(m_vecPile, byType, byGrade);

			//第二堆从顺子开始拆牌
			byType = INNER_TYPE_SHUNZI;
			byGrade = GRADE_INIT;
			continue;
		}
		else
		{
			if (m_vecPile.size() < 3)
			{
				//尝试拆牌
				byte byLastType = byType;
				byte byLastGrade = byGrade;

				byType = SplitlPile_ShunZi(byType, byGrade);
				if (byType != INNER_TYPE_GVP)
				{
					PushPile(m_vecPile, byType, byGrade);
					if (VSum > VSumMax)
					{
						VSumMax = VSum;
						vecPileMax = m_vecPile;
					}

					//重新从这个等级拆牌
					byType = byLastType;
					byGrade = byLastGrade;
					continue;
				}
			}
		}
		if (m_vecPile.empty())
			break;
		PopPile(m_vecPile, byType, byGrade);
	}

	//必须是三堆牌
	if (vecPileMax.size() != 3)
		return 0;

	//摆三堆牌
	byte byPlacedCard[8];
	PlaceCards_2(vecPileMax, byPlacedCard);

	//拆牌结果
	vecPile = vecPileMax;

	//卡牌索引位图
	return GetCardBitmap(byCard, byPlacedCard);
}
