#include "StdAfx.h"
#include "RPlayer.h"

using namespace std;

#define VA_ROCKET				0
#define VA_REDFLAG				0
#define VA_CANNON				0
#define VA_BOMB					0
#define VA_LIANDUI				0
#define VA_SHUNZI				-20
#define VA_BIG_SINGLE			-10
#define VA_SMALL_SINGLE			-30
#define VA_BIG_PAIR				-10
#define VA_SMALL_PAIR			-20

struct TSplit {
	int				 nVA;
	byte			 byType;
	byte			 byGrade;
	byte			 byCount;

	TSplit(int _nVA, byte _byType, byte _byGrade, byte _byCount)
	{
		nVA = _nVA;
		byType = _byType;
		byGrade = _byGrade;
		byCount = _byCount;
	}
};

static std::vector<TSplit>	 g_vecSplit;
static int					 g_nVA;

int CRPlayer::SplitCards()
{
	byte byType = TYPE_GVP;
	byte byGrade = GRADE_INIT;
	byte byCount = 0;

	g_vecSplit.clear();
	g_nVA = 0;
	int nMaxVA = -10000;
	while (true)
	{
		//尝试拆牌
		if (_SplitCards(byType, byGrade, byCount) != TYPE_GVP)
			continue;

		//计算残余牌的价值
		int nVA = _CalcRemainVA();
		if (g_nVA + nVA >= nMaxVA)
			nMaxVA = g_nVA + nVA;

		if (g_vecSplit.empty())
			break;

		//撤消拆牌
		_RevSplitCards(byType, byGrade, byCount);
	}
	return nMaxVA;
}

void CRPlayer::ValueAnalysis()
{
}

byte CRPlayer::_SplitCards(byte byType, byte &byGrade, byte &byCount)
{
	byte byRtnType = TYPE_GVP;
	switch (byType)
	{
	case TYPE_GVP:
	case TYPE_BOMB:
		byRtnType = _SplitBomb(byGrade, byCount); CHECK_RETURN(byRtnType);
	case TYPE_CANNON:
		byRtnType = _SplitCannon(byGrade, byCount); CHECK_RETURN(byRtnType);
		byRtnType = _SplitRedFlag(byGrade, byCount); CHECK_RETURN(byRtnType);
	case TYPE_REDFLAG:
		byRtnType = _SplitRocket(byGrade, byCount); CHECK_RETURN(byRtnType);
	case TYPE_ROCKET:
	case TYPE_SHUNZI:
		byRtnType = _SplitShunZi(byGrade, byCount); CHECK_RETURN(byRtnType);
	case TYPE_LIANDUI:
		byRtnType = _SplitLianDui(byGrade, byCount); CHECK_RETURN(byRtnType);
	}
	return TYPE_GVP;
}

byte CRPlayer::_SplitBomb(byte &byGrade, byte &byCount)
{
	word _bitmap = bitmap[2] | bitmap[3];
	_bitmap &= TblContraryMask[byGrade];
	if (_bitmap)
	{
		byGrade = TblBitIndex[(_bitmap & -_bitmap) % 37];

		OutCards(TYPE_BOMB, byGrade, 0);
		g_nVA += VA_BOMB;
		g_vecSplit.push_back(TSplit(VA_BOMB, TYPE_BOMB, byGrade, 0));
		return TYPE_BOMB;
	}
	byGrade = GRADE_INIT;
	byCount = 0;
	return TYPE_GVP;
}

byte CRPlayer::_SplitCannon(byte &byGrade, byte &byCount)
{
	word _bitmap = bitmap[3];
	_bitmap &= TblContraryMask[byGrade];
	if (_bitmap)
	{
		byGrade = TblBitIndex[(_bitmap & -_bitmap) % 37];

		OutCards(TYPE_CANNON, byGrade, 0);
		g_nVA += VA_CANNON;
		g_vecSplit.push_back(TSplit(VA_CANNON, TYPE_CANNON, byGrade, 0));
		return TYPE_CANNON;
	}
	byGrade = GRADE_INIT;
	byCount = 0;
	return TYPE_GVP;
}

