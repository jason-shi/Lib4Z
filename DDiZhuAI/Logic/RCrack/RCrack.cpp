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
		|| byContract == CONTRACT_FAIL && ((1 << byChair) & m_byFriend) == 0) //完成对家 or 未完成本家
	{
		if (m_nDepth == 1)
			return byContract;
		BackAction(byChair, byChair1, byType, byRspType, byGrade, byCount, wAttach, wChoice);
	}
	return CONTRACT_BKT;
}

byte CRCrack::Crack(byte byChair1, byte byType, byte byRspType, byte byGrade, byte byCount, word wAttach)
{
	//速胜应答
	byte byResult = m_pRPlayer[0]->OneHand(byType, byRspType, byGrade, byCount, wAttach);
	if (byResult == CONTRACT_YES)
	{
		m_tCrackResult.byRspType = byRspType;
		m_tCrackResult.byGrade = byGrade;
		m_tCrackResult.byCount = byCount;
		m_tCrackResult.wAttach = wAttach;
		SetResult(&m_tCrackResult);
		return CONTRACT_YES; //速胜
	}

	//计算卡牌数量
	int nCardsCount = 0;
	nCardsCount += m_pRPlayer[0]->GetCardsCount();
	nCardsCount += m_pRPlayer[1]->GetCardsCount();
	nCardsCount += m_pRPlayer[2]->GetCardsCount();

	return nCardsCount >= 30 ? Crack_1(byChair1, byType, byRspType, byGrade, byCount, wAttach)
							 : Crack_2(byChair1, byType, byRspType, byGrade, byCount, wAttach);
}

