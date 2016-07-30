#include "StdAfx.h"
#include "RPlayerSJ.h"

CRPlayerSJ::CRPlayerSJ() : CRPlayer()
{
}

CRPlayerSJ::~CRPlayerSJ()
{
}

byte CRPlayerSJ::_MakeGvp(byte &byGrade, byte &byCount, word &wAttach)
{
	if (m_bSinglePhobia)
	{
		//尝试攻单张
		byGrade = GRADE_INIT;
		byCount = 0;
		if (_MakeAnySingle(byGrade, byCount) != TYPE_GVP)
		{
			if (byGrade <= 11)
				return TYPE_SINGLE;
		}
	}
	if (m_bPairPhobia)
	{
		//尝试攻对子
		byGrade = GRADE_INIT;
		byCount = 0;
		if (_MakeAnyPair(byGrade, byCount) != TYPE_GVP)
		{
			if (byGrade <= 9)
				return TYPE_PAIR;
		}
	}

	//------------------战略防御
	byte byRtnType;
	byRtnType = _MakeGvpRound1(byGrade, byCount, wAttach); CHECK_RETURN(byRtnType);
	byRtnType = _MakeGvpRound2(byGrade, byCount, wAttach); CHECK_RETURN(byRtnType);
	byRtnType = _MakeGvpRound3(byGrade, byCount, wAttach); CHECK_RETURN(byRtnType);

	//尝试出大对子
	byGrade = GRADE_INIT;
	byCount = 0;
	if (_MakeAnyPair(byGrade, byCount) != TYPE_GVP)
		return TYPE_PAIR;
	//尝试出大单张
	byGrade = GRADE_INIT;
	byCount = 0;
	if (_MakeAnySingle(byGrade, byCount) != TYPE_GVP)
	{
		if (byGrade <= 12) //3-2
			return TYPE_SINGLE;
	}
	//尝试出三张
	byGrade = GRADE_INIT;
	byCount = 0;
	if (_MakeTrip(byGrade, byCount) != TYPE_GVP)
		return TYPE_TRIP;
	//尝试出炸弹
	byGrade = GRADE_INIT;
	byCount = 0;
	if (_MakeBomb(byGrade, byCount) != TYPE_GVP)
		return TYPE_BOMB;
	//尝试出火箭
	if (_MakeRocket(byGrade, byCount) != TYPE_GVP)
		return TYPE_ROCKET;

	//------------------晕头转向
	return CRPlayer::_MakeGvp(byGrade, byCount, wAttach);
}

byte CRPlayerSJ::_MakeSingle(byte &byGrade, byte &byCount)
{
	//自己没有获胜节奏，对家有获胜节奏，牌较大时，放弃
	if (ReleasePlayer(m_byChair1) || (m_byFriend & (1 << m_byChair1)) != 0 && byGrade >= 9 && byGrade != GRADE_INIT)
		return TYPE_GVP;

	//尝试压单张
	byte byTempGrade = byGrade;
	if (CRPlayer::_MakeSingle(byTempGrade, byCount) != TYPE_GVP)
	{
		if (byTempGrade <= 11)
		{
			byGrade = byTempGrade;
			return TYPE_SINGLE;
		}
	}

	byTempGrade = byGrade;
	return CRPlayer::_MakeSingle(byGrade, byCount);
}

byte CRPlayerSJ::_MakePair(byte &byGrade, byte &byCount)
{
	//自己没有获胜节奏，对家有获胜节奏，牌较大时，放弃
	if (ReleasePlayer(m_byChair1) || (m_byFriend & (1 << m_byChair1)) != 0 && byGrade >= 9 && byGrade != GRADE_INIT)
		return TYPE_GVP;

	//尝试压对子：K..A
	byte byTempGrade = byGrade;
	if (CRPlayer::_MakePair(byTempGrade, byCount) != TYPE_GVP)
	{
		if (byTempGrade <= 11)
		{
			byGrade = byTempGrade;
			return TYPE_PAIR;
		}
	}

	byTempGrade = byGrade;
	return CRPlayer::_MakePair(byGrade, byCount);
}

