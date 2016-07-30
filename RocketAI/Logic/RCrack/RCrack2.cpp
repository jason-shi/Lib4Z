#include "StdAfx.h"
#include "RCrack2.h"
#include "..\RPlayer\RPlayer.h"

CRCrack2::CRCrack2() : IRCrack()
{
}

CRCrack2::~CRCrack2()
{
}

bool CRCrack2::TranslatePut(byte &byChair, byte &byChair1, byte &byType, byte &byRspType, byte &byGrade, byte &byCount)
{
	if (byRspType == TYPE_SINGLE)
	{
		byte byChaChair = byChair1;
		if (CheckCanCha(byChaChair, byGrade))
		{
			byChair = byChaChair;
			byType = TYPE_CHA;
			byRspType = TYPE_GVP;
			return true;
		}
	}
	if (byRspType == TYPE_CHA)
	{
		byte byGouChair = byChair1;
		if (CheckCanGou(byGouChair, byGrade))
		{
			byChair = byGouChair;
			byType = TYPE_GOU;
			byRspType = TYPE_GVP;
		}
		else
		{
			if (!m_pRPlayer[byChair]->IsAlive())
			{
				byChair ^= 0x02; //�Լҽӳ���Ȩ
				byChair1 = byChair;
			}
			byType = TYPE_GVP;
			byRspType = TYPE_GVP;
		}
		return true;
	}
	if (byRspType == TYPE_GOU)
	{
		byType = TYPE_GVP;
		byRspType = TYPE_GVP;
		return true;
	}
	return false;
}

bool CRCrack2::TranslateGvp(byte &byChair, byte &byChair1, byte &byType, byte &byRspType, byte &byGrade, byte &byCount)
{
	if (byType == TYPE_CHA)
	{
		byChair = GetNextPlayer(byChair1);
		byType = TYPE_SINGLE;

		//�����ֽ�
		if (m_nDepth == 1)
		{
			m_pRAction[0]->byType = TYPE_SINGLE;
			m_bGvpTag = true;
		}
		return true;
	}
	if (byType == TYPE_GOU)
	{
		if (m_pRPlayer[byChair1]->IsAlive())
			byChair = byChair1;
		else
		{
			byChair = byChair1 ^ 0x02; //�Լҽӳ���Ȩ
			byChair1 = byChair;
		}
		byType = TYPE_GVP;
		byRspType = TYPE_GVP;

		//�����ֽ�
		if (m_nDepth == 1)
		{
			m_pRAction[0]->byType = TYPE_SINGLE;
			m_bGvpTag = true;
		}
		return true;
	}
	return false;
}

byte CRCrack2::TranslateWin(byte byScore, byte &byWinBitmap, byte &byChair, byte &byChair1, byte &byType, byte &byRspType, byte &byGrade, byte &byCount)
{
	byte byWinMask = 1 << TblBitCount[byWinBitmap];
	byWinBitmap |= (byChair & 1) ? byWinMask << 4 : byWinMask;

	byte byContract = (byChair & 1) == 0 ? CONTRACT_YES : CONTRACT_FAIL;
	while (byContract == CONTRACT_YES && (byChair & 1) == 0
		|| byContract == CONTRACT_FAIL && (byChair & 1) == 1) //��ɶԼ� or δ��ɱ���
	{
		if (m_nDepth == 1)
			return byContract;
		BackAction(byWinBitmap, byChair, byChair1, byType, byRspType, byGrade, byCount);
	}
	return CONTRACT_BKT;
}

