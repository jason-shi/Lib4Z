#include "StdAfx.h"
#include "RPlayerXJ.h"

CRPlayerXJ::CRPlayerXJ() : CRPlayer()
{
}

CRPlayerXJ::~CRPlayerXJ()
{
}

byte CRPlayerXJ::_MakeGvp(byte &byGrade, byte &byCount, word &wAttach)
{
	if (m_bSinglePhobia)
	{
		//���Թ����ţ�3..Q
		byGrade = GRADE_INIT;
		byCount = 0;
		if (_MakeAnySingle(byGrade, byCount) != TYPE_GVP)
		{
			if (byGrade <= 9)
				return TYPE_SINGLE;
		}
	}
	if (m_bPairPhobia)
	{
		//���Թ����ӣ�3..Q
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

byte CRPlayerXJ::_MakeSingle(byte &byGrade, byte &byCount)
{
	//�Լ�û�л�ʤ���࣬�Լ��л�ʤ����
	if (ReleasePlayer(m_byChair1))
		return TYPE_GVP;

	if ((m_byFriend & (1 << m_byChair1)) == 0)
	{
		if (byGrade >= 6)
		{
			//ֱ����ѹ��K..2����Ȼ���С����
			byte byTempGrade = 9 < byGrade ? byGrade : 9;
			if (CRPlayer::_MakeSingle(byTempGrade, byCount) != TYPE_GVP)
			{
				if (byTempGrade <= 12)
				{
					byGrade = byTempGrade;
					return TYPE_SINGLE;
				}
			}
		}
		//������Ӧ��
		byte byTempGrade = byGrade;
		if (CRPlayer::_MakeSingle(byTempGrade, byCount) != TYPE_GVP)
		{
			if (byTempGrade <= 6 || (m_byWinRhythm & 1) != 0) //˳�ƣ������Լ���ʤ������
			{
				byGrade = byTempGrade;
				return TYPE_SINGLE;
			}
		}
		if (byGrade >= 9) //�����Ĵ��ƣ�Ҫ��ѹ
		{
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
		}
	}

	byGrade = GRADE_INIT;
	byCount = 0;
	return TYPE_GVP;
}

byte CRPlayerXJ::_MakePair(byte &byGrade, byte &byCount)
{
	//�Լ�û�л�ʤ���࣬�Լ��л�ʤ���࣬�ƽϴ�ʱ������
	if (ReleasePlayer(m_byChair1) || (m_byFriend & (1 << m_byChair1)) != 0 && byGrade >= 9 && byGrade != GRADE_INIT)
		return TYPE_GVP;

	if (byGrade >= 5)
	{
		//ֱ����ѹ��K..2����Ȼ���С����
		byte byTempGrade = 9 < byGrade ? byGrade : 9;
		if (CRPlayer::_MakePair(byTempGrade, byCount) != TYPE_GVP)
		{
			if (byTempGrade <= 12)
			{
				byGrade = byTempGrade;
				return TYPE_PAIR;
			}
		}
	}

	//������Ӧ��
	byte byTempGrade = byGrade;
	if (CRPlayer::_MakePair(byTempGrade, byCount) != TYPE_GVP)
	{
		byGrade = byTempGrade;
		return TYPE_PAIR;
	}

	byGrade = GRADE_INIT;
	byCount = 0;
	return TYPE_GVP;
}

byte CRPlayerXJ::_MakeAnySingle(byte &byGrade, byte &byCount)
{
	byte byTempGrade = byGrade;
	byte byRtnType = CRPlayer::_MakeAnySingle(byGrade, byCount);
	if (byGrade >= 11)
	{
		if (bitmap[1] != 0 || bitmap[2] != 0)
		{
			byGrade = GRADE_INIT;
			byCount = 0;
			return TYPE_GVP;
		}
	}
	byGrade = byTempGrade;
	return TYPE_SINGLE;
}

byte CRPlayerXJ::_MakeAnyPair(byte &byGrade, byte &byCount)
{
	byte byTempGrade = byGrade;
	byte byRtnType = CRPlayer::_MakeAnyPair(byTempGrade, byCount);
	if (byGrade >= 11)
	{
		if (bitmap[0] != 0 || bitmap[2] != 0)
		{
			byGrade = GRADE_INIT;
			byCount = 0;
			return TYPE_GVP;
		}
	}
	byGrade = byTempGrade;
	return TYPE_PAIR;
}

byte CRPlayerXJ::_MakeGvpRound1(byte &byGrade, byte &byCount, word &wAttach)
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

byte CRPlayerXJ::_MakeGvpRound2(byte &byGrade, byte &byCount, word &wAttach)
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

byte CRPlayerXJ::_MakeGvpRound3(byte &byGrade, byte &byCount, word &wAttach)
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