byte CRPlayer::_SplitRedFlag(byte &byGrade, byte &byCount)
{
	if ((bitmap[0] & MASK_REDFLAG) == MASK_REDFLAG)
	{
		OutCards(TYPE_REDFLAG, byGrade, 0);
		g_nVA += VA_REDFLAG;
		g_vecSplit.push_back(TSplit(VA_REDFLAG, TYPE_REDFLAG, 0, 0));
		return TYPE_REDFLAG;
	}
	byGrade = GRADE_INIT;
	byCount = 0;
	return TYPE_GVP;
}

byte CRPlayer::_SplitRocket(byte &byGrade, byte &byCount)
{
	if (HasRocket())
	{
		OutCards(TYPE_ROCKET, byGrade, 0);
		g_nVA += VA_ROCKET;
		g_vecSplit.push_back(TSplit(VA_ROCKET, TYPE_ROCKET, 0, 0));
		return TYPE_ROCKET;
	}
	byGrade = GRADE_INIT;
	byCount = 0;
	return TYPE_GVP;
}

byte CRPlayer::_SplitShunZi(byte &byGrade, byte &byCount)
{
	word _bitmap = bitmap[0] | bitmap[1] | bitmap[2] | bitmap[3];
	_bitmap &= MASK_SHUNZI;
	if (byCount == 0) //第一次计算顺子
		byCount = 12;
	for ( ; byCount >= 3; byCount--)
	{
		word __bitmap = _bitmap;
		__bitmap &= TblContraryMask[byGrade];
		word mask = TblShunZi[__bitmap];
		if (WORD_BIT_COUNT(mask) >= byCount)
		{
			byGrade = TblBitIndex[(mask & -mask) % 37];

			OutCards(TYPE_SHUNZI, byGrade, byCount);
			g_nVA += VA_SHUNZI;
			g_vecSplit.push_back(TSplit(VA_SHUNZI, TYPE_SHUNZI, byGrade, byCount));
			return TYPE_SHUNZI;
		}
		byGrade = GRADE_INIT; //没有足够张数的顺子
	}
	byGrade = GRADE_INIT;
	byCount = 0;
	return TYPE_GVP;
}

byte CRPlayer::_SplitLianDui(byte &byGrade, byte &byCount)
{
	word _bitmap = bitmap[1] | bitmap[2] | bitmap[3];
	_bitmap &= MASK_SHUNZI;
	if (byCount == 0) //第一次计算连对
		byCount = 12;
	for ( ; byCount >= 3; byCount--)
	{
		word __bitmap = _bitmap;
		__bitmap &= TblContraryMask[byGrade];
		word mask = TblShunZi[__bitmap];
		if (WORD_BIT_COUNT(mask) >= byCount)
		{
			byGrade = TblBitIndex[(mask & -mask) % 37];

			OutCards(TYPE_LIANDUI, byGrade, byCount);
			g_nVA += VA_LIANDUI;
			g_vecSplit.push_back(TSplit(VA_LIANDUI, TYPE_LIANDUI, byGrade, byCount));
			return TYPE_LIANDUI;
		}
		byGrade = GRADE_INIT; //没有足够张数的连对
	}
	byGrade = GRADE_INIT;
	byCount = 0;
	return TYPE_GVP;
}

void CRPlayer::_RevSplitCards(byte &byType, byte &byGrade, byte &byCount)
{
	TSplit &tSplit = g_vecSplit.back();
	g_nVA -= tSplit.nVA;
	byType = tSplit.byType;
	byGrade = tSplit.byGrade;
	byCount = tSplit.byCount;
	g_vecSplit.pop_back();

	InCards(byType, byGrade, byCount);
}

int CRPlayer::_CalcRemainVA()
{
	//计算单张的价值
	int nVA1 = TblBitCount[bitmap[0] >> 12] * VA_BIG_SINGLE
			 + WORD_BIT_COUNT(bitmap[0] & 0x0FFF) * VA_SMALL_SINGLE;

	//计算对子的价值
	int nVA2 = TblBitCount[bitmap[1] >> 11] * VA_BIG_PAIR
			 + WORD_BIT_COUNT(bitmap[1] & 0x1FFF) * VA_SMALL_PAIR;

	return nVA1 + nVA2;
}
