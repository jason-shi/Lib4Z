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
		//���Թ�����
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
		//���Թ�����
		byGrade = GRADE_INIT;
		byCount = 0;
		if (_MakeAnyPair(byGrade, byCount) != TYPE_GVP)
		{
			if (byGrade <= 9)
				return TYPE_PAIR;
		}
	}

	//------------------ս�Է���
	byte byRtnType;
	byRtnType = _MakeGvpRound1(byGrade, byCount, wAttach); CHECK_RETURN(byRtnType);
	byRtnType = _MakeGvpRound2(byGrade, byCount, wAttach); CHECK_RETURN(byRtnType);
	byRtnType = _MakeGvpRound3(byGrade, byCount, wAttach); CHECK_RETURN(byRtnType);

	//���Գ������
	byGrade = GRADE_INIT;
	byCount = 0;
	if (_MakeAnyPair(byGrade, byCount) != TYPE_GVP)
		return TYPE_PAIR;
	//���Գ�����
	byGrade = GRADE_INIT;
	byCount = 0;
	if (_MakeAnySingle(byGrade, byCount) != TYPE_GVP)
	{
		if (byGrade <= 12) //3-2
			return TYPE_SINGLE;
	}
	//���Գ�����
	byGrade = GRADE_INIT;
	byCount = 0;
	if (_MakeTrip(byGrade, byCount) != TYPE_GVP)
		return TYPE_TRIP;
	//���Գ�ը��
	byGrade = GRADE_INIT;
	byCount = 0;
	if (_MakeBomb(byGrade, byCount) != TYPE_GVP)
		return TYPE_BOMB;
	//���Գ����
	if (_MakeRocket(byGrade, byCount) != TYPE_GVP)
		return TYPE_ROCKET;

	//------------------��ͷת��
	return CRPlayer::_MakeGvp(byGrade, byCount, wAttach);
}

byte CRPlayerSJ::_MakeSingle(byte &byGrade, byte &byCount)
{
	//�Լ�û�л�ʤ���࣬�Լ��л�ʤ���࣬�ƽϴ�ʱ������
	if (ReleasePlayer(m_byChair1) || (m_byFriend & (1 << m_byChair1)) != 0 && byGrade >= 9 && byGrade != GRADE_INIT)
		return TYPE_GVP;

	//����ѹ����
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
	//�Լ�û�л�ʤ���࣬�Լ��л�ʤ���࣬�ƽϴ�ʱ������
	if (ReleasePlayer(m_byChair1) || (m_byFriend & (1 << m_byChair1)) != 0 && byGrade >= 9 && byGrade != GRADE_INIT)
		return TYPE_GVP;

	//����ѹ���ӣ�K..A
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
	//���Գ�С˳��
	byGrade = GRADE_INIT;
	byCount = 0;
	if (_MakeAnyShunZi(byGrade, byCount) != TYPE_GVP)
	{
		if (byGrade + byCount - 1 <= 7) //3-10
			return TYPE_SHUNZI;
	}
	//���Գ�С����
	byGrade = GRADE_INIT;
	byCount = 0;
	if (_MakeAnyLianDui(byGrade, byCount) != TYPE_GVP)
	{
		if (byGrade + byCount <= 6) //3-9
			return TYPE_LIANDUI;
	}
	//���Գ�С����һ
	byGrade = GRADE_INIT;
	byCount = 0;
	if (_MakeTripS(byGrade, byCount, wAttach) != TYPE_GVP)
	{
		if (byGrade <= 6) //3-9
			return TYPE_TRIP_S;
	}
	//���Գ�С������
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
	//���Գ���˳��
	byGrade = GRADE_INIT;
	byCount = 0;
	if (_MakeAnyShunZi(byGrade, byCount) != TYPE_GVP)
	{
		if (byGrade + byCount - 1 <= 10) //3-K
			return TYPE_SHUNZI;
	}
	//���Գ�������
	byGrade = GRADE_INIT;
	byCount = 0;
	if (_MakeAnyLianDui(byGrade, byCount) != TYPE_GVP)
	{
		if (byGrade + byCount <= 9) //3-Q
			return TYPE_LIANDUI;
	}
	//���Գ�������һ
	byGrade = GRADE_INIT;
	byCount = 0;
	if (_MakeTripS(byGrade, byCount, wAttach) != TYPE_GVP)
	{
		if (byGrade <= 9) //3-Q
			return TYPE_TRIP_S;
	}
	//���Գ���������
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
	//���Գ���˳��
	byGrade = GRADE_INIT;
	byCount = 0;
	if (_MakeAnyShunZi(byGrade, byCount) != TYPE_GVP)
		return TYPE_SHUNZI;
	//���Գ�������
	byGrade = GRADE_INIT;
	byCount = 0;
	if (_MakeAnyLianDui(byGrade, byCount) != TYPE_GVP)
		return TYPE_LIANDUI;
	//���Գ�������һ
	byGrade = GRADE_INIT;
	byCount = 0;
	if (_MakeTripS(byGrade, byCount, wAttach) != TYPE_GVP)
		return TYPE_TRIP_S;
	//���Գ���������
	byGrade = GRADE_INIT;
	byCount = 0;
	if (_MakeTripP(byGrade, byCount, wAttach) != TYPE_GVP)
		return TYPE_TRIP_P;

	return TYPE_GVP;
}
