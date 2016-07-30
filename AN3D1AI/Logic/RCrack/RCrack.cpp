#include "StdAfx.h"
#include "RCrack.h"
#include "..\RPlayer\RPlayerRole\RPlayerBK.h"
#include "..\RPlayer\RPlayerRole\RPlayerSJ.h"
#include "..\RPlayer\RPlayerRole\RPlayerDJ.h"
#include "..\RPlayer\RPlayerRole\RPlayerXJ.h"

using namespace std;

static byte			 g_byOriCardCount;
static byte			 g_byOriTotal5TK;
static byte			 g_byTarget5TK;

CRCrack::CRCrack()
{
	for (int i = 0; i < 4; i++)
		m_pRPlayer[i] = NULL;
}

CRCrack::~CRCrack()
{
}

bool CRCrack::Initial()
{
	return true;
}

void CRCrack::CallCrack(TCrackParam *pCrackParam, TCrackResult *pCrackResult)
{
	static byte TFriend[4] = { 0x01, 0x0D, 0x07, 0x0B, };

	//保存参数
	memcpy(&m_tCrackParam, pCrackParam, sizeof(m_tCrackParam));

	//设置角色
	CRPlayer *pRole[4] = { g_pRPlayerBK, g_pRPlayerXJ, g_pRPlayerDJ, g_pRPlayerSJ, };
	for (int i = 0; i < 4; i++)
		m_pRPlayer[i] = pRole[(i - m_tCrackParam.byBanker + 4) & 0x03];

	g_dwCrackDepth = m_tCrackParam.dwCrackDepth;
	m_byBanker = m_tCrackParam.byBanker;
	m_byFriend = TFriend[m_tCrackParam.byBanker];

	//破解调度
	byte byCrackScore = CrackSche();
	GetResult(pCrackResult);
	pCrackResult->byCrackScore = byCrackScore;
}

void CRCrack::InitOriCardCount()
{
	g_byOriCardCount = 0;
	for (int i = 0; i < 4; i++)
		g_byOriCardCount += WORD_BIT_COUNT(m_tCrackParam.tPlayerParam[0].bitmap[i]);
	if (m_tCrackParam.tPlayerParam[0].byFuTwo != 0)
		g_byOriCardCount += m_tCrackParam.tPlayerParam[0].byFuTwo - 1;
}

void CRCrack::InitOriTotal5TK()
{
	g_byOriTotal5TK = m_tCrackParam.tBattleParam.byCurr5TK;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			word _bitmap = m_tCrackParam.tPlayerParam[i].bitmap[j] & MASK_5TK;
			while (_bitmap != 0)
			{
				g_byOriTotal5TK += TblMaxScore[_bitmap];
				_bitmap ^= 1 << TblPutScore[_bitmap];
			}
		}
	}
}

byte CRCrack::CrackSche()
{
	byte byResult = CONTRACT_FAIL;
#if 0
	InitOriCardCount();
	if (m_byBanker == 0)
	{
		if (g_byOriCardCount <= 2)
		{
			InitOriTotal5TK();
			for (g_byTarget5TK = 0; g_byTarget5TK <= g_byOriTotal5TK; g_byTarget5TK += 5)
			{
				byResult = Crack(); //暴力破解
				if (byResult == CONTRACT_YES || byResult == INTERRUPT)
					break;
			}
		}
	}
	else
	{
		if (g_byOriCardCount <= 2)
		{
			InitOriTotal5TK();
			for (g_byTarget5TK = g_byOriTotal5TK; g_byTarget5TK >= 0; g_byTarget5TK -= 5)
			{
				byResult = Crack(); //暴力破解
				if (byResult == CONTRACT_YES || byResult == INTERRUPT)
					break;
			}
		}
	}
	if (byResult == CONTRACT_YES)
	{
		GetResult(&m_tCrackResult);
		return SCORE_3;
	}
#endif
	Think(); //类人思考

	return byResult == INTERRUPT ? SCORE_UNK : SCORE_1;
}

byte CRCrack::TranslateWin()
{
	byte byContract = CONTRACT_YES;
	if (m_byBanker == 0)
	{
		if (Z.byGain5TK > g_byTarget5TK || Z.byChairM != 0)
			byContract = CONTRACT_FAIL;
	}
	else
	{
		if (Z.byGain5TK < g_byTarget5TK && Z.byChairM == 0)
			byContract = CONTRACT_FAIL;
	}

	if (m_nActionDepth == 1)
		return byContract;
	BackAction();
	while (byContract == CONTRACT_YES && ((1 << Z.byChair) & m_byFriend) != 0   //完成，一直退到敌人出牌
		|| byContract == CONTRACT_FAIL && ((1 << Z.byChair) & m_byFriend) == 0) //失败，一直退到朋友出牌
	{
		if (m_nActionDepth == 1)
			return byContract;
		BackAction();
	}
	return CONTRACT_BKT;
}

