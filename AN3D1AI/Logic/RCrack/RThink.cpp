#include "StdAfx.h"
#include "RCrack.h"
#include "..\RPlayer\RPlayerRole\RPlayerBK.h"
#include "..\RPlayer\RPlayerRole\RPlayerSJ.h"
#include "..\RPlayer\RPlayerRole\RPlayerDJ.h"
#include "..\RPlayer\RPlayerRole\RPlayerXJ.h"

using namespace std;

struct TMaxGrade {
	byte		 byType;
	byte		 byGrade;
};

struct TMaxScore {
	byte		 byType;
	byte		 byGrade;
	byte		 byScore;
};

//庄家使用的数据
static byte			 g_byTrumpCount[4];
static bool			 g_bCanTrumpKill;
static bool			 g_bEscapeCard[4];
static bool			 g_bBankerSweep[4];

//闲家使用的数据
static word			 g_wScoreBitmap[4];

static TMaxGrade	 g_tMaxGrade[4];
static byte			 g_byChairM0123;
static byte			 g_byTypeM0123;
static byte			 g_byGradeM0123;

static TMaxScore	 g_tMaxScore[4];
static byte			 g_byFriendScore;

static bool			 g_bICanOverBanker;
static bool			 g_bHadOverBanker;
static bool			 g_bOverBankerWithScore;

byte CRCrack::GetMiddleGrade(word _bitmap)
{
	word temp = _bitmap;

	//优先出不是分的牌
	_bitmap &= ~MASK_5TK;
	if (_bitmap != 0)
		return TblBitIndex[(_bitmap & -_bitmap) % 37];

	//出最小的牌
	return TblBitIndex[(temp & -temp) % 37];
}

void CRCrack::InitTrumpCount()
{
	for (int i = 0; i < 4; i++)
	{
		word _bitmap = m_pRPlayer[i]->bitmap[0];

		g_byTrumpCount[i] = WORD_BIT_COUNT(_bitmap);
		if (m_pRPlayer[i]->m_byFuTwo != 0)
			g_byTrumpCount[i] += m_pRPlayer[i]->m_byFuTwo - 1;
	}

	g_bCanTrumpKill = true;

	//庄家不可以杀的条件
	for (int i = 1; i < 4; i++)
	{
		if (g_byTrumpCount[0] <= g_byTrumpCount[i]) //主牌个数不够多
		{
			g_bCanTrumpKill = false;
			break;
		}
		if (g_byTrumpCount[0] <= g_byTrumpCount[i] + 1) //多一个
		{
			if (m_pRPlayer[0]->bitmap[0] <= m_pRPlayer[i]->bitmap[0]) //主牌不够大
			{
				g_bCanTrumpKill = false;
				break;
			}
		}
	}

	//庄家不可以杀的条件
	if (g_byTrumpCount[0] == 1 //最后一张主
		&& g_byTrumpCount[1] == 0
		&& g_byTrumpCount[2] == 0
		&& g_byTrumpCount[3] == 0)
	{
		for (int i = 1; i < 4; i++)
		{
			for (int j = 1; j < 4; j++)
			{
				if (m_pRPlayer[0]->bitmap[j] == 0)
					continue;
				if (m_pRPlayer[0]->bitmap[j] < m_pRPlayer[i]->bitmap[j]) //副牌不够大
				{
					g_bCanTrumpKill = false;
					break;
				}
			}
		}
	}
}

void CRCrack::InitEscapeCard()
{
	memset(g_bEscapeCard, 0, sizeof(g_bEscapeCard));
	for (byte bySuit = TYPE_HEART; bySuit != TYPE_ERROR; bySuit++)
	{
		word _bitmap = m_pRPlayer[0]->bitmap[bySuit - 1];
		if (_bitmap == 0)
			continue;

		if (m_pRPlayer[1]->bitmap[bySuit - 1] == 0 //只有庄家有的牌
			&& m_pRPlayer[2]->bitmap[bySuit - 1] == 0
			&& m_pRPlayer[3]->bitmap[bySuit - 1] == 0)
		{
			g_bEscapeCard[bySuit - 1] = false;
			continue;
		}
		if (WORD_BIT_COUNT(_bitmap) >= 5) //这种花色的牌较多
		{
			g_bEscapeCard[bySuit - 1] = false;
			continue;
		}
		if (_bitmap > m_pRPlayer[1]->bitmap[bySuit - 1] //大牌在手
			&& _bitmap > m_pRPlayer[2]->bitmap[bySuit - 1]
			&& _bitmap > m_pRPlayer[3]->bitmap[bySuit - 1])
		{
			g_bEscapeCard[bySuit - 1] = false;
			continue;
		}
		if ((_bitmap & 0x03FF) == 0) //不垫A,K
		{
			g_bEscapeCard[bySuit - 1] = false;
			continue;
		}
		g_bEscapeCard[bySuit - 1] = true;
	}

	for (byte bySuit = TYPE_HEART; bySuit != TYPE_ERROR; bySuit++)
	{
		word _bitmap = m_pRPlayer[0]->bitmap[bySuit - 1];
		if (_bitmap == 0)
			continue;
		if (_bitmap & _bitmap - 1) //不唯一
			continue;

		//唯一且不是最大，必垫
		if (m_pRPlayer[1]->bitmap[bySuit - 1] > _bitmap
			|| m_pRPlayer[2]->bitmap[bySuit - 1] > _bitmap
			|| m_pRPlayer[3]->bitmap[bySuit - 1] > _bitmap)
		{
			memset(g_bEscapeCard, 0, sizeof(g_bEscapeCard));
			g_bEscapeCard[bySuit - 1] = true;
			break;
		}
	}
}

