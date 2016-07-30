#include "StdAfx.h"
#include "RCrack.h"

using namespace std;

CRCrack::CRCrack() : IRCrack()
{
}

CRCrack::~CRCrack()
{
}

void CRCrack::CallCrack(const TCrackParam* pCrackParam, TCrackResult *pCrackResult)
{
	InitCrack(pCrackParam);

	TH2 == 0 ? Crack_1(pCrackParam, pCrackResult)
			 : Crack_2(pCrackParam, pCrackResult);
}

void CRCrack::Crack_1(const TCrackParam* pCrackParam, TCrackResult *pCrackResult)
{
	//拆牌索引
	int nIndex = GetSplitIndex();

	//神码
	dword v = TblFastSplit[nIndex];

	//设置牌堆的类型
	SetResultType(pCrackResult, v);
	SetResultCard(pCrackResult, v);

	//同花顺调整
	if (pCrackResult->bySpecType == TYPE_NORMAL || pCrackResult->bySpecType == TYPE_TRI_SHUNZI)
		THShunAdjust(pCrackResult);
}

void CRCrack::Crack_2(const TCrackParam* pCrackParam, TCrackResult *pCrackResult)
{
	byte byCard[8];

	//构造第一个同花顺
	word S1 = 0;
	for (int i = 0; i < 3; i++)
	{
		byte byGrade = TblBitIndex[(S[TH1] & -S[TH1]) % 37];
		S1 |= S[TH1] & -S[TH1];
		S[TH1] &= S[TH1] - 1;
		byCard[i + 2] = (TH1 << 4) + byGrade;
	}
	//构造第二个同花顺
	word S2 = 0;
	for (int i = 0; i < 3; i++)
	{
		byte byGrade = TblBitIndex[(S[TH2] & -S[TH2]) % 37];
		S2 |= S[TH2] & -S[TH2];
		S[TH2] &= S[TH2] - 1;
		byCard[i + 5] = (TH2 << 4) + byGrade;
	}
	//构造剩余的两张牌
	for (int i = 0; i < 2; i++)
	{
		for (int j = 1; j < 5; j++)
		{
			if (S[j] == 0)
				continue;

			byte byGrade = TblBitIndex[(S[j] & -S[j]) % 37];
			S[j] &= S[j] - 1;
			byCard[i] = (j << 4) + byGrade;
			break;
		}
	}
	//同花顺调整
	if (S1 > S2)
	{
		for (int i = 0; i < 3; i++)
		{
			byte byTemp = byCard[i + 2];
			byCard[i + 2] = byCard[i + 5];
			byCard[i + 5] = byTemp;
		}
	}

	//设置类型
	pCrackResult->bySpecType = TYPE_NORMAL;
	pCrackResult->byType[0] = TYPE_SINGLE;
	if ((byCard[0] & 15) == (byCard[1] & 15))
		pCrackResult->byType[0] = TYPE_PAIR;
	pCrackResult->byType[1] = TYPE_S_SHUNZI;
	pCrackResult->byType[2] = TYPE_S_SHUNZI;

	//设置卡牌
	memcpy(pCrackResult->byCard, byCard, sizeof(byCard));
}

void CRCrack::InitCrack(const TCrackParam *pCrackParam)
{
	memcpy(m_byOriCard, pCrackParam->byCard, sizeof (byte [8]));

	//初始化 V
	for (int i = 0; i < 8; i++)
		V[i] = m_byOriCard[i] & 0x0F;

	//追加一个元素
	V[8] = (V[7] - V[0]) + V[7];

	//初始化 S
	memset(S, 0, sizeof(S));
	for (int i = 0; i < 8; i++)
	{
		byte bySuit = m_byOriCard[i] >> 4;
		S[bySuit] |= 1 << V[i];
	}

	//初始化 TH1
	TH1 = 0;
	for (int i = 1; i < 5; i++)
	{
		if (TblShunZi[S[i] & MASK_SHUNZI] != 0
			|| (S[i] & MASK_A23) == MASK_A23
			|| (S[i] & MASK_QKA) == MASK_QKA)
		{
			TH1 = i;
			break;
		}
	}
	//初始化 TH2
	TH2 = 0;
	for (int i = TH1 + 1; i < 5; i++)
	{
		if (TblShunZi[S[i] & MASK_SHUNZI] != 0
			|| (S[i] & MASK_A23) == MASK_A23
			|| (S[i] & MASK_QKA) == MASK_QKA)
		{
			TH2 = i;
			break;
		}
	}
}

