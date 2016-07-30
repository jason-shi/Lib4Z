#include "StdAfx.h"
#include "CoCrack.h"

using namespace std;

static byte			 g_byParam1;
static byte			 g_byParam2;
static byte			 g_byParam3;
static byte			 g_byParam4;
static int			 g_nSplitIndex[TotalPlayer - 1];
static int			 g_nWinScore[TotalPlayer - 1];

static TCoCrackResult g_tTempResult;

CCoCrack::CCoCrack() : CRCrack()
{
}

CCoCrack::~CCoCrack()
{
}

void CCoCrack::CallCrack(const TCoCrackParam* pCoCrackParam, TCoCrackResult *pCoCrackResult)
{
	InitCrack(pCoCrackParam);

	g_byParam1 = pCoCrackParam->byParam1;
	g_byParam2 = pCoCrackParam->byParam2;
	g_byParam3 = pCoCrackParam->byParam3;
	g_byParam4 = pCoCrackParam->byParam4;

	for (int i = 0; i < TotalPlayer - 1; i++)
	{
		//��ò�������[0, 65536)
		g_nSplitIndex[i] = GetSplitIndex(i);

		//����
		//dword v = TblCoFastSplit[0][g_nSplitIndex[i]];
		dword v = TblFastSplit[g_nSplitIndex[i]];

		//�����ƶѵ�����
		SetResultType(i, pCoCrackResult, v);
		SetResultCard(i, pCoCrackResult, v);
	}
	memcpy(&g_tTempResult, pCoCrackResult, sizeof(g_tTempResult));

	pCoCrackResult->byCoordinate = 0;

	//�ж�ͨɱ��ҿ�����
	byte byKillAll = CheckKillAll(pCoCrackResult);
	if (byKillAll == 0)
	{
		pCoCrackResult->byCoordinate = 0; //�����
		goto MMM;
	}

	//���ݷ����ⶨͨɱ�����
	byte byMaxChair = GetChairByScore(byKillAll, pCoCrackResult);

	//��һ�ѳ���Q�ԣ���ڶ��ѳ���10ը �������
	//if (pCoCrackResult->byType[byMaxChair][0] == TYPE_PAIR && (pCoCrackResult->byCard[byMaxChair][0] & 0x0F) >= 10
	//	|| pCoCrackResult->byType[byMaxChair][1] >= TYPE_BOMB && (pCoCrackResult->byCard[byMaxChair][0] & 0x0F) >= 8)
	//	goto NNN;

	//for (int i = 0; i < TotalPlayer - 1; i++)
	//{
	//	if (i == byMaxChair)
	//		continue;
	//	if (pCoCrackResult->byType[i][0] == TYPE_PAIR && ComapreWith(0, pCoCrackResult->byCard[i], pCoCrackResult->byCard[byMaxChair])
	//		|| pCoCrackResult->byType[i][1] >= TYPE_BOMB && ComapreWith(1, pCoCrackResult->byCard[i], pCoCrackResult->byCard[byMaxChair])
	//		|| pCoCrackResult->byType[i][2] >= TYPE_BOMB && ComapreWith(2, pCoCrackResult->byCard[i], pCoCrackResult->byCard[byMaxChair]))
	//	{
	//		pCoCrackResult->byCoordinate = 0; //�����
	//		goto MMM;
	//	}
	//}

	bool bCanKillAll = true;
	int k = 1;
	for (int i = 0; i < TotalPlayer - 1; )
	{
		if (i == byMaxChair)
		{
			i++;
			continue;
		}
		if (pCoCrackResult->bySpecType[i] != 0)
		{
			i++;
			continue;
		}

		//�Ƚ��������Ƿ�ͨɱ
		int j;
		for (j = 0; j < 3; j++)
		{
			if (pCoCrackResult->byType[byMaxChair][j] > pCoCrackResult->byType[i][j])
				continue;
			if (pCoCrackResult->byType[byMaxChair][j] == pCoCrackResult->byType[i][j])
			{
				if (ComapreWith(j, pCoCrackResult->byCard[byMaxChair], pCoCrackResult->byCard[i]))
					continue;
			}

			//����ͨɱ�˻�����
			pCoCrackResult->byCoordinate = 1;
			break;
		}
		if (j != 3)
		{
			//�˻�����Ҫ��һ�ְڷ�
			if (TblCoFastSplit[k][g_nSplitIndex[i]] == 0)
			{
				bCanKillAll = false;
				break;
			}
			dword v = TblCoFastSplit[k][g_nSplitIndex[i]];
			k++;
			if (k == 8)
			{
				bCanKillAll = false;
				break;
			}

			//�����ƶѵĿ���
			SetResultType(i, pCoCrackResult, v);
			SetResultCard(i, pCoCrackResult, v);
			continue;
		}
		i++;
		k = 1;
	}

	//��������ܹ�ͨɱ
	if (bCanKillAll && pCoCrackResult->byCoordinate == 1)
	{
		//��ò���ϵĵ÷ֿ�����
		int oldScore = GetWinScore(&g_tTempResult);
		//�����ϵĵ÷ֿ�����
		int newScore = GetWinScore(pCoCrackResult);

		if (newScore > oldScore - g_byParam4)
		{
			pCoCrackResult->byCoordinate = 2;
		}
		else
		{
			bCanKillAll = false;
		}
	}

	if (bCanKillAll == false)
	{
		//����ͨɱ�����°�������õİ�
		for (int i = 0; i < TotalPlayer - 1; i++)
		{
			//����
			dword v = TblFastSplit[g_nSplitIndex[i]];

			//�����ƶѵ�����
			SetResultType(i, pCoCrackResult, v);
			SetResultCard(i, pCoCrackResult, v);
		}
	}

MMM:
	//ͬ��˳����
	for (int i = 0; i < TotalPlayer - 1; i++)
	{
		if (pCoCrackResult->bySpecType[i] == TYPE_NORMAL || pCoCrackResult->bySpecType[i] == TYPE_TRI_SHUNZI)
			THShunAdjust(i, pCoCrackResult);
	}
}