void CRCrack::SimSweepCard(word bitmap[4], byte byFuTwo[4])
{
	//庄家出最大的
	byte byGrade = bitmap[0] & 0xFF00 ? TblMaxGrade[bitmap[0] >> 8] + 8 : TblMaxGrade[bitmap[0]];
	if (byGrade != 12)
		bitmap[0] ^= 1 << byGrade;
	else
	{
		if (--byFuTwo[0] == 0)
			bitmap[0] ^= 1 << byGrade;
	}

	//闲家出最小的
	for (int i = 1; i < 4; i++)
	{
		if (bitmap[i] != 0)
		{
			byGrade = TblBitIndex[(bitmap[i] & -bitmap[i]) % 37];
			if (byGrade != 12)
				bitmap[i] ^= 1 << byGrade;
			else
			{
				if (--byFuTwo[i] == 0)
					bitmap[i] ^= 1 << byGrade;
			}
		}
	}
}

void CRCrack::InitBankerSweep()
{
	for (byte bySuit = TYPE_SPADE; bySuit != TYPE_ERROR; bySuit++)
	{
		word bitmap[4] = { 0 };
		byte byFuTwo[4] = { 0 };
		for (int i = 0; i < 4; i++)
		{
			bitmap[i] = m_pRPlayer[i]->bitmap[bySuit - 1];
			if (bySuit == TYPE_SPADE)
				byFuTwo[i] = m_pRPlayer[i]->m_byFuTwo;
		}

		g_bBankerSweep[bySuit - 1] = bitmap[0] != 0;
		while (bitmap[0] != 0)
		{
			//判断闲家是否比庄家大
			if (bitmap[1] > bitmap[0] || bitmap[2] > bitmap[0] || bitmap[3] > bitmap[0])
			{
				g_bBankerSweep[bySuit - 1] = false;
				break;
			}
			//判断闲家能用主牌杀
			if (bySuit != TYPE_SPADE)
			{
				if (bitmap[1] == 0 && m_pRPlayer[1]->bitmap[0] != 0
					|| bitmap[2] == 0 && m_pRPlayer[2]->bitmap[0] != 0
					|| bitmap[3] == 0 && m_pRPlayer[3]->bitmap[0] != 0)
				{
					g_bBankerSweep[bySuit - 1] = false;
					break;
				}
			}
			//模拟清牌动作
			SimSweepCard(bitmap, byFuTwo);
		}
	}
}

void CRCrack::InitMaxGrade()
{
	//获得最大的出牌方式
	memset(g_tMaxGrade, 0, sizeof(g_tMaxGrade));
	for (int i = 0; i < Z.byChair1; i++)
		g_tMaxGrade[i].byType = m_pRPlayer[i]->GetMaxGrade(Z.byType, g_tMaxGrade[i].byGrade);

	//最大的玩家
	byte byChairM = Z.byChairM;
	byte byTypeM = Z.byTypeM;
	byte byGradeM = Z.byGradeM;
	for (int i = 0; i < Z.byChair1; i++)
	{
		if (g_tMaxGrade[i].byType == byTypeM) //和最大牌同花色
		{
			if (g_tMaxGrade[i].byGrade > byGradeM || byGradeM == GRADE_INIT)
			{
				byChairM = i;
				byGradeM = g_tMaxGrade[i].byGrade;
			}
		}
		else if (g_tMaxGrade[i].byType == TYPE_SPADE) //第一次用主杀
		{
			byChairM = i;
			byTypeM = g_tMaxGrade[i].byType;
			byGradeM = g_tMaxGrade[i].byGrade;
		}
	}
	g_byChairM0123 = byChairM;
	g_byTypeM0123 = byTypeM;
	g_byGradeM0123 = byGradeM;
}

void CRCrack::InitMaxScore()
{
	byte byRspType;
	byte byGrade;
	byte byScore;
	memset(g_tMaxScore, 0, sizeof(g_tMaxScore));
	for (int i = 0; i < Z.byChair1; i++)
	{
		byRspType = m_pRPlayer[i]->GetMaxScore(Z.byType, byGrade, byScore);
		g_tMaxScore[i].byType = byRspType;
		g_tMaxScore[i].byGrade = byGrade;
		g_tMaxScore[i].byScore = byScore;
	}

	//另外两家队友的最大分数
	g_byFriendScore = m_tCrackParam.tBattleParam.byCurr5TK;
	for (int i = 1; i < Z.byChair1; i++)
	{
		if (i == m_byBanker)
			continue;
		g_byFriendScore += g_tMaxScore[i].byScore;
	}
}

