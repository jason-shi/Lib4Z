#include "StdAfx.h"
#include "IRCrack.h"
#include "..\RPlayer\RPlayer.h"

IRCrack::IRCrack()
{
	for (int i = 0; i < 4; i++)
		m_pRPlayer[i] = NULL;
	for (int i = 0; i < MAX_CRACK_DEPTH; i++)
		m_pRAction[i] = NULL;
}

IRCrack::~IRCrack()
{
	for (int i = 0; i < 4; i++)
	{
		delete m_pRPlayer[i];
		m_pRPlayer[i] = NULL;
	}
	for (int i = 0; i < MAX_CRACK_DEPTH; i++)
	{
		delete m_pRAction[i];
		m_pRAction[i] = NULL;
	}
}

bool IRCrack::Initial()
{
	for (int i = 0; i < 4; i++)
	{
		m_pRPlayer[i] = new CRPlayer();
		if (!m_pRPlayer[i])
			return false;
	}
	for (int i = 0; i < MAX_CRACK_DEPTH; i++)
	{
		m_pRAction[i] = new TRAction;
		if (!m_pRAction[i])
			return false;
	}
	return true;
}

void IRCrack::SetBitmap(byte byChair, word bitmap[4])
{
	m_pRPlayer[byChair]->SetBitmap(bitmap);
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
	}
}

byte IRCrack::GetNextPlayer(byte byChair)
{
	while (true)
	{
		if (++byChair == 4)
			byChair = 0;
		if (m_pRPlayer[byChair]->IsAlive())
			return byChair;
	}
}

void IRCrack::GetNextRspser(byte &byChair, byte &byChair1)
{
	while (true)
	{
		if (++byChair == 4)
			byChair = 0;
		if (byChair == byChair1 || m_pRPlayer[byChair]->IsAlive())
			break;
	}
	if (byChair == byChair1) //全部放弃
	{
		if (!m_pRPlayer[byChair]->IsAlive())
		{
			byChair ^= 0x02; //对家接出牌权
			byChair1 = byChair;
		}
	}
}

bool IRCrack::CheckCanCha(byte &byChaChair, byte byGrade)
{
	for (byte i = 0; i < 4; i++)
	{
		if (i == byChaChair)
			continue;
		if (!m_pRPlayer[i]->IsAlive())
			continue;

		if (m_pRPlayer[i]->HasCards(byGrade, 2))
		{
			byChaChair = i;
			return true;
		}
	}
	return false;
}

bool IRCrack::CheckCanGou(byte &byGouChair, byte byGrade)
{
	for (byte i = 0; i < 4; i++)
	{
		if (i == byGouChair)
			continue;
		if (!m_pRPlayer[i]->IsAlive())
			continue;

		if (m_pRPlayer[i]->HasCards(byGrade, 1))
		{
			byGouChair = i;
			return true;
		}
	}
	return false;
}

bool IRCrack::IsStupidPut(byte byType, byte byGrade, byte byCount)
{
	return m_pRPlayer[0]->IsStupidPut(byType, byGrade, byCount);
}

void IRCrack::InitThkParam()
{
	//复原手牌位图
	for (int i = 0; i < 4; i++)
		m_pRPlayer[i]->RestoreBitmap();

	//获得类型恐惧
	byte byTypePhobia[4][3];
	for (int i = 0; i < 4; i++)
		m_pRPlayer[i]->GetTypePhobia(byTypePhobia[i]);

	//拟制恐惧攻击
	byte byPhobiaAttack = 0x00;
	for (int i = 0; i < 3; i++)
	{
		int nPhobiaDegree = 0; //恐惧程度
		for (int j = 0; j < 4; j++)
		{
			if (!m_pRPlayer[j]->IsAlive())
				continue;
			nPhobiaDegree += (j & 1) ? byTypePhobia[j][i] : -byTypePhobia[j][i];
		}
		if (nPhobiaDegree >= 4)
			byPhobiaAttack |= 1 << i;
	}

	//获得胜利节奏
	byte byWinRate[4];
	for (int i = 0; i < 4; i++)
	{
		if (!m_pRPlayer[i]->IsAlive())
			continue;
		m_pRPlayer[i]->GetWinRate(byWinRate[i]);
	}

	//标记炸弹抑制
	byte byWinRhythm = 0;
	if (byWinRate[0] <= 3)
		byWinRhythm |= 1 << 0;
	if (byWinRate[1] <= 2)
		byWinRhythm |= 1 << 1;
	if (byWinRate[2] <= 2)
		byWinRhythm |= 1 << 2;
	if (byWinRate[3] <= 2)
		byWinRhythm |= 1 << 3;

	m_pRPlayer[0]->SetThkParam(byPhobiaAttack, byWinRhythm);
}

void IRCrack::SaveAction(byte byWinBitmap, byte byChair, byte byChair1, byte byType, byte byRspType, byte byGrade, byte byCount)
{
	m_pRAction[m_nDepth]->byWinBitmap = byWinBitmap;
	m_pRAction[m_nDepth]->byChair = byChair;
	m_pRAction[m_nDepth]->byChair1 = byChair1;
	m_pRAction[m_nDepth]->byType = byType;
	m_pRAction[m_nDepth]->byRspType = byRspType;
	m_pRAction[m_nDepth]->byGrade = byGrade;
	m_pRAction[m_nDepth]->byCount = byCount;
	m_nDepth++;

	if (m_nDepth != 0)
		m_pRPlayer[byChair]->OutCards(byRspType, byGrade, byCount);
}

void IRCrack::BackAction(byte &byWinBitmap, byte &byChair, byte &byChair1, byte &byType, byte &byRspType, byte &byGrade, byte &byCount)
{
	m_nDepth--;
	byWinBitmap = m_pRAction[m_nDepth]->byWinBitmap;
	byChair = m_pRAction[m_nDepth]->byChair;
	byChair1 = m_pRAction[m_nDepth]->byChair1;
	byType = m_pRAction[m_nDepth]->byType;
	byRspType = m_pRAction[m_nDepth]->byRspType;
	byGrade = m_pRAction[m_nDepth]->byGrade;
	byCount = m_pRAction[m_nDepth]->byCount;

	if (m_nDepth != 0)
		m_pRPlayer[byChair]->InCards(byRspType, byGrade, byCount);
}

void IRCrack::CallCrack(TCrackParam *pCrackParam, TCrackResult *pCrackResult)
{
	//保存破解参数
	for (int i = 0; i < 4; i++)
		SetBitmap(i, pCrackParam->bitmap[i]);
	memcpy(&m_tCrackParam, pCrackParam, sizeof(m_tCrackParam));

	//获得破解结果
	byte byCrackScore = CrackSche();
	GetResult(pCrackResult);
	pCrackResult->byCrackScore = byCrackScore;
}