byte CRPlayerSJ::_MakeAnySingle(byte &byGrade, byte &byCount)
{
	return CRPlayer::_MakeAnySingle(byGrade, byCount);
}

byte CRPlayerSJ::_MakeAnyPair(byte &byGrade, byte &byCount)
{
	return CRPlayer::_MakeAnyPair(byGrade, byCount);
}

byte CRPlayerSJ::_MakeGvpRound1(byte &byGrade, byte &byCount, word &wAttach)
{
	//尝试出小顺子
	byGrade = GRADE_INIT;
	byCount = 0;
	if (_MakeAnyShunZi(byGrade, byCount) != TYPE_GVP)
	{
		if (byGrade + byCount - 1 <= 7) //3-10
			return TYPE_SHUNZI;
	}
	//尝试出小连对
	byGrade = GRADE_INIT;
	byCount = 0;
	if (_MakeAnyLianDui(byGrade, byCount) != TYPE_GVP)
	{
		if (byGrade + byCount <= 6) //3-9
			return TYPE_LIANDUI;
	}
	//尝试出小三带一
	byGrade = GRADE_INIT;
	byCount = 0;
	if (_MakeTripS(byGrade, byCount, wAttach) != TYPE_GVP)
	{
		if (byGrade <= 6) //3-9
			return TYPE_TRIP_S;
	}
	//尝试出小三带二
	byGrade = GRADE_INIT;
	byCount = 0;
	if (_MakeTripP(byGrade, byCount, wAttach) != TYPE_GVP)
	{
		if (byGrade <= 6) //3-9
			return TYPE_TRIP_P;
	}

	return TYPE_GVP;
}

byte CRPlayerSJ::_MakeGvpRound2(byte &byGrade, byte &byCount, word &wAttach)
{
	//尝试出中顺子
	byGrade = GRADE_INIT;
	byCount = 0;
	if (_MakeAnyShunZi(byGrade, byCount) != TYPE_GVP)
	{
		if (byGrade + byCount - 1 <= 10) //3-K
			return TYPE_SHUNZI;
	}
	//尝试出中连对
	byGrade = GRADE_INIT;
	byCount = 0;
	if (_MakeAnyLianDui(byGrade, byCount) != TYPE_GVP)
	{
		if (byGrade + byCount <= 9) //3-Q
			return TYPE_LIANDUI;
	}
	//尝试出中三带一
	byGrade = GRADE_INIT;
	byCount = 0;
	if (_MakeTripS(byGrade, byCount, wAttach) != TYPE_GVP)
	{
		if (byGrade <= 9) //3-Q
			return TYPE_TRIP_S;
	}
	//尝试出中三带二
	byGrade = GRADE_INIT;
	byCount = 0;
	if (_MakeTripP(byGrade, byCount, wAttach) != TYPE_GVP)
	{
		if (byGrade <= 9) //3-Q
			return TYPE_TRIP_P;
	}

	return TYPE_GVP;
}

byte CRPlayerSJ::_MakeGvpRound3(byte &byGrade, byte &byCount, word &wAttach)
{
	//尝试出大顺子
	byGrade = GRADE_INIT;
	byCount = 0;
	if (_MakeAnyShunZi(byGrade, byCount) != TYPE_GVP)
		return TYPE_SHUNZI;
	//尝试出大连对
	byGrade = GRADE_INIT;
	byCount = 0;
	if (_MakeAnyLianDui(byGrade, byCount) != TYPE_GVP)
		return TYPE_LIANDUI;
	//尝试出大三带一
	byGrade = GRADE_INIT;
	byCount = 0;
	if (_MakeTripS(byGrade, byCount, wAttach) != TYPE_GVP)
		return TYPE_TRIP_S;
	//尝试出大三带二
	byGrade = GRADE_INIT;
	byCount = 0;
	if (_MakeTripP(byGrade, byCount, wAttach) != TYPE_GVP)
		return TYPE_TRIP_P;

	return TYPE_GVP;
}