void CRCrack::InitOverBanker()
{
	//自己和庄家比较
	if (Z.byChairM == m_byBanker)
	{
		g_bICanOverBanker = false;
		if (g_tMaxGrade[0].byType == Z.byTypeM) //和最大牌同花色
		{
			if (g_tMaxGrade[0].byGrade > Z.byGradeM || Z.byGradeM == GRADE_INIT)
				g_bICanOverBanker = true;
		}
		else if (g_tMaxGrade[0].byType == TYPE_SPADE) //第一次用主杀
			g_bICanOverBanker = true;
	}
	else
	{
		if (Z.byChair1 <= m_byBanker)
		{
			g_bICanOverBanker = false;
		}
		else
		{
			g_bICanOverBanker = true;
			if (g_tMaxGrade[m_byBanker].byType != TYPE_GVP
				&& g_tMaxGrade[m_byBanker].byType == g_tMaxGrade[0].byType) //和最大牌同花色
			{
				if (g_tMaxGrade[m_byBanker].byGrade > g_tMaxGrade[0].byGrade || g_tMaxGrade[0].byGrade == GRADE_INIT)
					g_bICanOverBanker = false;
			}
			else if (g_tMaxGrade[m_byBanker].byType == TYPE_SPADE) //第一次用主杀
				g_bICanOverBanker = false;
		}
	}

	//队友出分是否比庄家大
	g_bOverBankerWithScore = false;
	if (Z.byChair1 > m_byBanker)
	{
		for (int i = 1; i < Z.byChair1; i++)
		{
			if (i == m_byBanker)
				continue;
			if (g_tMaxScore[i].byScore == 0)
				continue;
			if (g_tMaxScore[i].byType != TYPE_GVP
				&& g_tMaxScore[i].byType == g_tMaxGrade[m_byBanker].byType
				&& g_tMaxScore[i].byGrade > g_tMaxGrade[m_byBanker].byGrade) //类型相同等级大
			{
				g_bOverBankerWithScore = true;
				break;
			}
			if (g_tMaxScore[i].byType == TYPE_SPADE
				&& g_tMaxGrade[m_byBanker].byType != TYPE_SPADE) //队友用主分杀
			{
				g_bOverBankerWithScore = true;
				break;
			}
		}
	}

	//队友是否已经比庄家大
	g_bHadOverBanker = false;
	if (Z.byChair1 > m_byBanker)
	{
		if (g_tMaxGrade[m_byBanker].byType == TYPE_GVP) //庄家垫副牌
		{
			g_bHadOverBanker = true;
		}
		if (Z.byTypeM == g_tMaxGrade[m_byBanker].byType
			&& Z.byGradeM >= g_tMaxGrade[m_byBanker].byGrade) //类型相同等级大
		{
			g_bHadOverBanker = true;
		}
		if (Z.byTypeM == TYPE_SPADE
			&& g_tMaxGrade[m_byBanker].byType != TYPE_SPADE) //队友用主杀
		{
			g_bHadOverBanker = true;
		}
	}
}

