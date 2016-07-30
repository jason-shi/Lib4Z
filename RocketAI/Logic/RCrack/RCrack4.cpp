#include "StdAfx.h"
#include "RCrack4.h"
#include "..\RPlayer\RPlayer.h"

CRCrack4::CRCrack4() : IRCrack()
{
}

CRCrack4::~CRCrack4()
{
}

byte CRCrack4::GetContract(byte byScore, byte byWinBitmap)
{
	switch (byScore)
	{
	case SCORE_5: //要求双赢
		if (byWinBitmap & 0xF0)
			return CONTRACT_FAIL;
		if (byWinBitmap == 0x03)
			return CONTRACT_YES;
		break;
	case SCORE_4:
		if (byWinBitmap & 0x10 || byWinBitmap == 0x61)
			return CONTRACT_FAIL;
		if (byWinBitmap == 0x03 || byWinBitmap == 0x25)
			return CONTRACT_YES;
		break;
	case SCORE_3:
		if (byWinBitmap == 0x30 || byWinBitmap == 0x52)
			return CONTRACT_FAIL;
		if (byWinBitmap & 0x01 || byWinBitmap == 0x16)
			return CONTRACT_YES;
		break;
	case SCORE_2:
		if (byWinBitmap == 0x30)
			return CONTRACT_FAIL;
		if (byWinBitmap & 0x01 || (byWinBitmap & 0x12) == 0x12)
			return CONTRACT_YES;
		break;
	}
	return CONTRACT_UNK;
}

byte CRCrack4::GetMaxScore(byte byWinBitmap)
{
	if (byWinBitmap == 0x00 || byWinBitmap == 0x01)
		return SCORE_5;
	if (byWinBitmap == 0x21)
		return SCORE_4;
	if (byWinBitmap == 0x10 || byWinBitmap == 0x12)
		return SCORE_3;

	return SCORE_1;
}

bool CRCrack4::TranslatePut(byte &byChair, byte &byChair1, byte &byType, byte &byRspType, byte &byGrade, byte &byCount)
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
				byChair ^= 0x02; //对家接出牌权
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

bool CRCrack4::TranslateGvp(byte &byChair, byte &byChair1, byte &byType, byte &byRspType, byte &byGrade, byte &byCount)
{
	if (byType == TYPE_CHA)
	{
		byChair = GetNextPlayer(byChair1);
		byType = TYPE_SINGLE;

		//做点手脚
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
			byChair = byChair1 ^ 0x02; //对家接出牌权
			byChair1 = byChair;
		}
		byType = TYPE_GVP;
		byRspType = TYPE_GVP;

		//做点手脚
		if (m_nDepth == 1)
		{
			m_pRAction[0]->byType = TYPE_SINGLE;
			m_bGvpTag = true;
		}
		return true;
	}
	return false;
}

byte CRCrack4::TranslateWin(byte byScore, byte &byWinBitmap, byte &byChair, byte &byChair1, byte &byType, byte &byRspType, byte &byGrade, byte &byCount)
{
	byte byWinMask = 1 << TblBitCount[byWinBitmap];
	byWinBitmap |= (byChair & 1) ? byWinMask << 4 : byWinMask;

	byte byContract = GetContract(byScore, byWinBitmap);
	if (byContract != CONTRACT_UNK)
	{
		while (byContract == CONTRACT_YES && (byChair & 1) == 0
			|| byContract == CONTRACT_FAIL && (byChair & 1) == 1) //完成对家 or 未完成本家
		{
			if (m_nDepth == 1)
				return byContract;
			BackAction(byWinBitmap, byChair, byChair1, byType, byRspType, byGrade, byCount);
		}
		return CONTRACT_BKT;
	}
	return CONTRACT_UNK;
}