byte CRCrack::Crack_1(byte byChair1, byte byType, byte byRspType, byte byGrade, byte byCount, word wAttach)
{
	//破解初始化
	byte byChair = 0;
	word wChoice = 0;
	m_nDepth = 0;
	m_bGvpTag = false;

	//保存初始状态
	SaveAction(byChair, byChair1, byType, byRspType, byGrade, byCount, wAttach, wChoice);

	//复原手牌位图
	for (int i = 0; i < 3; i++)
		m_pRPlayer[i]->RestoreBitmap();

	dword dwInterrupt = 0;
	while (m_nDepth < MAX_CRACK_DEPTH)
	{
		if (dwInterrupt++ >= g_dwCrackDepth) //检测中断
			return INTERRUPT;

		if (byChair != byChair1 && ((m_byFriend & (1 << byChair)) == 0) == ((m_byFriend & (1 << byChair1)) == 0)
			&& ((byType != TYPE_SINGLE && byType != TYPE_PAIR) || byGrade >= 7))
		{
			//队友的牌，直接放弃
			byRspType = TYPE_GVP;
		}
		else
		{
			byRspType = m_pRPlayer[byChair]->RspCards(byType, byRspType, byGrade, byCount, wAttach, wChoice); //行为应答
		}
		if (byRspType == TYPE_GVP)
		{
			if (byType == TYPE_GVP) //无牌可出
			{
				if (m_nDepth == 1)
					return ((1 << byChair) & m_byFriend) == 0 ? CONTRACT_YES : CONTRACT_FAIL;
				BackAction(byChair, byChair1, byType, byRspType, byGrade, byCount, wAttach, wChoice);
				continue;
			}

			//获得下一个出牌的玩家
			byChair = GetNextPlayer(byChair);

			//更新出牌类型
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
		SaveAction(byChair, byChair1, byType, byRspType, byGrade, byCount, wAttach, wChoice); //保存出牌行为

		//获胜处理
		if (!m_pRPlayer[byChair]->IsAlive())
		{
			byte byContract = TranslateWin(byChair, byChair1, byType, byRspType, byGrade, byCount, wAttach, wChoice);
			if (byContract == CONTRACT_BKT)
				continue;
			if (byContract == CONTRACT_FAIL || byContract == CONTRACT_YES)
				return byContract;
		}

		//更新最后出牌者
		byChair1 = byChair;
		byChair = GetNextPlayer(byChair);
		byType = byRspType;

		//非回退时置零
		wChoice = 0;
	}
	return CONTRACT_UNK; //无效语句
}

byte CRCrack::Crack_2(byte byChair1, byte byType, byte byRspType, byte byGrade, byte byCount, word wAttach)
{
	//破解初始化
	byte byChair = 0;
	word wChoice = 0;
	m_nDepth = 0;
	m_bGvpTag = false;

	//保存初始状态
	SaveAction(byChair, byChair1, byType, byRspType, byGrade, byCount, wAttach, wChoice);

	//复原手牌位图
	for (int i = 0; i < 3; i++)
		m_pRPlayer[i]->RestoreBitmap();

	dword dwInterrupt = 0;
	while (m_nDepth < MAX_CRACK_DEPTH)
	{
		if (dwInterrupt++ >= g_dwCrackDepth) //检测中断
			return INTERRUPT;

		byRspType = m_pRPlayer[byChair]->RspCards(byType, byRspType, byGrade, byCount, wAttach, wChoice); //行为应答
		if (byRspType == TYPE_GVP)
		{
			if (byType == TYPE_GVP) //无牌可出
			{
				if (m_nDepth == 1)
					return ((1 << byChair) & m_byFriend) == 0 ? CONTRACT_YES : CONTRACT_FAIL;
				BackAction(byChair, byChair1, byType, byRspType, byGrade, byCount, wAttach, wChoice);
				continue;
			}

			//获得下一个出牌的玩家
			byChair = GetNextPlayer(byChair);

			//更新出牌类型
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
		SaveAction(byChair, byChair1, byType, byRspType, byGrade, byCount, wAttach, wChoice); //保存出牌行为

		//获胜处理
		if (!m_pRPlayer[byChair]->IsAlive())
		{
			byte byContract = TranslateWin(byChair, byChair1, byType, byRspType, byGrade, byCount, wAttach, wChoice);
			if (byContract == CONTRACT_BKT)
				continue;
			if (byContract == CONTRACT_FAIL || byContract == CONTRACT_YES)
				return byContract;
		}

		//更新最后出牌者
		byChair1 = byChair;
		byChair = GetNextPlayer(byChair);
		byType = byRspType;

		//非回退时置零
		wChoice = 0;
	}
	return CONTRACT_UNK; //无效语句
}

void CRCrack::Think(byte byChair1, byte byType, byte byRspType, byte byGrade, byte byCount, word wAttach)
{
	byRspType = m_pRPlayer[0]->MakeCards(byType, byGrade, byCount, wAttach);

	//保存在结果中
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
	//破解深度
	g_dwCrackDepth = m_tCrackParam.dwCrackDepth;

	byte byChair1 = m_tCrackParam.byChair1;
	byte byType = m_tCrackParam.byType;
	byte byRspType = m_tCrackParam.byRspType;
	byte byGrade = m_tCrackParam.byGrade;
	byte byCount = m_tCrackParam.byCount;
	word wAttach = m_tCrackParam.wAttach;

	//检测是否能压过
	if (!m_pRPlayer[0]->CheckBeat(byType, byGrade, byCount, wAttach))
	{
		m_bGvpTag = true;
		return SCORE_UNK;
	}

	//初始化思考参数（暴力破解部分）
	InitCrackParam();
	byte byResult = Crack(byChair1, byType, byRspType, byGrade, byCount, wAttach);
	if (byResult == CONTRACT_YES)
	{
		GetResult(&m_tCrackResult);
		if (IsStupidPut(m_tCrackResult.byRspType, m_tCrackResult.byGrade, m_tCrackResult.byCount, m_tCrackResult.wAttach))
		{
			//寻找更优解
			InitOptimizeParam();
			byResult = Optimize(byChair1, byType, m_tCrackResult.byRspType, m_tCrackResult.byGrade, m_tCrackResult.byCount, m_tCrackResult.wAttach);
			if (byResult != CONTRACT_YES)
			{
				//还原为第一个Stupid解
				SetResult(&m_tCrackResult);
			}
		}
		return SCORE_3;
	}

	//初始化思考参数（类人思考部分）
	InitThinkParam();
	Think(byChair1, byType, byRspType, byGrade, byCount, wAttach);

	return byResult == INTERRUPT ? SCORE_UNK : SCORE_1;
}