byte CRCrack::BankerFirstCard(byte &byGrade)
{
	//初始化清牌数据
	InitTrumpCount();
	InitBankerSweep();

	//掉主以防被抠底
	if (m_pRPlayer[1]->bitmap[0] > m_pRPlayer[0]->bitmap[0]
		|| m_pRPlayer[2]->bitmap[0] > m_pRPlayer[0]->bitmap[0]
		|| m_pRPlayer[3]->bitmap[0] > m_pRPlayer[0]->bitmap[0])
	{
		word _bitmap = m_pRPlayer[0]->bitmap[0];
		if (_bitmap != 0)
		{
			if ((_bitmap & ~MASK_5TK) != 0)
			{
				_bitmap &= ~MASK_5TK;
				if ((_bitmap & _bitmap - 1) == 0) //不用最大的掉主
				{
					byte byTrumpCount = 0;
					for (int i = 1; i < 4; i++)
						byTrumpCount += g_byTrumpCount[i];
					if (byTrumpCount >= 2)
						_bitmap = m_pRPlayer[0]->bitmap[0];
				}
			}
			byGrade = TblBitIndex[(_bitmap & -_bitmap) % 37];
			return TYPE_SPADE;
		}
	}

	//不能掉主以防被抠底
	for (int i = 1; i < 4; i++)
	{
		if (g_byTrumpCount[i] > g_byTrumpCount[0])
		{
			//主牌大致还足够，防副牌被杀，仍然要掉主：考虑分布6张、7张、2张、3张
			if (g_byTrumpCount[0] >= 4) //继续清有三张主牌的玩家
			{
				word _bitmap = m_pRPlayer[0]->bitmap[0];
				if ((_bitmap & ~MASK_5TK) != 0)
				{
					_bitmap &= ~MASK_5TK;
					if ((_bitmap & _bitmap - 1) == 0) //不用最大的掉主
					{
						byte byTrumpCount = 0;
						for (int i = 1; i < 4; i++)
							byTrumpCount += g_byTrumpCount[i];
						if (byTrumpCount >= 2)
							_bitmap = m_pRPlayer[0]->bitmap[0];
					}
				}
				byGrade = TblBitIndex[(_bitmap & -_bitmap) % 37];
				return TYPE_SPADE;
			}
			else
			{
				//出副牌的大牌
				for (byte byRspType = TYPE_HEART; byRspType != TYPE_ERROR; byRspType++)
				{
					word _bitmap = m_pRPlayer[0]->bitmap[byRspType - 1];
					if (_bitmap != 0)
					{
						byGrade = _bitmap & 0xFF00 ? TblMaxGrade[_bitmap >> 8] + 8 : TblMaxGrade[_bitmap];
						return byRspType;
					}
				}
			}
		}
	}

	//闲家没有主牌时，优先出副牌
	byte byRspFrom = TYPE_SPADE;
	if (m_pRPlayer[1]->bitmap[0] == 0 && m_pRPlayer[2]->bitmap[0] == 0 && m_pRPlayer[3]->bitmap[0] == 0)
		byRspFrom = TYPE_HEART;

	//如果能清牌，则优先清牌
	for (byte byRspType = byRspFrom; byRspType != TYPE_ERROR; byRspType++)
	{
		if (g_bBankerSweep[byRspType - 1])
		{
			word _bitmap = m_pRPlayer[0]->bitmap[byRspType - 1];

			//从刚好超过闲家的牌开始出
			word _other = m_pRPlayer[1]->bitmap[byRspType - 1]
						| m_pRPlayer[2]->bitmap[byRspType - 1]
						| m_pRPlayer[3]->bitmap[byRspType - 1];
			if (_other != 0)
			{
				byte _grade = _other & 0xFF00 ? TblMaxGrade[_other >> 8] + 8 : TblMaxGrade[_other];
				if (_grade == 12)
					_grade = 11;
				_bitmap &= ~((1 << (_grade + 1)) - 1); //刚好超过闲家的牌
			}
			byGrade = TblBitIndex[(_bitmap & -_bitmap) % 37];
			return byRspType;
		}
	}

	//不能清牌，出最多的副牌
	for (byte byRspType = byRspFrom; byRspType != TYPE_ERROR; byRspType++)
	{
		word _bitmap = m_pRPlayer[0]->bitmap[byRspType - 1];
		if (WORD_BIT_COUNT(_bitmap) >= 3)
		{
			byGrade = GetMiddleGrade(_bitmap);
			return byRspType;
		}
	}

	//不能清牌，出副牌
	for (byte byRspType = byRspFrom; byRspType != TYPE_ERROR; byRspType++)
	{
		word _bitmap = m_pRPlayer[0]->bitmap[byRspType - 1];
		if (_bitmap != 0)
		{
			byGrade = GetMiddleGrade(_bitmap);
			return byRspType;
		}
	}

	//最后的选择
	return m_pRPlayer[0]->MakeCards(TYPE_GVP, byGrade);
}