void CCoCrack::InitCrack(const TCoCrackParam *pCoCrackParam)
{
	for (int i = 0; i < TotalPlayer - 1; i++)
	{
		//��ʼ�� V
		memset(V[i], 0, sizeof(V[i]));
		for (int j = 0; j < 8; j++)
			V[i][j] = pCoCrackParam->byCard[i][j] & 0x0F;

		//׷��һ��Ԫ��
		V[i][8] = (V[i][7] - V[i][0]) + V[i][7];

		//��ʼ�� S
		memset(S[i], 0, sizeof(S[i]));
		for (int j = 0; j < 8; j++)
		{
			byte bySuit = pCoCrackParam->byCard[i][j] >> 4;
			S[i][bySuit] |= 1 << V[i][j];
		}

		//��ʼ�� TH1
		TH1[i] = 0;
		for (int j = 1; j < 5; j++)
		{
			if (TblShunZi[S[i][j] & MASK_SHUNZI] != 0
				|| (S[i][j] & MASK_A23) == MASK_A23
				|| (S[i][j] & MASK_QKA) == MASK_QKA)
			{
				TH1[i] = j;
				break;
			}
		}
		//��ʼ�� TH2
		TH2[i] = 0;
		for (int j = TH1[i] + 1; j < 5; j++)
		{
			if (TblShunZi[S[i][j] & MASK_SHUNZI] != 0
				|| (S[i][j] & MASK_A23) == MASK_A23
				|| (S[i][j] & MASK_QKA) == MASK_QKA)
			{
				TH2[i] = j;
				break;
			}
		}

		//���濨��
		memcpy(m_byOriCard[i], pCoCrackParam->byCard[i], sizeof (byte [8]));
	}
}

int CCoCrack::GetSplitIndex(int i)
{
	static byte TStep[13] = { 0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, };
	static word TSame[13] = { 0x0003, 0x0006, 0x000C, 0x0018, 0x0030, 0x0060, 0x00C0, 0x0180, 0x0300, 0x0600, 0x0C00, 0x1800, 0x1001, };

	//��ò�������[0, 65536)
	int nIndex = 0;
	for (int j = 0; j < 8; j++)
	{
		byte byFlag = TStep[V[i][j + 1] - V[i][j]];
		if (byFlag == 1)
		{
			if ((TSame[V[i][j]] & S[i][TH1[i]]) == TSame[V[i][j]])
				byFlag = 3;
		}
		nIndex |= byFlag << j << j;
	}
	return nIndex;
}

