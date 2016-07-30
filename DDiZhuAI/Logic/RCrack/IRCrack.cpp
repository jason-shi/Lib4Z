#include "StdAfx.h"
#include "IRCrack.h"
#include "..\RPlayer\RPlayerRole\RPlayerDZ.h"
#include "..\RPlayer\RPlayerRole\RPlayerSJ.h"
#include "..\RPlayer\RPlayerRole\RPlayerXJ.h"

IRCrack::IRCrack()
{
	for (int i = 0; i < 3; i++)
		m_pRPlayer[i] = NULL;

	for (int i = 0; i < MAX_CRACK_DEPTH; i++)
		m_pRAction[i] = NULL;
}

IRCrack::~IRCrack()
{
	for (int i = 0; i < MAX_CRACK_DEPTH; i++)
	{
		delete m_pRAction[i];
		m_pRAction[i] = NULL;
	}
}

bool IRCrack::Initial()
{
	for (int i = 0; i < MAX_CRACK_DEPTH; i++)
	{
		m_pRAction[i] = new TRAction;
		if (!m_pRAction[i])
			return false;
	}
	return true;
}

void IRCrack::CallCrack(TCrackParam *pCrackParam, TCrackResult *pCrackResult)
{
	//保存破解参数
	memcpy(&m_tCrackParam, pCrackParam, sizeof(m_tCrackParam));

	//设置角色和手牌
	CRPlayer *pRole[3] = { g_pRPlayerDZ, g_pRPlayerXJ, g_pRPlayerSJ, };
	for (int i = 0; i < 3; i++)
	{
		m_pRPlayer[i] = pRole[(i - m_tCrackParam.byDiZhu + 3) % 3];
		m_pRPlayer[i]->SetBitmap(m_tCrackParam.bitmap[i]);
	}

	//设置队友位图
	static byte TFriend[3] = { 0x01, 0x05, 0x03, };
	m_byFriend = TFriend[m_tCrackParam.byDiZhu];

	//破解调度过程
	byte byCrackScore = CrackSche();
	GetResult(pCrackResult);
	pCrackResult->byCrackScore = byCrackScore;
}

bool IRCrack::IsStupidPut(byte byType, byte byGrade, byte byCount, word wAttach)
{
	return m_pRPlayer[0]->IsStupidPut(byType, byGrade, byCount, wAttach);
}

void IRCrack::GetResult(TCrackResult *pCrackResult)
{
	if (m_bGvpTag)
		pCrackResult->byRspType = TYPE_GVP;
	else
	{
		pCrackResult->byRspType = m_pRAction[1]->byRspType;
		pCrackResult->byGrade = m_pRAction[1]->byGrade;
		pCrackResult->byCount = m_pRAction[1]->byCount;
		pCrackResult->wAttach = m_pRAction[1]->wAttach;
	}
}

void IRCrack::SetResult(TCrackResult *pCrackResult)
{
	if (pCrackResult->byRspType == TYPE_GVP)
		m_bGvpTag = true;
	else
	{
		m_bGvpTag = false;
		m_pRAction[1]->byRspType = pCrackResult->byRspType;
		m_pRAction[1]->byGrade = pCrackResult->byGrade;
		m_pRAction[1]->byCount = pCrackResult->byCount;
		m_pRAction[1]->wAttach = pCrackResult->wAttach;
	}
}

void IRCrack::SaveAction(byte byChair, byte byChair1, byte byType, byte byRspType, byte byGrade, byte byCount, word wAttach, word wChoice)
{
	m_pRAction[m_nDepth]->byChair = byChair;
	m_pRAction[m_nDepth]->byChair1 = byChair1;
	m_pRAction[m_nDepth]->byType = byType;
	m_pRAction[m_nDepth]->byRspType = byRspType;
	m_pRAction[m_nDepth]->byGrade = byGrade;
	m_pRAction[m_nDepth]->byCount = byCount;
	m_pRAction[m_nDepth]->wAttach = wAttach;
	m_pRAction[m_nDepth]->wChoice = wChoice;
	m_nDepth++;

	if (m_nDepth != 0)
		m_pRPlayer[byChair]->OutCards(byRspType, byGrade, byCount, wAttach);
}

void IRCrack::BackAction(byte &byChair, byte &byChair1, byte &byType, byte &byRspType, byte &byGrade, byte &byCount, word &wAttach, word &wChoice)
{
	m_nDepth--;
	byChair = m_pRAction[m_nDepth]->byChair;
	byChair1 = m_pRAction[m_nDepth]->byChair1;
	byType = m_pRAction[m_nDepth]->byType;
	byRspType = m_pRAction[m_nDepth]->byRspType;
	byGrade = m_pRAction[m_nDepth]->byGrade;
	byCount = m_pRAction[m_nDepth]->byCount;
	wAttach = m_pRAction[m_nDepth]->wAttach;
	wChoice = m_pRAction[m_nDepth]->wChoice;

	if (m_nDepth != 0)
		m_pRPlayer[byChair]->InCards(byRspType, byGrade, byCount, wAttach);
}

void IRCrack::InitCrackParam()
{
	m_pRPlayer[0]->SetFriend(m_byFriend);
	m_pRPlayer[0]->SetChair1(m_tCrackParam.byChair1);

	m_pRPlayer[0]->RestoreBitmap();
	m_pRPlayer[0]->SetOriginVA(m_pRPlayer[0]->SplitCards());
}

void IRCrack::InitThinkParam()
{
	//复原手牌位图
	for (int i = 0; i < 3; i++)
		m_pRPlayer[i]->RestoreBitmap();

	//设置地主的类型恐惧
	if (m_byFriend != 0x01)
	{
		byte byDiZhu = m_byFriend == 0x05 ? 1 : 2;
		m_pRPlayer[0]->SetDZPhobia(m_pRPlayer[byDiZhu]);
	}

	//设置胜利节奏位图
	m_pRPlayer[0]->SetWinRhythm(m_pRPlayer);
}

void IRCrack::InitOptimizeParam()
{
}