byte CRCrack::BankerFollowCard(byte &byGrade)
{
	//初始化思考数据
	InitTrumpCount();
	InitEscapeCard();
	InitBankerSweep();
	InitMaxGrade();
	InitMaxScore();

	if (m_pRPlayer[0]->bitmap[Z.byType - 1] != 0)
	{
		word _bitmap = m_pRPlayer[0]->bitmap[Z.byType - 1];
		if (g_byFriendScore >= 15 || Z.byCurr5TK >= 10) //闲家超过15分或者已经超过10分
		{
			if (Z.byChair1 == 1) //三家已经出过
			{
				if (g_byChairM0123 == 0) //庄家大，说明最大牌是同花色
				{
					_bitmap &= TblContraryMask[Z.byGradeM];
					if ((_bitmap & MASK_5TK) != 0) //有分先出分牌
						_bitmap &= MASK_5TK;
				}
				else //闲家大
				{
					if ((_bitmap & ~MASK_5TK) != 0) //优先出分牌以外的小牌
						_bitmap &= ~MASK_5TK;
				}
			}
			else //还有闲家没有出牌
			{
				if (Z.byTypeM == Z.byType) //能够大于已经出过的牌
				{
					if (_bitmap & TblContraryMask[Z.byGradeM])
						_bitmap &= TblContraryMask[Z.byGradeM];
				}
				if ((_bitmap & ~MASK_5TK) != 0) //优先出分牌以外的小牌
					_bitmap &= ~MASK_5TK;
			}
		}
		else //分不多
		{
			if (Z.byChair1 == 1) //三家已经出过
			{
				if (g_byChairM0123 == 0) //庄家大，说明最大牌是同花色
				{
					_bitmap &= TblContraryMask[Z.byGradeM];
					if ((_bitmap & MASK_5TK) != 0) //有分先出分牌
						_bitmap &= MASK_5TK;
				}
				else //闲家大
				{
					if ((_bitmap & ~MASK_5TK) != 0) //优先出分牌以外的小牌
						_bitmap &= ~MASK_5TK;
				}
			}
			else
			{
				if (_bitmap & TblContraryMask[Z.byGradeM])
					_bitmap &= TblContraryMask[Z.byGradeM];
				if ((_bitmap & ~MASK_5TK) != 0) //优先出分牌以外的小牌
					_bitmap &= ~MASK_5TK;
			}
		}
		byGrade = TblBitIndex[(_bitmap & -_bitmap) % 37];
		return Z.byType;
	}
	else
	{
		if (g_bCanTrumpKill)
		{
			word _bitmap = m_pRPlayer[0]->bitmap[0];
			if (Z.byTypeM != TYPE_SPADE
				&& g_tMaxGrade[1].byType != TYPE_SPADE
				&& g_tMaxGrade[2].byType != TYPE_SPADE
				&& g_tMaxGrade[3].byType != TYPE_SPADE) //随意杀
			{
				if (_bitmap & MASK_5TK)
					_bitmap &= MASK_5TK;
				byGrade = TblBitIndex[(_bitmap & -_bitmap) % 37];
				return TYPE_SPADE;
			}
			if (Z.byCurr5TK >= 15) //已经有15分以上，并且不能随意杀
			{
				if (Z.byTypeM == TYPE_SPADE)
				{
					if (_bitmap & TblContraryMask[Z.byGradeM])
						_bitmap &= TblContraryMask[Z.byGradeM];
				}
				byGrade = TblBitIndex[(_bitmap & -_bitmap) % 37];
				return TYPE_SPADE;
			}
		}

		//不适合用主牌杀
		for (byte byRspType = TYPE_HEART; byRspType != TYPE_ERROR; byRspType++)
		{
			if (!g_bEscapeCard[byRspType - 1])
				continue;

			word _bitmap = m_pRPlayer[0]->bitmap[byRspType - 1];

			//垫牌，即便是分也要垫
			byGrade = TblBitIndex[(_bitmap & -_bitmap) % 37];
			return byRspType;
		}
		for (byte byRspType = TYPE_HEART; byRspType != TYPE_ERROR; byRspType++)
		{
			word _bitmap = m_pRPlayer[0]->bitmap[byRspType - 1];
			if (_bitmap != 0)
			{
				byGrade = TblBitIndex[(_bitmap & -_bitmap) % 37];
				return byRspType;
			}
		}
	}

	//最后的选择
	byte byRspType = m_pRPlayer[0]->MakeCards(Z.byType, byGrade);
	return byRspType;
}

void CRCrack::InitScoreBitmap()
{
	memset(g_wScoreBitmap, 0, sizeof(g_wScoreBitmap));
	for (byte byRspType = TYPE_SPADE; byRspType != TYPE_ERROR; byRspType++)
	{
		word _bitmap = 0;
		for (int i = 0; i < 4; i++)
		{
			if (i == m_byBanker)
				continue;
			_bitmap |= m_pRPlayer[i]->bitmap[byRspType - 1];
		}
		word v = m_pRPlayer[m_byBanker]->bitmap[byRspType - 1];
		if (v & v - 1)
			_bitmap |= v;
		_bitmap &= MASK_5TK;
		if (_bitmap == 0) //这门花色没有分
			continue;
		g_wScoreBitmap[byRspType - 1] = _bitmap;
	}
}