void CCoCrack::SetResultType(int i, TCoCrackResult *pCoCrackResult, const dword v)
{
	if (v >> 31)
	{
		pCoCrackResult->bySpecType[i] = (v >> 24) & 0x7F;
		pCoCrackResult->byType[i][0] = TYPE_NORMAL;
		pCoCrackResult->byType[i][1] = TYPE_NORMAL;
		pCoCrackResult->byType[i][2] = TYPE_NORMAL;

		//�������ͣ���˳�Ӵ���
		switch (pCoCrackResult->bySpecType[i])
		{
		case TYPE_TRI_SHUNZI_1S:
			pCoCrackResult->bySpecType[i] = TYPE_TRI_SHUNZI;
			pCoCrackResult->byType[i][2] = TYPE_S_SHUNZI;
			break;
		case TYPE_TRI_SHUNZI_2S:
			pCoCrackResult->bySpecType[i] = TYPE_TRI_SHUNZI;
			pCoCrackResult->byType[i][1] = TYPE_S_SHUNZI;
			pCoCrackResult->byType[i][2] = TYPE_S_SHUNZI;
			break;
		}
	}
	else
	{
		pCoCrackResult->bySpecType[i] = TYPE_NORMAL;
		pCoCrackResult->byType[i][0] = ((v >> 24) & 01) == 0 ? TYPE_SINGLE : TYPE_PAIR;
		pCoCrackResult->byType[i][1] = (v >> 25) & 07;
		pCoCrackResult->byType[i][2] = (v >> 28) & 07;
	}
}

void CCoCrack::SetResultCard(int i, TCoCrackResult *pCoCrackResult, const dword v)
{
	pCoCrackResult->byCard[i][0] = m_byOriCard[i][((v) >>  0) & 07];
	pCoCrackResult->byCard[i][1] = m_byOriCard[i][((v) >>  3) & 07];
	pCoCrackResult->byCard[i][2] = m_byOriCard[i][((v) >>  6) & 07];
	pCoCrackResult->byCard[i][3] = m_byOriCard[i][((v) >>  9) & 07];
	pCoCrackResult->byCard[i][4] = m_byOriCard[i][((v) >> 12) & 07];
	pCoCrackResult->byCard[i][5] = m_byOriCard[i][((v) >> 15) & 07];
	pCoCrackResult->byCard[i][6] = m_byOriCard[i][((v) >> 18) & 07];
	pCoCrackResult->byCard[i][7] = m_byOriCard[i][((v) >> 21) & 07];
}

void CCoCrack::THShunAdjust(int i, TCoCrackResult *pCoCrackResult)
{
	//ͬ��˳����
	if (pCoCrackResult->byType[i][1] == TYPE_S_SHUNZI)
	{
		for (int j = 0; j < 3; j++)
		{
			if ((pCoCrackResult->byCard[i][2 + j] >> 4) != TH1[i])
			{
				for (int k = 0; k < 8; k++)
				{
					if ((pCoCrackResult->byCard[i][k] & 15) == (pCoCrackResult->byCard[i][2 + j] & 15)
						&& (pCoCrackResult->byCard[i][k] >> 4) == TH1[i])
					{
						byte byTemp = pCoCrackResult->byCard[i][2 + j];
						pCoCrackResult->byCard[i][2 + j] = pCoCrackResult->byCard[i][k];
						pCoCrackResult->byCard[i][k] = byTemp;
						break;
					}
				}
			}
		}
	}

	//ͬ��˳����
	if (pCoCrackResult->byType[i][2] == TYPE_S_SHUNZI)
	{
		for (int j = 0; j < 3; j++)
		{
			if ((pCoCrackResult->byCard[i][5 + j] >> 4) != TH1[i])
			{
				for (int k = 0; k < 8; k++)
				{
					if ((pCoCrackResult->byCard[i][k] & 15) == (pCoCrackResult->byCard[i][5 + j] & 15)
						&& (pCoCrackResult->byCard[i][k] >> 4) == TH1[i])
					{
						byte byTemp = pCoCrackResult->byCard[i][5 + j];
						pCoCrackResult->byCard[i][5 + j] = pCoCrackResult->byCard[i][k];
						pCoCrackResult->byCard[i][k] = byTemp;
						break;
					}
				}
			}
		}
	}
}