byte CRCrack::UpdateAction()
{
	static byte TScore5TK[16] = { 0, 0, 5, 0, 0, 0, 0, 10, 0, 0, 10, 0, 0, 0, 0, 0, };
	Z.byCurr5TK += TScore5TK[Z.byGrade];

	if (Z.byChair != Z.byChair1) //非首家出牌
	{
		if (Z.byRspType == Z.byTypeM) //和最大牌同花色
		{
			if (Z.byGrade > Z.byGradeM || Z.byGradeM == GRADE_INIT)
			{
				Z.byChairM = Z.byChair;
				Z.byGradeM = Z.byGrade;
			}
		}
		else if (Z.byRspType == TYPE_SPADE) //第一次用主杀
		{
			Z.byChairM = Z.byChair;
			Z.byTypeM = Z.byRspType;
			Z.byGradeM = Z.byGrade;
		}
	}

	if (((Z.byChair + 1) & 0x03) == Z.byChair1) //最后一家出牌
	{
		if (Z.byChairM != m_byBanker) //闲家得分
			Z.byGain5TK += Z.byCurr5TK;
		Z.byCurr5TK = 0;

		//一轮牌结束时进行结算
		CRPlayer *pMe = m_pRPlayer[0];
		if (pMe->bitmap[0] + pMe->bitmap[1] + pMe->bitmap[2] + pMe->bitmap[3] == 0)
		{
			byte byContract = TranslateWin();
			if (byContract == CONTRACT_BKT)
				return CONTRACT_BKT;
			if (byContract == CONTRACT_FAIL || byContract == CONTRACT_YES)
				return byContract;
		}

		Z.byType = TYPE_GVP;
		Z.byChair = Z.byChairM;
		Z.byChair1 = Z.byChair;
	}
	else if (Z.byChair == Z.byChair1) //首家出牌
	{
		Z.byType = Z.byRspType;
		Z.byChairM = Z.byChair;
		Z.byTypeM = Z.byType;
		Z.byGradeM = Z.byGrade;
		Z.byChair = (Z.byChair + 1) & 0x03;
	}
	else
	{
		Z.byChair = (Z.byChair + 1) & 0x03;
	}
	Z.byRspType = TYPE_GVP;
	Z.byGrade = GRADE_INIT;

	return CONTRACT_UNK;
}

void CRCrack::RestoreWith(const TCrackParam& tCrackParam)
{
	Z.byType    = tCrackParam.tBattleParam.byType;
	Z.byRspType = tCrackParam.tBattleParam.byRspType;
	Z.byGrade   = tCrackParam.tBattleParam.byGrade;
	Z.byChair   = 0;
	Z.byChair1  = tCrackParam.tBattleParam.byChair1;
	Z.byChairM  = tCrackParam.tBattleParam.byChairM;
	Z.byTypeM   = tCrackParam.tBattleParam.byTypeM;
	Z.byGradeM  = tCrackParam.tBattleParam.byGradeM;
	Z.byCurr5TK = tCrackParam.tBattleParam.byCurr5TK;
	Z.byGain5TK = 0;
	Z.byLost5TK = 0;

	for (int i = 0; i < 4; i++)
		m_pRPlayer[i]->RestoreWith(i, tCrackParam);
}

byte CRCrack::Crack()
{
	//从保存的原始参数恢复
	RestoreWith(m_tCrackParam);

	//第一个节点存放输入参数
	m_nActionDepth = 0;
	SaveAction();

	dword dwInterrupt = 0;
	while (m_nActionDepth < MAX_ACTION_DEPTH)
	{
		if (dwInterrupt++ >= g_dwCrackDepth) //检测中断
			return INTERRUPT;

		Z.byRspType = m_pRPlayer[Z.byChair]->RspCards(Z.byType, Z.byRspType, Z.byGrade); //行为应答
		if (Z.byRspType == TYPE_GVP)
		{
			byte byTempChair = Z.byChair;
			if (m_nActionDepth == 1)
				return ((1 << Z.byChair) & m_byFriend) == 0 ? CONTRACT_YES : CONTRACT_FAIL;
			BackAction();
			while (((1 << Z.byChair) & m_byFriend) != ((1 << byTempChair) & m_byFriend)) //一直退到朋友出牌
			{
				if (m_nActionDepth == 1)
					return ((1 << byTempChair) & m_byFriend) == 0 ? CONTRACT_YES : CONTRACT_FAIL;
				BackAction();
			}
			continue;
		}
		SaveAction();
		UpdateAction();
	}
	return CONTRACT_UNK; //无效语句
}
