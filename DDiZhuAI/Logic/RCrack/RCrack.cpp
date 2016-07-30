#include "StdAfx.h"
#include "RCrack.h"
#include "..\RPlayer\RPlayer.h"

using namespace std;

CRCrack::CRCrack() : IRCrack()
{
}

CRCrack::~CRCrack()
{
}

byte CRCrack::TranslateWin(byte &byChair, byte &byChair1, byte &byType, byte &byRspType, byte &byGrade, byte &byCount, word &wAttach, word &wChoice)
{
	byte byContract = ((1 << byChair) & m_byFriend) ? CONTRACT_YES : CONTRACT_FAIL;
	while (byContract == CONTRACT_YES && ((1 << byChair) & m_byFriend) != 0
		|| byContract == CONTRACT_FAIL && ((1 << byChair) & m_byFriend) == 0) //��ɶԼ� or δ��ɱ���
	{
		if (m_nDepth == 1)
			return byContract;
		BackAction(byChair, byChair1, byType, byRspType, byGrade, byCount, wAttach, wChoice);
	}
	return CONTRACT_BKT;
}

byte CRCrack::Crack(byte byChair1, byte byType, byte byRspType, byte byGrade, byte byCount, word wAttach)
{
	//��ʤӦ��
	byte byResult = m_pRPlayer[0]->OneHand(byType, byRspType, byGrade, byCount, wAttach);
	if (byResult == CONTRACT_YES)
	{
		m_tCrackResult.byRspType = byRspType;
		m_tCrackResult.byGrade = byGrade;
		m_tCrackResult.byCount = byCount;
		m_tCrackResult.wAttach = wAttach;
		SetResult(&m_tCrackResult);
		return CONTRACT_YES; //��ʤ
	}

	//���㿨������
	int nCardsCount = 0;
	nCardsCount += m_pRPlayer[0]->GetCardsCount();
	nCardsCount += m_pRPlayer[1]->GetCardsCount();
	nCardsCount += m_pRPlayer[2]->GetCardsCount();

	return nCardsCount >= 30 ? Crack_1(byChair1, byType, byRspType, byGrade, byCount, wAttach)
							 : Crack_2(byChair1, byType, byRspType, byGrade, byCount, wAttach);
}

byte CRCrack::Crack_1(byte byChair1, byte byType, byte byRspType, byte byGrade, byte byCount, word wAttach)
{
	//�ƽ��ʼ��
	byte byChair = 0;
	word wChoice = 0;
	m_nDepth = 0;
	m_bGvpTag = false;

	//�����ʼ״̬
	SaveAction(byChair, byChair1, byType, byRspType, byGrade, byCount, wAttach, wChoice);

	//��ԭ����λͼ
	for (int i = 0; i < 3; i++)
		m_pRPlayer[i]->RestoreBitmap();

	dword dwInterrupt = 0;
	while (m_nDepth < MAX_CRACK_DEPTH)
	{
		if (dwInterrupt++ >= g_dwCrackDepth) //����ж�
			return INTERRUPT;

		if (byChair != byChair1 && ((m_byFriend & (1 << byChair)) == 0) == ((m_byFriend & (1 << byChair1)) == 0)
			&& ((byType != TYPE_SINGLE && byType != TYPE_PAIR) || byGrade >= 7))
		{
			//���ѵ��ƣ�ֱ�ӷ���
			byRspType = TYPE_GVP;
		}
		else
		{
			byRspType = m_pRPlayer[byChair]->RspCards(byType, byRspType, byGrade, byCount, wAttach, wChoice); //��ΪӦ��
		}
		if (byRspType == TYPE_GVP)
		{
			if (byType == TYPE_GVP) //���ƿɳ�
			{
				if (m_nDepth == 1)
					return ((1 << byChair) & m_byFriend) == 0 ? CONTRACT_YES : CONTRACT_FAIL;
				BackAction(byChair, byChair1, byType, byRspType, byGrade, byCount, wAttach, wChoice);
				continue;
			}

			//�����һ�����Ƶ����
			byChair = GetNextPlayer(byChair);

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
				wAttach = m_pRAction[m_nDepth - 1]->wAttach;
				wChoice = 0;
			}

			if (m_nDepth == 1)
				m_bGvpTag = true;
			continue;
		}
		SaveAction(byChair, byChair1, byType, byRspType, byGrade, byCount, wAttach, wChoice); //���������Ϊ

		//��ʤ����
		if (!m_pRPlayer[byChair]->IsAlive())
		{
			byte byContract = TranslateWin(byChair, byChair1, byType, byRspType, byGrade, byCount, wAttach, wChoice);
			if (byContract == CONTRACT_BKT)
				continue;
			if (byContract == CONTRACT_FAIL || byContract == CONTRACT_YES)
				return byContract;
		}

		//������������
		byChair1 = byChair;
		byChair = GetNextPlayer(byChair);
		byType = byRspType;

		//�ǻ���ʱ����
		wChoice = 0;
	}
	return CONTRACT_UNK; //��Ч���
}