byte CCoCrack::CheckKillAll(TCoCrackResult *pCoCrackResult)
{
	byte byKillAll = 0;
	for (int i = 0; i < TotalPlayer - 1; i++)
	{
		//���������;Ͳ���ͨɱ
		if (pCoCrackResult->bySpecType[i] != 0)
		{
			byKillAll = 0;
			break;
		}

		//��һ��Ϊ����Param1�Ķ��ӣ��ڶ���Ϊ����Param2��˳�ӣ�������Ϊ����Param3��˳��
		if ((pCoCrackResult->byType[i][0] > TYPE_SINGLE
				|| pCoCrackResult->byType[i][0] == TYPE_SINGLE && (pCoCrackResult->byCard[i][0] & 0x0F) >= g_byParam1)
			&& (pCoCrackResult->byType[i][1] > TYPE_SHUNZI
				|| pCoCrackResult->byType[i][1] == TYPE_SHUNZI && (pCoCrackResult->byCard[i][2] & 0x0F) >= g_byParam2)
			&& (pCoCrackResult->byType[i][2] > TYPE_SHUNZI
				|| pCoCrackResult->byType[i][2] == TYPE_SHUNZI && (pCoCrackResult->byCard[i][5] & 0x0F) >= g_byParam3))
		{
			byKillAll |= 1 << i;
			continue;
		}
	}
	return byKillAll;
}

byte CCoCrack::GetChairByScore(byte byKillAll, TCoCrackResult *pCoCrackResult)
{
	byte byScore[3] = { 0 };
	for (int i = 0; i < TotalPlayer - 1; i++)
	{
		if ((byKillAll & (1 << i)) == 0)
			continue;

		if (pCoCrackResult->byType[i][0] == TYPE_PAIR)
			byScore[i] += (pCoCrackResult->byCard[i][0] & 0x0F) + 2;
		if (pCoCrackResult->byType[i][1] == TYPE_BOMB)
			byScore[i] += 8;
		else if (pCoCrackResult->byType[i][1] == TYPE_S_SHUNZI)
			byScore[i] += 10;
		else
			byScore[i] += 1;
		//������
		if (pCoCrackResult->byType[i][2] == TYPE_BOMB)
			byScore[i] += 4;
		else if (pCoCrackResult->byType[i][2] == TYPE_S_SHUNZI)
			byScore[i] += 5;
		else
			byScore[i] += 1;
	}
	byte byMaxScore = 0;
	byte byMaxChair;
	for (int i = 0; i < TotalPlayer - 1; i++)
	{
		if (byScore[i] > byMaxScore)
		{
			byMaxScore = byScore[i];
			byMaxChair = i;
		}
	}
	return byMaxChair;
}

bool CCoCrack::ComapreWith(byte byPileNum, byte byCard1[8], byte byCard2[8])
{
	dword dwIndex1 = 0, dwIndex2 = 0;
	switch (byPileNum)
	{
	case 0:
		dwIndex1 = ((byCard1[0] - 16) << 12) + ((byCard1[1] - 16) << 6) + 0x3F;
		dwIndex2 = ((byCard2[0] - 16) << 12) + ((byCard2[1] - 16) << 6) + 0x3F;
		break;
	case 1:
		dwIndex1 = ((byCard1[2] - 16) << 12) + ((byCard1[3] - 16) << 6) + (byCard1[4] - 16);
		dwIndex2 = ((byCard2[2] - 16) << 12) + ((byCard2[3] - 16) << 6) + (byCard2[4] - 16);
		break;
	case 2:
		dwIndex1 = ((byCard1[5] - 16) << 12) + ((byCard1[6] - 16) << 6) + (byCard1[7] - 16);
		dwIndex2 = ((byCard2[5] - 16) << 12) + ((byCard2[6] - 16) << 6) + (byCard2[7] - 16);
		break;
	}
	return TblFastCompare[dwIndex1] > TblFastCompare[dwIndex2];
}

int CCoCrack::GetWinScore(TCoCrackResult *pCoCrackResult)
{
	int nScore[TotalPlayer - 1] = { 0 };
	for (int i = 0; i < TotalPlayer - 1; i++)
	{
		if (pCoCrackResult->byType[i][0] == TYPE_PAIR)
			nScore[i] += (pCoCrackResult->byCard[i][0] & 0x0F) + 2;
		if (pCoCrackResult->byType[i][1] == TYPE_BOMB)
			nScore[i] += 8;
		else if (pCoCrackResult->byType[i][1] == TYPE_S_SHUNZI)
			nScore[i] += 10;
		else if (pCoCrackResult->byType[i][1] == TYPE_SHUNZI)
			nScore[i] += 1;
		//������
		if (pCoCrackResult->byType[i][2] == TYPE_BOMB)
			nScore[i] += 4;
		else if (pCoCrackResult->byType[i][2] == TYPE_S_SHUNZI)
			nScore[i] += 5;
		else if (pCoCrackResult->byType[i][2] == TYPE_SHUNZI)
			nScore[i] += 1;
	}
	return nScore[0] + nScore[1] + nScore[2];
}
