#include "StdAfx.h"
#include "CoSplit.h"

using namespace std;

CCoSplit::CCoSplit() : CSplit()
{
}

CCoSplit::~CCoSplit()
{
}

dword CCoSplit::ForceSplit(dword &dwLimitScore, byte byCard[8], word wFlushBitmap)
{
	//��������
	dword v = SplitSpecType(byCard, wFlushBitmap);
	if (v != 0)
		return v;

	if (dwLimitScore == 0)
		return 0;

	//��ͨ����
	return SplitCards(dwLimitScore, byCard, wFlushBitmap);
}

void CCoSplit::TransToOuterType(byte byType, byte &byOuterType)
{
	switch (byType)
	{
	case INNER_TYPE_SINGLE:
		byOuterType = TYPE_SINGLE;
		break;
	case INNER_TYPE_PAIR:
		byOuterType = TYPE_PAIR;
		break;
	case INNER_TYPE_SHUNZI:
	case INNER_TYPE_A23:
	case INNER_TYPE_QKA:
		byOuterType = TYPE_SHUNZI;
		break;
	case INNER_TYPE_BOMB:
		byOuterType = TYPE_BOMB;
		break;
	case INNER_TYPE_S_SHUNZI:
	case INNER_TYPE_S_A23:
	case INNER_TYPE_S_QKA:
		byOuterType = TYPE_S_SHUNZI;
		break;
	}
}

bool CCoSplit::IsTypeMatch(VECBYTE &vecOuterType, byte byPlacedCard[8])
{
	dword dwIndex[3];
	dwIndex[0] = TblFastCompareNT[(byPlacedCard[0] << 8) + (byPlacedCard[1] << 4) + 0x0F];
	dwIndex[1] = TblFastCompareNT[(byPlacedCard[2] << 8) + (byPlacedCard[3] << 4) + byPlacedCard[4]];
	dwIndex[2] = TblFastCompareNT[(byPlacedCard[5] << 8) + (byPlacedCard[6] << 4) + byPlacedCard[7]];

	//ע����λ���������
	byte byType[3];
	byType[0] = dwIndex[0] >> 18;
	byType[1] = dwIndex[1] >> 18;
	byType[2] = dwIndex[2] >> 18;

	//ע�������ǻ�ɫʱ������ͬ��˳ƥ��У��
	if (byType[0] != vecOuterType[0])
		return false;
	if (byType[1] != vecOuterType[1] && vecOuterType[1] != TYPE_S_SHUNZI)
		return false;
	if (byType[2] != vecOuterType[2] && vecOuterType[2] != TYPE_S_SHUNZI)
		return false;

	//ע�������ǻ�ɫ�ĵ�����ζ�ſ��ǻ�ɫ��С��
	return dwIndex[0] <= dwIndex[1] && dwIndex[1] <= dwIndex[2];
}

void CCoSplit::CollectSwapIndex(VECBYTE &vecSwapIndex, const VECBYTE &vecOuterType)
{
	if (vecOuterType[0] == TYPE_SINGLE)
	{
		vecSwapIndex.push_back(0);
		vecSwapIndex.push_back(1);
	}

	if (vecOuterType[1] == TYPE_SINGLE)
	{
		vecSwapIndex.push_back(2);
		vecSwapIndex.push_back(3);
		vecSwapIndex.push_back(4);
	}
	else if (vecOuterType[1] == TYPE_PAIR)
		vecSwapIndex.push_back(4);

	if (vecOuterType[2] == TYPE_SINGLE)
	{
		vecSwapIndex.push_back(5);
		vecSwapIndex.push_back(6);
		vecSwapIndex.push_back(7);
	}
	else if (vecOuterType[2] == TYPE_PAIR)
		vecSwapIndex.push_back(7);
}

bool CCoSplit::SwapSingleCard(dword &dwLimitScore, VECBYTE &vecOuterType, byte byPlacedCard[8])
{
	//����ƥ�����轻��
	if (IsTypeMatch(vecOuterType, byPlacedCard))
		return true;

	//�����ƺ������Ƿ�ƥ��
	byte byTempCard[8];
	memcpy(byTempCard, byPlacedCard, sizeof (byte [8]));

	//���ҽ����ĵ���λ��
	VECBYTE vecSwapIndex;
	CollectSwapIndex(vecSwapIndex, vecOuterType);

	for (int i = 0; i < (int)vecSwapIndex.size(); i++)
	{
		for (int j = i + 1; j < (int)vecSwapIndex.size(); j++)
		{
			//��������
			SwapCard(byTempCard, vecSwapIndex[i], vecSwapIndex[j]);

			if (IsTypeMatch(vecOuterType, byTempCard))
			{
				memcpy(byPlacedCard, byTempCard, sizeof (byte [8]));
				return true;
			}

			//��ԭ����
			SwapCard(byTempCard, vecSwapIndex[i], vecSwapIndex[j]);
		}
	}

	//��������
	dwLimitScore = VSumMax;
	return false;
}

dword CCoSplit::SplitCards(dword &dwLimitScore, byte byCard[8], word wFlushBitmap)
{
	VECPILE vecPileMax;

	VSum = 0;
	VSumMax = 0;
	m_vecPile.clear();

	//ת����λͼ
	TransToBitmap(byCard);
	m_wFlushBitmap = wFlushBitmap;

	byte byType = INNER_TYPE_GVP;
	byte byGrade = GRADE_INIT;
	while (true)
	{
		//���Բ���
		if (m_vecPile.size() < 3)
		{
			byte byLastType = byType;
			byte byLastGrade = byGrade;
			if ((byType = SplitlPile(byType, byGrade)) != INNER_TYPE_GVP)
			{
				PushPile(m_vecPile, byType, byGrade);
				if (VSum > VSumMax)
				{
					if (VSum < dwLimitScore)
					{
						VSumMax = VSum;
						vecPileMax = m_vecPile;
					}
				}

				//���´�����ȼ�����
				byType = byLastType;
				byGrade = byLastGrade;
				continue;
			}
		}
		if (m_vecPile.empty())
			break;
		PopPile(m_vecPile, byType, byGrade);
	}

	//��������
	dwLimitScore = VSumMax;

	//�����ֵ���
	for (int i = 0; i < (int)vecPileMax.size(); i++)
		OutCards(vecPileMax[i].byType, vecPileMax[i].byGrade);

	//�����Ϊ����
	while (vecPileMax.size() < 3)
		vecPileMax.insert(vecPileMax.begin(), TPile(0, 0, INNER_TYPE_SINGLE, 0));

	//�Ȱڵ���
	byte byPlacedCard[8];
	PlaceCards_1(vecPileMax, byPlacedCard);
	PlaceCards_2(vecPileMax, byPlacedCard);

	//ת��Ϊ�ⲿ����
	VECBYTE vecOuterType;
	for (int i = 0; i < (int)vecPileMax.size(); i++)
	{
		byte byOuterType;
		TransToOuterType(vecPileMax[i].byType, byOuterType);
		vecOuterType.push_back(byOuterType);
	}

	//��������
	if (!SwapSingleCard(dwLimitScore, vecOuterType, byPlacedCard))
		return 0;

	dword dwTypeBitmap = GetTypeBitmap(vecPileMax);
	dword dwCardBitmap = GetCardBitmap(byCard, byPlacedCard);

	//���ͺͿ���λͼ
	return (dwTypeBitmap << 24) | dwCardBitmap;
}