byte CRCrack::PlayerFirstCard(byte &byGrade)
{
	//初始化分数位图
	InitScoreBitmap();

	//庄家没有主后：选择庄家没有的花色
	if (m_pRPlayer[m_byBanker]->bitmap[0] == 0)
	{
		for (byte byRspType = TYPE_HEART; byRspType != TYPE_ERROR; byRspType++)
		{
			if (m_pRPlayer[0]->bitmap[byRspType - 1] == 0)
				continue;
			if (m_pRPlayer[m_byBanker]->bitmap[byRspType - 1] != 0) //庄家有这个花色
				continue;

			word _bitmap = m_pRPlayer[0]->bitmap[byRspType - 1];

			//从最大的副牌开始
			byGrade = _bitmap & 0xFF00 ? TblMaxGrade[_bitmap >> 8] + 8 : TblMaxGrade[_bitmap];
			return byRspType;
		}
	}

	//首先选择朋友没有的花色
	for (byte byRspType = TYPE_SPADE; byRspType != TYPE_ERROR; byRspType++)
	{
		if (m_pRPlayer[0]->bitmap[byRspType - 1] == 0)
			continue;
		if (g_wScoreBitmap[byRspType - 1] == 0) //没有分可加
			continue;

		word _bitmap = m_pRPlayer[0]->bitmap[byRspType - 1];
		for (int i = 1; i < 4; i++)
		{
			if (i == m_byBanker)
				continue;
			if (m_pRPlayer[i]->bitmap[byRspType - 1] == 0)
			{
				word v2 = g_wScoreBitmap[byRspType - 1];
				v2 ^= m_pRPlayer[m_byBanker]->bitmap[byRspType - 1] & MASK_5TK; //去掉庄家的分数
				if ((_bitmap & MASK_5TK) == v2) //分全在自己这
				{
					if (v2 == 0)
						break;
					byGrade = TblBitIndex[(v2 & -v2) % 37];
				}
				else
				{
					//可扩展：分上家、下家不同处理
					byGrade = TblBitIndex[(_bitmap & -_bitmap) % 37];
				}
				return byRspType;
			}
		}
	}

	//上家特殊处理：选择庄家没有的花色，并且队友没有的花色
	if (m_byBanker == 1)
	{
		for (byte byRspType = TYPE_SPADE; byRspType != TYPE_ERROR; byRspType++)
		{
			if (m_pRPlayer[0]->bitmap[byRspType - 1] == 0)
				continue;
			if (m_pRPlayer[m_byBanker]->bitmap[byRspType - 1] != 0) //庄家有这个花色
				continue;

			//选择队友没有的花色
			word _bitmap = m_pRPlayer[0]->bitmap[byRspType - 1];
			for (int i = 1; i < 4; i++)
			{
				if (i == m_byBanker)
					continue;
				if (m_pRPlayer[i]->bitmap[byRspType - 1] == 0)
				{
					word v2 = g_wScoreBitmap[byRspType - 1];
					v2 ^= m_pRPlayer[m_byBanker]->bitmap[byRspType - 1] & MASK_5TK; //去掉庄家的分数
					if ((_bitmap & MASK_5TK) == v2) //分全在自己这
					{
						if (v2 == 0)
							byGrade = TblBitIndex[(_bitmap & -_bitmap) % 37]; //即使不是分也要出副牌
						else
							byGrade = TblBitIndex[(v2 & -v2) % 37];
					}
					else
					{
						//可扩展：分上家、下家不同处理
						byGrade = TblBitIndex[(_bitmap & -_bitmap) % 37];
					}
					return byRspType;
				}
			}
		}
	}

	//上家特殊处理：选择庄家没有的花色，队友全部都有此花色
	if (m_byBanker == 1)
	{
		for (byte byRspType = TYPE_SPADE; byRspType != TYPE_ERROR; byRspType++)
		{
			if (m_pRPlayer[0]->bitmap[byRspType - 1] == 0)
				continue;
			if (g_wScoreBitmap[byRspType - 1] == 0) //没有分可加
				continue;
			if (m_pRPlayer[m_byBanker]->bitmap[byRspType - 1] != 0) //庄家有这个花色
				continue;

			//队友全部都有此花色
			word _bitmap = m_pRPlayer[0]->bitmap[byRspType - 1];
			word v2 = g_wScoreBitmap[byRspType - 1];
			v2 ^= m_pRPlayer[m_byBanker]->bitmap[byRspType - 1] & MASK_5TK; //去掉庄家的分数
			if ((_bitmap & MASK_5TK) == v2) //分全在自己这
			{
				if (v2 == 0)
					break;
				byGrade = TblBitIndex[(v2 & -v2) % 37];
			}
			else
			{
				//可扩展：分上家、下家不同处理
				byGrade = TblBitIndex[(_bitmap & -_bitmap) % 37];
			}
			return byRspType;
		}
	}

	//对家特殊处理：选择庄家没有的花色 ，并且队友没有的花色
	if (m_byBanker == 2)
	{
		for (byte byRspType = TYPE_SPADE; byRspType != TYPE_ERROR; byRspType++)
		{
			if (m_pRPlayer[0]->bitmap[byRspType - 1] == 0)
				continue;
			if (m_pRPlayer[m_byBanker]->bitmap[byRspType - 1] != 0) //庄家有这个花色
				continue;

			//选择队友没有的花色
			word _bitmap = m_pRPlayer[0]->bitmap[byRspType - 1];
			for (int i = 1; i < 4; i++)
			{
				if (i == m_byBanker)
					continue;
				if (m_pRPlayer[i]->bitmap[byRspType - 1] == 0)
				{
					word v2 = g_wScoreBitmap[byRspType - 1];
					v2 ^= m_pRPlayer[m_byBanker]->bitmap[byRspType - 1] & MASK_5TK; //去掉庄家的分数
					if ((_bitmap & MASK_5TK) == v2) //分全在自己这
					{
						if (v2 == 0)
							byGrade = TblBitIndex[(_bitmap & -_bitmap) % 37]; //即使不是分也要出副牌
						else
							byGrade = TblBitIndex[(v2 & -v2) % 37];
					}
					else
					{
						//可扩展：分上家、下家不同处理
						byGrade = TblBitIndex[(_bitmap & -_bitmap) % 37];
					}
					return byRspType;
				}
			}
		}
	}

	//选择庄家有的花色：庄家唯一牌
	for (byte byRspType = TYPE_SPADE; byRspType != TYPE_ERROR; byRspType++)
	{
		if (m_pRPlayer[0]->bitmap[byRspType - 1] == 0)
			continue;
		if (g_wScoreBitmap[byRspType - 1] == 0) //没有分可加
			continue;
		word v1 = m_pRPlayer[m_byBanker]->bitmap[byRspType - 1];
		if (v1 == 0) //庄家没有这个花色
			continue;
		if ((v1 & v1 - 1) != 0) //庄家不是唯一牌
			continue;

		word _bitmap = m_pRPlayer[0]->bitmap[byRspType - 1];
		if (_bitmap > v1) //自己比庄家大
		{
			byGrade = _bitmap & 0xFF00 ? TblMaxGrade[_bitmap >> 8] + 8 : TblMaxGrade[_bitmap];
			return byRspType;
		}
		else
		{
			for (int i = 1; i < 4; i++)
			{
				if (i == m_byBanker)
					continue;
				if (m_pRPlayer[i]->bitmap[byRspType - 1] > v1) //朋友比庄家大
				{
					byGrade = _bitmap & 0xFF00 ? TblMaxGrade[_bitmap >> 8] + 8 : TblMaxGrade[_bitmap];
					return byRspType;
				}
			}
		}
	}

	//对家、下家特殊处理：选择庄家有的花色
	if (m_byBanker == 2 || m_byBanker == 3)
	{
		for (byte byRspType = TYPE_SPADE; byRspType != TYPE_ERROR; byRspType++)
		{
			if (m_pRPlayer[0]->bitmap[byRspType - 1] == 0)
				continue;
			if (g_wScoreBitmap[byRspType - 1] == 0) //没有分可加
				continue;
			if (m_pRPlayer[m_byBanker]->bitmap[byRspType - 1] == 0) //庄家没有这个花色
				continue;

			word _bitmap = m_pRPlayer[0]->bitmap[byRspType - 1];
			for (int i = 1; i < 4; i++)
			{
				if (i == m_byBanker)
					continue;
				if (m_pRPlayer[i]->bitmap[byRspType - 1] == 0)
				{
					word v2 = g_wScoreBitmap[byRspType - 1];
					v2 ^= m_pRPlayer[m_byBanker]->bitmap[byRspType - 1] & MASK_5TK; //去掉庄家的分数
					if ((_bitmap & MASK_5TK) == v2) //分全在自己这
					{
						if (v2 == 0)
							break;
						byGrade = TblBitIndex[(v2 & -v2) % 37];
					}
					else
					{
						//可扩展：分上家、下家不同处理
						byGrade = TblBitIndex[(_bitmap & -_bitmap) % 37];
					}
					return byRspType;
				}
			}
		}
	}

	//选择有分的花色
	for (byte byRspType = TYPE_SPADE; byRspType != TYPE_ERROR; byRspType++)
	{
		if (m_pRPlayer[0]->bitmap[byRspType - 1] == 0)
			continue;
		if (g_wScoreBitmap[byRspType - 1] == 0) //没有分可加
			continue;

		word _bitmap = m_pRPlayer[0]->bitmap[byRspType - 1];
		for (int i = 1; i < 4; i++)
		{
			if (i == m_byBanker)
				continue;
			if (m_pRPlayer[i]->bitmap[byRspType - 1] == 0)
			{
				word v2 = g_wScoreBitmap[byRspType - 1];
				v2 ^= m_pRPlayer[m_byBanker]->bitmap[byRspType - 1] & MASK_5TK; //去掉庄家的分数
				if ((_bitmap & MASK_5TK) == v2) //分全在自己这
				{
					if (v2 == 0)
						break;
					byGrade = TblBitIndex[(v2 & -v2) % 37];
				}
				else
				{
					//可扩展：分上家、下家不同处理
					byGrade = TblBitIndex[(_bitmap & -_bitmap) % 37];
				}
				return byRspType;
			}
		}
	}

	//最后的选择
	for (byte byRspType = TYPE_SPADE; byRspType != TYPE_ERROR; byRspType++)
	{
		if (m_pRPlayer[0]->bitmap[byRspType - 1] == 0)
			continue;

		word _bitmap = m_pRPlayer[0]->bitmap[byRspType - 1];
		byGrade = TblBitIndex[(_bitmap & -_bitmap) % 37];
		return byRspType;
	}
	return TYPE_GVP;
}