int CRCrack::GetSplitIndex()
{
	static byte TStep[13] = { 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, };
	static word TSame[13] = { 0x0003, 0x0006, 0x000C, 0x0018, 0x0030, 0x0060, 0x00C0, 0x0180, 0x0300, 0x0600, 0x0C00, 0x1800, 0x1001, };

	//获得拆牌索引[0, 65536)
	int nIndex = 0;
	for (int i = 0; i < 8; i++)
	{
		byte byFlag = TStep[V[i + 1] - V[i]];
		if (byFlag == 1)
		{
			if ((TSame[V[i]] & S[TH1]) == TSame[V[i]])
				byFlag = 3;
		}
		nIndex |= byFlag << i << i;
	}
	return nIndex;
}

void CRCrack::SetResultType(TCrackResult *pCrackResult, const dword v)
{
	if (v >> 31)
	{
		pCrackResult->bySpecType = (v >> 24) & 0x7F;
		pCrackResult->byType[0] = TYPE_NORMAL;
		pCrackResult->byType[1] = TYPE_NORMAL;
		pCrackResult->byType[2] = TYPE_NORMAL;

		//特殊牌型：三顺子处理
		switch (pCrackResult->bySpecType)
		{
		case TYPE_TRI_SHUNZI_1S:
			pCrackResult->bySpecType = TYPE_TRI_SHUNZI;
			pCrackResult->byType[2] = TYPE_S_SHUNZI;
			break;
		case TYPE_TRI_SHUNZI_2S:
			pCrackResult->bySpecType = TYPE_TRI_SHUNZI;
			pCrackResult->byType[1] = TYPE_S_SHUNZI;
			pCrackResult->byType[2] = TYPE_S_SHUNZI;
			break;
		}
	}
	else
	{
		pCrackResult->bySpecType = TYPE_NORMAL;
		pCrackResult->byType[0] = ((v >> 24) & 01) == 0 ? TYPE_SINGLE : TYPE_PAIR;
		pCrackResult->byType[1] = (v >> 25) & 07;
		pCrackResult->byType[2] = (v >> 28) & 07;
	}
}

void CRCrack::SetResultCard(TCrackResult *pCrackResult, const dword v)
{
	pCrackResult->byCard[0] = m_byOriCard[((v) >>  0) & 07];
	pCrackResult->byCard[1] = m_byOriCard[((v) >>  3) & 07];
	pCrackResult->byCard[2] = m_byOriCard[((v) >>  6) & 07];
	pCrackResult->byCard[3] = m_byOriCard[((v) >>  9) & 07];
	pCrackResult->byCard[4] = m_byOriCard[((v) >> 12) & 07];
	pCrackResult->byCard[5] = m_byOriCard[((v) >> 15) & 07];
	pCrackResult->byCard[6] = m_byOriCard[((v) >> 18) & 07];
	pCrackResult->byCard[7] = m_byOriCard[((v) >> 21) & 07];
}

void CRCrack::THShunAdjust(TCrackResult *pCrackResult)
{
	//同花顺调整
	if (pCrackResult->byType[1] == TYPE_S_SHUNZI)
	{
		for (int j = 0; j < 3; j++)
		{
			if ((pCrackResult->byCard[2 + j] >> 4) != TH1)
			{
				for (int k = 0; k < 8; k++)
				{
					if ((pCrackResult->byCard[k] & 15) == (pCrackResult->byCard[2 + j] & 15)
						&& (pCrackResult->byCard[k] >> 4) == TH1)
					{
						byte byTemp = pCrackResult->byCard[2 + j];
						pCrackResult->byCard[2 + j] = pCrackResult->byCard[k];
						pCrackResult->byCard[k] = byTemp;
						break;
					}
				}
			}
		}
	}

	//同花顺调整
	if (pCrackResult->byType[2] == TYPE_S_SHUNZI)
	{
		for (int j = 0; j < 3; j++)
		{
			if ((pCrackResult->byCard[5 + j] >> 4) != TH1)
			{
				for (int k = 0; k < 8; k++)
				{
					if ((pCrackResult->byCard[k] & 15) == (pCrackResult->byCard[5 + j] & 15)
						&& (pCrackResult->byCard[k] >> 4) == TH1)
					{
						byte byTemp = pCrackResult->byCard[5 + j];
						pCrackResult->byCard[5 + j] = pCrackResult->byCard[k];
						pCrackResult->byCard[k] = byTemp;
						break;
					}
				}
			}
		}
	}
}
