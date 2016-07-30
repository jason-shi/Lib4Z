#include "StdAfx.h"
#include "RPlayerDZ.h"

CRPlayerDZ::CRPlayerDZ() : CRPlayer()
{
}

CRPlayerDZ::~CRPlayerDZ()
{
}

byte CRPlayerDZ::_MakeGvp(byte &byGrade, byte &byCount, word &wAttach)
{
	if (m_nOriginVA <= -2 && WORD_BIT_COUNT(bitmap[2] | bitmap[3]) <= 2) //���������ƻ����
	{
		//���Թ�����
		byGrade = GRADE_INIT;
		byCount = 0;
		if (_MakeAnySingle(byGrade, byCount) != TYPE_GVP)
		{
			if (byGrade <= 6)
				return TYPE_SINGLE;
		}
		//���Թ�����
		byGrade = GRADE_INIT;
		byCount = 0;
		if (_MakeAnyPair(byGrade, byCount) != TYPE_GVP)
		{
			if (byGrade <= 6)
				return TYPE_PAIR;
		}
		//���Թ�����
		byGrade = GRADE_INIT;
		byCount = 0;
		if (_MakeAnySingle(byGrade, byCount) != TYPE_GVP)
		{
			if (byGrade <= 10)
				return TYPE_SINGLE;
		}
		//���Թ�����
		byGrade = GRADE_INIT;
		byCount = 0;
		if (_MakeAnyPair(byGrade, byCount) != TYPE_GVP)
		{
			if (byGrade <= 10)
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

byte CRPlayerDZ::_MakeSingle(byte &byGrade, byte &byCount)
{
	byte byTempGrade = byGrade;
	if (CRPlayer::_MakeSingle(byTempGrade, byCount) != TYPE_GVP)
	{
		byGrade = byTempGrade;
		return TYPE_SINGLE;
	}

	word _bitmap = bitmap[0] | bitmap[1] | bitmap[2] | bitmap[3];
	_bitmap &= TblContraryMask[byGrade];
	while (_bitmap)
	{
		byGrade = TblBitIndex[(_bitmap & -_bitmap) % 37];
		if (IsStupidPut(TYPE_SINGLE, byGrade, byCount))
		{
			_bitmap &= TblContraryMask[byGrade];
			continue;
		}
		return TYPE_SINGLE;
	}
	byGrade = GRADE_INIT;
	byCount = 0;
	return TYPE_GVP;
}

byte CRPlayerDZ::_MakePair(byte &byGrade, byte &byCount)
{
	byte byTempGrade = byGrade;
	if (CRPlayer::_MakePair(byTempGrade, byCount) != TYPE_GVP)
	{
		byGrade = byTempGrade;
		return TYPE_PAIR;
	}

	word _bitmap = bitmap[1] | bitmap[2] | bitmap[3];
	_bitmap &= TblContraryMask[byGrade];
	while (_bitmap)
	{
		byGrade = TblBitIndex[(_bitmap & -_bitmap) % 37];
		if (IsStupidPut(TYPE_PAIR, byGrade, byCount))
		{
			_bitmap &= TblContraryMask[byGrade];
			continue;
		}
		return TYPE_PAIR;
	}
	byGrade = GRADE_INIT;
	byCount = 0;
	return TYPE_GVP;
}

byte CRPlayerDZ::_MakeAnySingle(byte &byGrade, byte &byCount)
{
	return CRPlayer::_MakeAnySingle(byGrade, byCount);
}

byte CRPlayerDZ::_MakeAnyPair(byte &byGrade, byte &byCount)
{
	return CRPlayer::_MakeAnyPair(byGrade, byCount);
}

byte CRPlayerDZ::_MakeGvpRound1(byte &byGrade, byte &byCount, word &wAttach)
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

byte CRPlayerDZ::_MakeGvpRound2(byte &byGrade, byte &byCount, word &wAttach)
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

byte CRPlayerDZ::_MakeGvpRound3(byte &byGrade, byte &byCount, word &wAttach)
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