byte CRCrack4::Crack(byte byWinBitmap, byte byChair1, byte byType, byte byRspType, byte byGrade, byte byCount, byte byScore)
{
	//破解初始化
	byte byChair = 0;
	m_nDepth = 0;
	m_bGvpTag = false;

	//保存初始状态
	SaveAction(byWinBitmap, byChair, byChair1, byType, byRspType, byGrade, byCount);

	//复原手牌位图
	for (int i = 0; i < 4; i++)
		m_pRPlayer[i]->RestoreBitmap();

	dword dwInterrupt = 0;
	while (m_nDepth < MAX_CRACK_DEPTH)
	{
		if (dwInterrupt++ >= g_dwCrackDepth) //检测中断
			return INTERRUPT;

		byRspType = m_pRPlayer[byChair]->RspCards(byType, byRspType, byGrade, byCount); //行为应答
		if (byRspType == TYPE_GVP)
		{
			//放弃操作转义
			if (TranslateGvp(byChair, byChair1, byType, byRspType, byGrade, byCount))
				continue;

			if (byType == TYPE_GVP) //无牌可出
			{
				if (m_nDepth == 1)
					return (byChair & 1) ? CONTRACT_YES : CONTRACT_FAIL;
				BackAction(byWinBitmap, byChair, byChair1, byType, byRspType, byGrade, byCount);
				continue;
			}

			//获得下一个出牌的玩家
			GetNextRspser(byChair, byChair1);

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
			}

			if (m_nDepth == 1)
				m_bGvpTag = true;
			continue;
		}
		SaveAction(byWinBitmap, byChair, byChair1, byType, byRspType, byGrade, byCount); //保存出牌行为

		//获胜处理
		if (!m_pRPlayer[byChair]->IsAlive())
		{
			byte byContract = TranslateWin(byScore, byWinBitmap, byChair, byChair1, byType, byRspType, byGrade, byCount);
			if (byContract == CONTRACT_BKT)
				continue;
			if (byContract == CONTRACT_FAIL || byContract == CONTRACT_YES)
				return byContract;
		}

		//更新最后出牌者
		byChair1 = byChair;

		//出牌操作转义
		if (TranslatePut(byChair, byChair1, byType, byRspType, byGrade, byCount))
			continue;

		//下一个出牌者
		byChair = GetNextPlayer(byChair);
		byType = byRspType;
	}
	return CONTRACT_UNK; //无效语句
}

void CRCrack4::Think(byte byChair1, byte byType, byte byRspType, byte byGrade, byte byCount)
{
	byRspType = m_pRPlayer[0]->MakeCards(byChair1, byType, byGrade, byCount);

	//保存在结果中
	m_pRAction[1]->byRspType = byRspType;
	if (byRspType != TYPE_GVP)
	{
		m_pRAction[1]->byGrade = byGrade;
		m_pRAction[1]->byCount = byCount;
	}
	m_bGvpTag = byRspType == TYPE_GVP;
}

byte CRCrack4::Optimize(byte byWinBitmap, byte byChair1, byte byType, byte byRspType, byte byGrade, byte byCount, byte byScore)
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

byte CRCrack4::CrackSche()
{
	//破解深度
	g_dwCrackDepth = m_tCrackParam.dwCrackDepth;

	byte byWinBitmap = m_tCrackParam.byWinBitmap;
	byte byChair1 = m_tCrackParam.byChair1;
	byte byType = m_tCrackParam.byType;
	byte byRspType = m_tCrackParam.byRspType;
	byte byGrade = m_tCrackParam.byGrade;
	byte byCount = m_tCrackParam.byCount;

	//尝试不同的订约，若终止则选择人脑式出牌
	byte byMaxScore = GetMaxScore(byWinBitmap);

	byte byScore, byResult;
	for (byScore = byMaxScore; byScore > SCORE_1; byScore--)
	{
		byResult = Crack(byWinBitmap, byChair1, byType, byRspType, byGrade, byCount, byScore);
		if (byResult == CONTRACT_YES || byResult == INTERRUPT)
			break;
	}

	//暴力破解成功
	if (byResult == CONTRACT_YES)
	{
		GetResult(&m_tCrackResult);
		if (IsStupidPut(m_tCrackResult.byRspType, m_tCrackResult.byGrade, m_tCrackResult.byCount))
		{
			//寻找更优解
			byResult = Optimize(byWinBitmap, byChair1, byType, m_tCrackResult.byRspType, m_tCrackResult.byGrade, m_tCrackResult.byCount, byScore);
			if (byResult != CONTRACT_YES)
			{
				//还原为第一个Stupid解
				SetResult(&m_tCrackResult);
			}
		}
		return byScore;
	}

	//采用类人思考
	InitThkParam();
	Think(byChair1, byType, byRspType, byGrade, byCount);

	return byResult == INTERRUPT ? SCORE_UNK : SCORE_1;
}