byte CRCrack::PlayerFollowCard(byte &byGrade)
{
	byte byRspType;
	if (g_byChairM0123 == m_byBanker) //四家相比庄家大
	{
		if (Z.byChair1 <= m_byBanker) //庄家已经出过
		{
			word wBitmapBak[4];
			memcpy(wBitmapBak, m_pRPlayer[0]->bitmap, sizeof(word [4]));

			//只垫比庄家小的副牌
			for (byte bySuit = TYPE_HEART; bySuit != TYPE_ERROR; bySuit++)
			{
				word _banker = m_pRPlayer[m_byBanker]->bitmap[bySuit - 1];
				byte _bankergrade = _banker & 0xFF00 ? TblMaxGrade[_banker >> 8] + 8 : TblMaxGrade[_banker];
				m_pRPlayer[0]->bitmap[bySuit - 1] &= (1 << _bankergrade) - 1;
			}

			//不加分，垫牌
			byRspType = m_pRPlayer[0]->MakeWithoutScore(Z.byType, byGrade);
			if (byRspType == TYPE_GVP)
				byRspType = m_pRPlayer[0]->MakeByLittle(Z.byType, byGrade);

			memcpy(m_pRPlayer[0]->bitmap, wBitmapBak, sizeof(word [4])); //还原

			//放弃或者错误的垫牌选择
			if (byRspType == TYPE_GVP
				|| m_pRPlayer[0]->bitmap[Z.byType - 1] != 0 && byRspType != Z.byType)
			{
				//不加分，垫牌
				byRspType = m_pRPlayer[0]->MakeWithoutScore(Z.byType, byGrade);
				if (byRspType == TYPE_GVP)
					byRspType = m_pRPlayer[0]->MakeByLittle(Z.byType, byGrade);
			}
		}
		else
		{
			if (!m_pRPlayer[m_byBanker]->HasCard(Z.byType) //庄家需要出杀
				&& g_byFriendScore == 0) //队友无分
			{
				//加分逼杀
				byRspType = m_pRPlayer[0]->MakeWithScore(Z.byType, byGrade);
				if (byRspType == TYPE_GVP)
					byRspType = m_pRPlayer[0]->MakeByLittle(Z.byType, byGrade);
			}
			else
			{
				word wBitmapBak[4];
				memcpy(wBitmapBak, m_pRPlayer[0]->bitmap, sizeof(word [4]));

				//只垫比庄家小的副牌
				for (byte bySuit = TYPE_HEART; bySuit != TYPE_ERROR; bySuit++)
				{
					word _banker = m_pRPlayer[m_byBanker]->bitmap[bySuit - 1];
					byte _bankergrade = _banker & 0xFF00 ? TblMaxGrade[_banker >> 8] + 8 : TblMaxGrade[_banker];
					m_pRPlayer[0]->bitmap[bySuit - 1] &= (1 << _bankergrade) - 1;
				}

				//不加分，垫牌
				byRspType = m_pRPlayer[0]->MakeWithoutScore(Z.byType, byGrade);
				if (byRspType == TYPE_GVP)
					byRspType = m_pRPlayer[0]->MakeByLittle(Z.byType, byGrade);

				memcpy(m_pRPlayer[0]->bitmap, wBitmapBak, sizeof(word [4])); //还原

				//放弃或者错误的垫牌选择
				if (byRspType == TYPE_GVP
					|| m_pRPlayer[0]->bitmap[Z.byType - 1] != 0 && byRspType != Z.byType)
				{
					//不加分，垫牌
					byRspType = m_pRPlayer[0]->MakeWithoutScore(Z.byType, byGrade);
					if (byRspType == TYPE_GVP)
						byRspType = m_pRPlayer[0]->MakeByLittle(Z.byType, byGrade);
				}
			}
		}
	}
	else
	{
		if (Z.byChair1 > m_byBanker //庄家还没有出过
			|| Z.byChairM == m_byBanker && g_bICanOverBanker && g_byFriendScore != 0) //自己能比庄家大，队友能加分
		{
			if (g_bHadOverBanker || g_bOverBankerWithScore) //队友加分的牌比庄家大
			{
				//加分
				byRspType = m_pRPlayer[0]->MakeWithScore(Z.byType, byGrade);
				if (byRspType == TYPE_GVP)
					byRspType = m_pRPlayer[0]->MakeByLittle(Z.byType, byGrade);
			}
			else
			{
				//出比庄家大的牌
				byte byTypeM = Z.byChair1 > m_byBanker ? g_tMaxGrade[m_byBanker].byType : Z.byTypeM;
				byte byGradeM = Z.byChair1 > m_byBanker ? g_tMaxGrade[m_byBanker].byGrade : Z.byGradeM;

				byRspType = m_pRPlayer[0]->MakeJustOver(Z.byType, byGrade, byTypeM, byGradeM);

				//大不过，出小牌
				if (byRspType == TYPE_GVP)
					byRspType = m_pRPlayer[0]->MakeByLittle(Z.byType, byGrade);
			}
		}
		else
		{
			//加分
			byRspType = m_pRPlayer[0]->MakeWithScore(Z.byType, byGrade);
			if (byRspType == TYPE_GVP)
				byRspType = m_pRPlayer[0]->MakeByLittle(Z.byType, byGrade);
		}
	}
	return byRspType;
}

void CRCrack::Think()
{
	//从保存的参数恢复
	RestoreWith(m_tCrackParam);

	//初始化思考数据
	InitMaxGrade();
	InitMaxScore();
	InitOverBanker();

	//类人思考后出牌
	byte byRspType;
	byte byGrade;
	if (m_byBanker == 0)
	{
		byRspType = Z.byChair1 == 0 ? BankerFirstCard(byGrade)
									: BankerFollowCard(byGrade);
	}
	else
	{
		byRspType = Z.byChair1 == 0 ? PlayerFirstCard(byGrade)
									: PlayerFollowCard(byGrade);
	}

	//保存在结果中
	m_tAction[1].byRspType = byRspType;
	m_tAction[1].byGrade = byGrade;
}