byte CRCrack::Crack_2(byte byChair1, byte byType, byte byRspType, byte byGrade, byte byCount, word wAttach)
{
	//�ƽ��ʼ��
	byte byChair = 0;
	word wChoice = 0;
	m_nDepth = 0;
	m_bGvpTag = false;

	//�����ʼ״̬
	SaveAction(byChair, byChair1, byType, byRspType, byGrade, byCount, wAttach, wChoice);

	//��ԭ����λͼ
	for (int i = 0; i < 3; i++)
		m_pRPlayer[i]->RestoreBitmap();

	dword dwInterrupt = 0;
	while (m_nDepth < MAX_CRACK_DEPTH)
	{
		if (dwInterrupt++ >= g_dwCrackDepth) //����ж�
			return INTERRUPT;

		byRspType = m_pRPlayer[byChair]->RspCards(byType, byRspType, byGrade, byCount, wAttach, wChoice); //��ΪӦ��
		if (byRspType == TYPE_GVP)
		{
			if (byType == TYPE_GVP) //���ƿɳ�
			{
				if (m_nDepth == 1)
					return ((1 << byChair) & m_byFriend) == 0 ? CONTRACT_YES : CONTRACT_FAIL;
				BackAction(byChair, byChair1, byType, byRspType, byGrade, byCount, wAttach, wChoice);
				continue;
			}

			//�����һ�����Ƶ����
			byChair = GetNextPlayer(byChair);

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
				wAttach = m_pRAction[m_nDepth - 1]->wAttach;
				wChoice = 0;
			}

			if (m_nDepth == 1)
				m_bGvpTag = true;
			continue;
		}
		SaveAction(byChair, byChair1, byType, byRspType, byGrade, byCount, wAttach, wChoice); //���������Ϊ

		//��ʤ����
		if (!m_pRPlayer[byChair]->IsAlive())
		{
			byte byContract = TranslateWin(byChair, byChair1, byType, byRspType, byGrade, byCount, wAttach, wChoice);
			if (byContract == CONTRACT_BKT)
				continue;
			if (byContract == CONTRACT_FAIL || byContract == CONTRACT_YES)
				return byContract;
		}

		//������������
		byChair1 = byChair;
		byChair = GetNextPlayer(byChair);
		byType = byRspType;

		//�ǻ���ʱ����
		wChoice = 0;
	}
	return CONTRACT_UNK; //��Ч���
}

void CRCrack::Think(byte byChair1, byte byType, byte byRspType, byte byGrade, byte byCount, word wAttach)
{
	byRspType = m_pRPlayer[0]->MakeCards(byType, byGrade, byCount, wAttach);

	//�����ڽ����
	m_pRAction[1]->byRspType = byRspType;
	if (byRspType != TYPE_GVP)
	{
		m_pRAction[1]->byGrade = byGrade;
		m_pRAction[1]->byCount = byCount;
		m_pRAction[1]->wAttach = wAttach;
	}
	m_bGvpTag = byRspType == TYPE_GVP;
}

byte CRCrack::Optimize(byte byChair1, byte byType, byte byRspType, byte byGrade, byte byCount, word wAttach)
{
	TCrackResult tTmpResult;
	while (true)
	{
		byte byResult = Crack(byChair1, byType, byRspType, byGrade, byCount, wAttach);
		if (byResult == CONTRACT_YES)
		{
			GetResult(&tTmpResult);
			if (IsStupidPut(tTmpResult.byRspType, tTmpResult.byGrade, tTmpResult.byCount, tTmpResult.wAttach))
			{
				byRspType = tTmpResult.byRspType;
				byGrade = tTmpResult.byGrade;
				byCount = tTmpResult.byCount;
				wAttach = tTmpResult.wAttach;
				continue;
			}
		}
		return byResult;
	}
}

byte CRCrack::CrackSche()
{
	//�ƽ����
	g_dwCrackDepth = m_tCrackParam.dwCrackDepth;

	byte byChair1 = m_tCrackParam.byChair1;
	byte byType = m_tCrackParam.byType;
	byte byRspType = m_tCrackParam.byRspType;
	byte byGrade = m_tCrackParam.byGrade;
	byte byCount = m_tCrackParam.byCount;
	word wAttach = m_tCrackParam.wAttach;

	//����Ƿ���ѹ��
	if (!m_pRPlayer[0]->CheckBeat(byType, byGrade, byCount, wAttach))
	{
		m_bGvpTag = true;
		return SCORE_UNK;
	}

	//��ʼ��˼�������������ƽⲿ�֣�
	InitCrackParam();
	byte byResult = Crack(byChair1, byType, byRspType, byGrade, byCount, wAttach);
	if (byResult == CONTRACT_YES)
	{
		GetResult(&m_tCrackResult);
		if (IsStupidPut(m_tCrackResult.byRspType, m_tCrackResult.byGrade, m_tCrackResult.byCount, m_tCrackResult.wAttach))
		{
			//Ѱ�Ҹ��Ž�
			InitOptimizeParam();
			byResult = Optimize(byChair1, byType, m_tCrackResult.byRspType, m_tCrackResult.byGrade, m_tCrackResult.byCount, m_tCrackResult.wAttach);
			if (byResult != CONTRACT_YES)
			{
				//��ԭΪ��һ��Stupid��
				SetResult(&m_tCrackResult);
			}
		}
		return SCORE_3;
	}

	//��ʼ��˼������������˼�����֣�
	InitThinkParam();
	Think(byChair1, byType, byRspType, byGrade, byCount, wAttach);

	return byResult == INTERRUPT ? SCORE_UNK : SCORE_1;
}