byte CRCrack2::Crack(byte byWinBitmap, byte byChair1, byte byType, byte byRspType, byte byGrade, byte byCount, byte byScore)
{
	//�ƽ��ʼ��
	byte byChair = 0;
	m_nDepth = 0;
	m_bGvpTag = false;

	//�����ʼ״̬
	SaveAction(byWinBitmap, byChair, byChair1, byType, byRspType, byGrade, byCount);

	//��ԭ����λͼ
	for (int i = 0; i < 4; i++)
		m_pRPlayer[i]->RestoreBitmap();

	dword dwInterrupt = 0;
	while (m_nDepth < MAX_CRACK_DEPTH)
	{
		if (dwInterrupt++ >= g_dwCrackDepth) //����ж�
			return INTERRUPT;

		byRspType = m_pRPlayer[byChair]->RspCards(byType, byRspType, byGrade, byCount); //��ΪӦ��
		if (byRspType == TYPE_GVP)
		{
			//��������ת��
			if (TranslateGvp(byChair, byChair1, byType, byRspType, byGrade, byCount))
				continue;

			if (byType == TYPE_GVP) //���ƿɳ�
			{
				if (m_nDepth == 1)
					return (byChair & 1) ? CONTRACT_YES : CONTRACT_FAIL;
				BackAction(byWinBitmap, byChair, byChair1, byType, byRspType, byGrade, byCount);
				continue;
			}

			//�����һ�����Ƶ����
			GetNextRspser(byChair, byChair1);

			//���³�������
			if (byChair == byChair1)
				byType = TYPE_GVP;
			else
			{
				byType = m_pRAction[m_nDepth - 1]->byRspType;
				if (byType == TYPE_GVP)
					byType = m_pRAction[m_nDepth - 1]->byType;
				byGrade = m_pRAction[m_nDepth - 1]->byGrade;
				byCount = m_pRAction[m_nDepth - 1]->byCount;
			}

			if (m_nDepth == 1)
				m_bGvpTag = true;
			continue;
		}
		SaveAction(byWinBitmap, byChair, byChair1, byType, byRspType, byGrade, byCount); //���������Ϊ

		//��ʤ����
		if (!m_pRPlayer[byChair]->IsAlive())
		{
			byte byContract = TranslateWin(byScore, byWinBitmap, byChair, byChair1, byType, byRspType, byGrade, byCount);
			if (byContract == CONTRACT_BKT)
				continue;
			if (byContract == CONTRACT_FAIL || byContract == CONTRACT_YES)
				return byContract;
		}

		//������������
		byChair1 = byChair;

		//���Ʋ���ת��
		if (TranslatePut(byChair, byChair1, byType, byRspType, byGrade, byCount))
			continue;

		//��һ��������
		byChair = GetNextPlayer(byChair);
		byType = byRspType;
	}
	return CONTRACT_UNK; //��Ч���
}

void CRCrack2::Think(byte byChair1, byte byType, byte byRspType, byte byGrade, byte byCount)
{
	byRspType = m_pRPlayer[0]->MakeCards(byChair1, byType, byGrade, byCount);

	//�����ڽ����
	m_pRAction[1]->byRspType = byRspType;
	if (byRspType != TYPE_GVP)
	{
		m_pRAction[1]->byGrade = byGrade;
		m_pRAction[1]->byCount = byCount;
	}
	m_bGvpTag = byRspType == TYPE_GVP;
}

byte CRCrack2::Optimize(byte byWinBitmap, byte byChair1, byte byType, byte byRspType, byte byGrade, byte byCount, byte byScore)
{
	TCrackResult tTmpResult;
	while (true)
	{
		byte byResult = Crack(byWinBitmap, byChair1, byType, byRspType, byGrade, byCount, byScore);
		if (byResult == CONTRACT_YES)
		{
			GetResult(&tTmpResult);
			if (IsStupidPut(tTmpResult.byRspType, tTmpResult.byGrade, tTmpResult.byCount))
			{
				byRspType = tTmpResult.byRspType;
				byGrade = tTmpResult.byGrade;
				byCount = tTmpResult.byCount;
				continue;
			}
		}
		return byResult;
	}
}

byte CRCrack2::CrackSche()
{
	//�ƽ����
	g_dwCrackDepth = m_tCrackParam.dwCrackDepth;

	byte byWinBitmap = m_tCrackParam.byWinBitmap;
	byte byChair1 = m_tCrackParam.byChair1;
	byte byType = m_tCrackParam.byType;
	byte byRspType = m_tCrackParam.byRspType;
	byte byGrade = m_tCrackParam.byGrade;
	byte byCount = m_tCrackParam.byCount;

	byte byResult = Crack(byWinBitmap, byChair1, byType, byRspType, byGrade, byCount, SCORE_5);

	//�����ƽ�ɹ�
	if (byResult == CONTRACT_YES)
	{
		GetResult(&m_tCrackResult);
		if (IsStupidPut(m_tCrackResult.byRspType, m_tCrackResult.byGrade, m_tCrackResult.byCount))
		{
			//Ѱ�Ҹ��Ž�
			byResult = Optimize(byWinBitmap, byChair1, byType, m_tCrackResult.byRspType, m_tCrackResult.byGrade, m_tCrackResult.byCount, SCORE_5);
			if (byResult != CONTRACT_YES)
			{
				//��ԭΪ��һ��Stupid��
				SetResult(&m_tCrackResult);
			}
		}
		return SCORE_5;
	}

	//��������˼��
	InitThkParam();
	Think(byChair1, byType, byRspType, byGrade, byCount);

	return byResult == INTERRUPT ? SCORE_UNK : SCORE_1;
}
