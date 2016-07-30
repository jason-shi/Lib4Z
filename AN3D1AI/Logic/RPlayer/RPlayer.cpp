#include "StdAfx.h"
#include "RPlayer.h"

CRPlayer::CRPlayer()
{
}

CRPlayer::~CRPlayer()
{
}

void CRPlayer::RestoreWith(byte byIndex, const TCrackParam& tCrackParam)
{
	static byte TFriend[4] = { 0x00, 0x0C, 0x06, 0x0A, };

	memcpy(bitmap, tCrackParam.tPlayerParam[byIndex].bitmap, sizeof(word [4]));
	m_byFuTwo = tCrackParam.tPlayerParam[byIndex].byFuTwo;

	m_byFriend = TFriend[tCrackParam.byBanker];
	m_byChair1 = tCrackParam.tBattleParam.byChair1;
}

void CRPlayer::OutCards(byte byType, byte byGrade)
{
	bitmap[byType - 1] ^= 1 << byGrade;
	m_byFuTwo -= byGrade == 12;
}

void CRPlayer::InCards(byte byType, byte byGrade)
{
	bitmap[byType - 1] |= 1 << byGrade;
	m_byFuTwo += byGrade == 12;
}

byte CRPlayer::GetMaxGrade(byte byType, byte &byGrade)
{
	word _bitmap = bitmap[byType - 1];
	if (_bitmap != 0) //同花色
	{
		byGrade = _bitmap & 0xFF00 ? TblMaxGrade[_bitmap >> 8] + 8 : TblMaxGrade[_bitmap];
		return byType;
	}
	else
	{
		_bitmap = bitmap[0];
		if (_bitmap != 0) //用主杀
		{
			byGrade = _bitmap & 0xFF00 ? TblMaxGrade[_bitmap >> 8] + 8 : TblMaxGrade[_bitmap];
			return TYPE_SPADE;
		}
		else //垫副牌
		{
			byGrade = 0;
			return TYPE_GVP;
		}
	}
}

byte CRPlayer::GetMaxScore(byte byType, byte &byGrade, byte &byScore)
{
	word _bitmap = bitmap[byType - 1];
	if (_bitmap != 0) //同花色
	{
		if ((_bitmap & MASK_5TK) == 0)
		{
			byGrade = 0;
			byScore = 0;
			return TYPE_GVP;
		}

		if (_bitmap & MASK_K)
		{
			byGrade = 10;
			byScore = 10;
		}
		else if (_bitmap & MASK_T)
		{
			byGrade = 7;
			byScore = 10;
		}
		else
		{
			byGrade = 2;
			byScore = 5;
		}
		return byType;
	}
	else
	{
		for (byte byRspType = TYPE_SPADE; byRspType != TYPE_ERROR; byRspType++)
		{
			_bitmap = bitmap[byRspType - 1];
			if ((_bitmap & MASK_5TK) == 0)
				continue;

			if (_bitmap & MASK_K)
			{
				byGrade = 10;
				byScore = 10;
			}
			else if (_bitmap & MASK_T)
			{
				byGrade = 7;
				byScore = 10;
			}
			else
			{
				byGrade = 2;
				byScore = 5;
			}
			return byRspType;
		}
		byGrade = 0;
		byScore = 0;
		return TYPE_GVP;
	}
}
