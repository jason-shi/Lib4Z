#include "StdAfx.h"
#include "RPlayer.h"

#define SHIFT_1(n)				((1 << (n)) - 1)

bool CRPlayer::IsSinglePhobia()
{
	word _bitmap = bitmap[0] | bitmap[1] | bitmap[2];

	byte bySingleCount = 0;
	for (byte byGrade = 0; byGrade < 12; byGrade++)
	{
		if ((_bitmap & (1 << byGrade)) == 0)
			continue;
		if (!IsStupidPut(TYPE_SINGLE, byGrade, 0))
			bySingleCount++;
	}
	return bySingleCount <= 2;
}

bool CRPlayer::IsPairPhobia()
{
	word _bitmap = bitmap[1] | bitmap[2];

	byte byPairCount = 0;
	for (byte byGrade = 0; byGrade < 12; byGrade++)
	{
		if ((_bitmap & (1 << byGrade)) == 0)
			continue;
		if (!IsStupidPut(TYPE_PAIR, byGrade, 0))
			byPairCount++;
	}
	return byPairCount <= 2;
}

void CRPlayer::SetDZPhobia(CRPlayer *pDiZhu)
{
	m_bSinglePhobia = pDiZhu->IsSinglePhobia();
	m_bPairPhobia = pDiZhu->IsPairPhobia();
}

bool CRPlayer::HasWinRhythm()
{
	if (WORD_BIT_COUNT(bitmap[0] & MASK_SHUNZI)
		+ WORD_BIT_COUNT(bitmap[1]) * 2
		+ WORD_BIT_COUNT(bitmap[2]) * 3 >= 10) //��û�б���
		return false;

	int nBomb = WORD_BIT_COUNT(bitmap[3]) + ((bitmap[0] & MASK_ROCKET) == MASK_ROCKET); //ը���ͻ��������
	int nTrip = WORD_BIT_COUNT(bitmap[2]); //���ŵ�����

	if (nTrip >= 2 || nBomb >= 1)
		return true;

	//�����˳�ӣ�һ���ǰ�˳�ӳ���
	word _bitmap = bitmap[0] | bitmap[1] | bitmap[2] | bitmap[3];
	_bitmap &= MASK_SHUNZI;
	word shunzi = TblShunZi5[_bitmap];
	if (shunzi)
	{
		//���˳�ӳ��ȴ��ڻ����7
		if (WORD_BIT_COUNT(shunzi) >= 7)
			return true;

		//������˳�߽������Ż�ը��
		if (TblFiveShun[shunzi] == 0)
		{
			if ((bitmap[2] | bitmap[3]) & TblShun5PrhE[shunzi])
				return true;
		}

		//����˳�߽���ը��
		if (TblFiveShun[shunzi] != 0)
		{
			if (bitmap[3] & TblShun5PrhE[shunzi])
				return true;
		}

		_OutShunZi(TblShunZi5[_bitmap]);

		//���ȥ����ʣ�µ���
		int nRemain = WORD_BIT_COUNT(bitmap[0] | bitmap[1]); //���źͶ��ӵ�����
		bool bWinRhythm = nRemain == 1
						|| nRemain == 2 && ((bitmap[0] | bitmap[1]) & 0x7000);

		_InShunZi(TblShunZi5[_bitmap]);
		return bWinRhythm;
	}

	//��������ԣ���һ���ǰ����Գ���
	_bitmap = bitmap[1] | bitmap[2];
	_bitmap &= MASK_SHUNZI;
	word liandui = TblShunZi3[_bitmap];
	if (liandui)
	{
		_OutLianDui(TblShunZi3[_bitmap]);

		//���ȥ����ʣ�µ���
		int nRemain = WORD_BIT_COUNT(bitmap[0] | bitmap[1]); //���źͶ��ӵ�����
		bool bWinRhythm = nRemain == 1
						|| nRemain == 2 && ((bitmap[0] | bitmap[1]) & 0x7000);

		_InLianDui(TblShunZi3[_bitmap]);
		return bWinRhythm;
	}

	//��������϶��ǽϴ����
	if (WORD_BIT_COUNT((bitmap[0] | bitmap[1]) & 0x07FF) <= 1)
		return true;

	//TODO�����������ʤ����������ͣ���������
	return false;
}

void CRPlayer::SetWinRhythm(CRPlayer *pRPlayer[3])
{
	m_byWinRhythm = 0x00;

	//����ʤ������λͼ
	for (int i = 0; i < 3; i++)
	{
		if (pRPlayer[i]->HasWinRhythm())
			m_byWinRhythm |= 1 << i;
	}
}

bool CRPlayer::IsBestPut(byte byType, byte byGrade, byte byCount, word wAttach)
{
	//���ƺ�ļ�ֵ
	OutCards(byType, byGrade, byCount, wAttach);
	int nNewVA = SplitCards();
	InCards(byType, byGrade, byCount, wAttach);

	//�����͵������
	if (_CheckStupid_OnesOwn(byType, byGrade, byCount))
		return false;

	//��ʧ��ֵ����
	int nVAThr = 0;
	if (m_byChair1 == 0)
		nVAThr = -1; //�������Ʋ������ֵ��ʧ
	else
	{
		if (m_byFriend == 0)
			nVAThr = -1; //����ѹũ��
		else
		{
			if (m_byFriend & (1 << m_byChair1))
				nVAThr = 0; //ũ��ѹ����
			else
				nVAThr = -1; //ũ��ѹũ��
		}
	}

	//��ʧ�����ޱȽ�
	return m_nOriginVA - nNewVA <= nVAThr;
}

bool CRPlayer::IsBetterPut(byte byType, byte byGrade, byte byCount, word wAttach)
{
	//���ƺ�ļ�ֵ
	OutCards(byType, byGrade, byCount, wAttach);
	int nNewVA = SplitCards();
	InCards(byType, byGrade, byCount, wAttach);

	//�����͵������
	if (_CheckStupid_OnesOwn(byType, byGrade, byCount))
		return false;

	//��ʧ��ֵ����
	int nVAThr = 0;
	if (m_byChair1 == 0)
		nVAThr = -1;
	else
	{
		if (m_byFriend == 0)
			nVAThr = 0; //����ѹũ��
		else
		{
			if (m_byFriend & (1 << m_byChair1))
				nVAThr = 1; //ũ��ѹ����
			else
				nVAThr = 0; //ũ��ѹũ��
		}
	}

	//��ʧ�����ޱȽ�
	return m_nOriginVA - nNewVA <= nVAThr;
}

bool CRPlayer::IsStupidPut(byte byType, byte byGrade, byte byCount, word wAttach)
{
	//���ƺ�ļ�ֵ
	OutCards(byType, byGrade, byCount, wAttach);
	int nNewVA = SplitCards();
	InCards(byType, byGrade, byCount, wAttach);

	if (nNewVA == 100) //ʤ��
		return false;

	//�����͵������
	if (_CheckStupid_OnesOwn(byType, byGrade, byCount))
		return true;

	//��ʧ��ֵ����
	int nVAThr = 0;
	if (m_byChair1 == 0)
		nVAThr = 0; //�������Ʋ������ֵ��ʧ
	else
	{
		if (m_byFriend == 0)
		{
			nVAThr = 1; //����ѹũ��
			if (bitmap[0] == 0 && bitmap[1] == 0 && bitmap[3] == 0)
				nVAThr++; //���ڲ�����
		}
		else
		{
			if (m_byFriend & (1 << m_byChair1))
				nVAThr = 2; //ũ��ѹ����
			else
				nVAThr = 1; //ũ��ѹũ��
		}
	}

	//��ʧ�����ޱȽ�
	return m_nOriginVA - nNewVA > nVAThr;
}

bool CRPlayer::_CheckStupid_OnesOwn(byte byType, byte byGrade, byte byCount)
{
	//�ֱ�Ե��ţ����ӣ����ţ�˳�����������
	switch (byType)
	{
	case TYPE_SINGLE:
		return _CheckStupid_Single(byGrade, byCount);
	case TYPE_PAIR:
		return _CheckStupid_Pair(byGrade, byCount);
	case TYPE_TRIP:
	case TYPE_TRIP_S: //��������ѡ��ʱ�Ѿ�����
	case TYPE_TRIP_P: //��������ѡ��ʱ�Ѿ�����
		return _CheckStupid_Trip(byGrade, byCount);
	case TYPE_SHUNZI:
		return _CheckStupid_ShunZi(byGrade, byCount);
	case TYPE_LIANDUI:
		return _CheckStupid_LianDui(byGrade, byCount);
	case TYPE_BOMB:
		return _CheckStupid_Bomb(byGrade, byCount);
	case TYPE_ROCKET:
		return _CheckStupid_Rocket(byGrade, byCount);
	}
	return false;
}

bool CRPlayer::_CheckStupid_Single(byte byGrade, byte byCount)
{
	//���ɲ𴿵�����
	if (TblShun3PrhS[bitmap[1] & MASK_SHUNZI] & (1 << byGrade))
		return true;

	//���˵��Ž�ֹ
	word _bitmap = bitmap[0] | bitmap[1] | bitmap[2] | bitmap[3];
	_bitmap &= MASK_SHUNZI;
	if (TblShun5PrhE[_bitmap] & (1 << byGrade))
	{
		if (bitmap[0] & (1 << byGrade))
			return true;
	}
	//���ڵ����м��ֹ
	if (bitmap[2] & (1 << byGrade))
	{
		word mask = CARD_3 | CARD_5;
		if (byGrade != 0 && byGrade <= 10) //A���Ҳ�
			mask <<= byGrade - 1;
		if ((bitmap[0] & mask) == mask)
			return true;
	}
	return false;
}

bool CRPlayer::_CheckStupid_Pair(byte byGrade, byte byCount)
{
	//���ɲ𴿵�����
	word liandui = TblShun3PrhS[bitmap[1] & MASK_SHUNZI];
	if (liandui & (1 << byGrade))
	{
		liandui ^= TblSanShun[liandui];
		//��ֹ�������ԣ�ǰ�к��ޣ�
		if ((liandui & (1 << byGrade)) == 0)
			return true;
		//��ֹ�����Ե��м�
		liandui ^= TblShun3PrhE[liandui];
		if (liandui & (1 << byGrade))
			return true;
	}

	//���ڶ����м��ֹ
	if (bitmap[2] & (1 << byGrade))
	{
		word mask = CARD_3 | CARD_5;
		if (byGrade != 0 && byGrade <= 10) //A���Ҳ�
			mask <<= byGrade - 1;
		if ((bitmap[1] & mask) == mask)
			return true;
	}
	return false;
}

bool CRPlayer::_CheckStupid_Trip(byte byGrade, byte byCount)
{
	if ((m_byFriend & (1 << m_byChair1)) != 0) //�Ƕ���
		return true;

	if (bitmap[3] & (1 << byGrade))
	{
		word _bitmap = bitmap[0] | bitmap[1] | bitmap[2] | bitmap[3];
		_bitmap &= MASK_SHUNZI;
		word shunzi = TblShun5PrhS[_bitmap];
		if (shunzi & (1 << byGrade))
		{
			//˳�������Ż�����ʱ�����˽�ֹ
			if (TblFiveShun[shunzi] == 0 && TblShun5PrhE[shunzi] & (1 << byGrade))
				return true;
		}
		//˳�������Ż�����ʱ�������˽�ֹ
		shunzi = TblShunZi5[shunzi];
		if (WORD_BIT_COUNT(shunzi) >= 7)
		{
			shunzi ^= TblShun5PrhE[shunzi]; //ȥ������
			if (shunzi & (1 << byGrade))
				return true;
		}
	}
	return false;
}

bool CRPlayer::_CheckStupid_ShunZi(byte byGrade, byte byCount)
{
	if ((m_byFriend & (1 << m_byChair1)) != 0) //�Ƕ���
		return true;

	//˳��ͬʱ������ʱ����
	word mask = SHIFT_1(byGrade + byCount) ^ SHIFT_1(byGrade);
	if ((bitmap[1] & mask) == mask)
		return true;

	if (byCount == 5)
	{
		//����˳�Ӻ�ը��
		if (bitmap[3] & mask)
		{
			//������������
			if (WORD_BIT_COUNT(bitmap[0] & mask) != 2)
				return true;
		}
	}

	//��ĩ�����Ż�ը��
	if (byCount != 5)
	{
		if ((bitmap[2] | bitmap[3]) & TblShun5PrhE[mask])
			return true;
	}

	//��ɢ��������ը��
	if (WORD_BIT_COUNT(bitmap[3] & mask) >= 2)
		return true;

	return false;
}

bool CRPlayer::_CheckStupid_LianDui(byte byGrade, byte byCount)
{
	word mask = SHIFT_1(byGrade + byCount) ^ SHIFT_1(byGrade);
	if (bitmap[3] & mask)
		return true;

	return false;
}

bool CRPlayer::_CheckStupid_Bomb(byte byGrade, byte byCount)
{
	if ((m_byFriend & (1 << m_byChair1)) != 0) //�Ƕ���
		return true;

	return false;
}

bool CRPlayer::_CheckStupid_Rocket(byte byGrade, byte byCount)
{
	if ((m_byFriend & (1 << m_byChair1)) != 0) //�Ƕ���
		return true;

	return false;
}

byte CRPlayer::MakeCards(byte byType, byte &byGrade, byte &byCount, word &wAttach)
{
	byte byRtnType = TYPE_GVP;
	switch (byType)
	{
	case TYPE_GVP:
		byRtnType = _MakeGvp(byGrade, byCount, wAttach); CHECK_RETURN(byRtnType);
		break;
	case TYPE_SINGLE:
		byRtnType = _MakeSingle(byGrade, byCount); CHECK_RETURN(byRtnType);
		break;
	case TYPE_PAIR:
		byRtnType = _MakePair(byGrade, byCount); CHECK_RETURN(byRtnType);
		break;
	case TYPE_TRIP:
		byRtnType = _MakeTrip(byGrade, byCount); CHECK_RETURN(byRtnType);
		break;
	case TYPE_TRIP_S:
		byRtnType = _MakeTripS(byGrade, byCount, wAttach); CHECK_RETURN(byRtnType);
		break;
	case TYPE_TRIP_P:
		byRtnType = _MakeTripP(byGrade, byCount, wAttach); CHECK_RETURN(byRtnType);
		break;
	case TYPE_PLANE:
		byRtnType = _MakePlane(byGrade, byCount); CHECK_RETURN(byRtnType);
		break;
	case TYPE_PLANE_S:
		byRtnType = _MakePlaneS(byGrade, byCount, wAttach); CHECK_RETURN(byRtnType);
		break;
	case TYPE_PLANE_P:
		byRtnType = _MakePlaneP(byGrade, byCount, wAttach); CHECK_RETURN(byRtnType);
		break;
	case TYPE_QUAD_S:
		byRtnType = _MakeQuadS(byGrade, byCount, wAttach); CHECK_RETURN(byRtnType);
		break;
	case TYPE_QUAD_P:
		byRtnType = _MakeQuadP(byGrade, byCount, wAttach); CHECK_RETURN(byRtnType);
		break;
	case TYPE_SHUNZI:
		byRtnType = _MakeShunZi(byGrade, byCount); CHECK_RETURN(byRtnType);
		break;
	case TYPE_LIANDUI:
		byRtnType = _MakeLianDui(byGrade, byCount); CHECK_RETURN(byRtnType);
		break;
	case TYPE_BOMB: //׹��
		byRtnType = _MakeBomb(byGrade, byCount); CHECK_RETURN(byRtnType);
	case TYPE_ROCKET:
		return _MakeRocket(byGrade, byCount);
	}

	byRtnType = _MakeBomb(byGrade, byCount); CHECK_RETURN(byRtnType);
	byRtnType = _MakeRocket(byGrade, byCount); CHECK_RETURN(byRtnType);
	return TYPE_GVP;
}

byte CRPlayer::_MakeGvp(byte &byGrade, byte &byCount, word &wAttach)
{
	//------------------��ͷת��
	word _bitmap = bitmap[0] | bitmap[1] | bitmap[2] | bitmap[3];
	byGrade = TblBitIndex[(_bitmap & -_bitmap) % 37];
	byCount = 1;
	return TYPE_SINGLE;
}

byte CRPlayer::_MakeSingle(byte &byGrade, byte &byCount)
{
	//��˳���ɲ����Բ��ɲ𣨵�Q��
	word prhbmp = (TblShun5PrhH[bitmap[0] & MASK_SHUNZI]
			| TblShun3PrhS[bitmap[1] & MASK_SHUNZI]) & MASK_SHUNZI_Q;

	//��ó���
	{
		//�����ĵ��Ÿ�����ѡ
		word _single = bitmap[0] & TblContraryMask[byGrade];
		if (_single & ~TblShun5PrhS[(bitmap[0] | bitmap[1] | bitmap[2]) & MASK_SHUNZI])
			prhbmp = (bitmap[1] | bitmap[2]) & MASK_SHUNZI;

		byte byTempGrade = byGrade;

		word _bitmap = bitmap[0] | bitmap[1] | bitmap[2]; //Ӧ���Ų���ը��
		_bitmap ^= prhbmp;
		_bitmap &= TblContraryMask[byTempGrade];
		while (_bitmap)
		{
			byTempGrade = TblBitIndex[(_bitmap & -_bitmap) % 37];
			if (!IsBestPut(TYPE_SINGLE, byTempGrade, byCount))
			{
				_bitmap &= TblContraryMask[byTempGrade];
				continue;
			}
			byGrade = byTempGrade;
			return TYPE_SINGLE;
		}
	}
	//�Ϻó���
	{
		//�����ĵ��Ÿ�����ѡ
		word _single = bitmap[0] & TblContraryMask[byGrade];
		if (_single & ~TblShun5PrhS[(bitmap[0] | bitmap[1] | bitmap[2]) & MASK_SHUNZI])
			prhbmp = (bitmap[1] | bitmap[2]) & MASK_SHUNZI;

		byte byTempGrade = byGrade;

		word _bitmap = bitmap[0] | bitmap[1] | bitmap[2]; //Ӧ���Ų���ը��
		_bitmap ^= prhbmp;
		_bitmap &= TblContraryMask[byTempGrade];
		while (_bitmap)
		{
			byTempGrade = TblBitIndex[(_bitmap & -_bitmap) % 37];
			if (!IsBetterPut(TYPE_SINGLE, byTempGrade, byCount))
			{
				_bitmap &= TblContraryMask[byTempGrade];
				continue;
			}
			byGrade = byTempGrade;
			return TYPE_SINGLE;
		}
	}
	//��ɵX����
	{
		byte byTempGrade = byGrade;

		word _bitmap = bitmap[0] | bitmap[1] | bitmap[2]; //Ӧ���Ų���ը��
		_bitmap ^= prhbmp;
		_bitmap &= TblContraryMask[byTempGrade];
		while (_bitmap)
		{
			byTempGrade = TblBitIndex[(_bitmap & -_bitmap) % 37];
			if (IsStupidPut(TYPE_SINGLE, byTempGrade, byCount))
			{
				_bitmap &= TblContraryMask[byTempGrade];
				continue;
			}
			byGrade = byTempGrade;
			return TYPE_SINGLE;
		}
	}
	byGrade = GRADE_INIT;
	byCount = 0;
	return TYPE_GVP;
}

byte CRPlayer::_MakePair(byte &byGrade, byte &byCount)
{
	//��ó���
	{
		byte byTempGrade = byGrade;

		word _bitmap = bitmap[1] | bitmap[2];
		_bitmap &= TblContraryMask[byTempGrade];
		while (_bitmap)
		{
			byTempGrade = TblBitIndex[(_bitmap & -_bitmap) % 37];
			if (!IsBestPut(TYPE_PAIR, byTempGrade, byCount))
			{
				_bitmap &= TblContraryMask[byTempGrade];
				continue;
			}
			byGrade = byTempGrade;
			return TYPE_PAIR;
		}
	}
	//�Ϻó���
	{
		byte byTempGrade = byGrade;

		word _bitmap = bitmap[1] | bitmap[2];
		_bitmap &= TblContraryMask[byTempGrade];
		while (_bitmap)
		{
			byTempGrade = TblBitIndex[(_bitmap & -_bitmap) % 37];
			if (!IsBetterPut(TYPE_PAIR, byTempGrade, byCount))
			{
				_bitmap &= TblContraryMask[byTempGrade];
				continue;
			}
			byGrade = byTempGrade;
			return TYPE_PAIR;
		}
	}
	//��ɵX����
	{
		byte byTempGrade = byGrade;

		word _bitmap = bitmap[1] | bitmap[2];
		_bitmap &= TblContraryMask[byTempGrade];
		while (_bitmap)
		{
			byTempGrade = TblBitIndex[(_bitmap & -_bitmap) % 37];
			if (IsStupidPut(TYPE_PAIR, byTempGrade, byCount))
			{
				_bitmap &= TblContraryMask[byTempGrade];
				continue;
			}
			byGrade = byTempGrade;
			return TYPE_PAIR;
		}
	}
	byGrade = GRADE_INIT;
	byCount = 0;
	return TYPE_GVP;
}

byte CRPlayer::_MakeTrip(byte &byGrade, byte &byCount)
{
	if ((m_byFriend & (1 << m_byChair1)) != 0) //�Ƕ���
		return TYPE_GVP;

	//�Ϻó���
	{
		byte byTempGrade = byGrade;

		word _bitmap = bitmap[2] | bitmap[3];
		_bitmap &= TblContraryMask[byTempGrade];
		while (_bitmap)
		{
			byTempGrade = TblBitIndex[(_bitmap & -_bitmap) % 37];
			if (IsStupidPut(TYPE_TRIP, byTempGrade, byCount))
			{
				_bitmap &= TblContraryMask[byTempGrade];
				continue;
			}
			byGrade = byTempGrade;
			return TYPE_TRIP;
		}
	}
	//��ɵX����
	{
		byte byTempGrade = byGrade;

		word _bitmap = bitmap[2] | bitmap[3];
		_bitmap &= TblContraryMask[byTempGrade];
		while (_bitmap)
		{
			byTempGrade = TblBitIndex[(_bitmap & -_bitmap) % 37];
			if (IsStupidPut(TYPE_TRIP, byTempGrade, byCount))
			{
				_bitmap &= TblContraryMask[byTempGrade];
				continue;
			}
			byGrade = byTempGrade;
			return TYPE_TRIP;
		}
	}
	byGrade = GRADE_INIT;
	byCount = 0;
	return TYPE_GVP;
}

byte CRPlayer::_MakeTripS(byte &byGrade, byte &byCount, word &wAttach)
{
	if ((m_byFriend & (1 << m_byChair1)) != 0) //�Ƕ���
		return TYPE_GVP;

	if (_MakeTrip(byGrade, byCount) == TYPE_GVP)
	{
		byGrade = GRADE_INIT;
		byCount = 0;
		return TYPE_GVP;
	}
	OutCards(TYPE_TRIP, byGrade, byCount);
	_MakeAttach1S(wAttach);
	InCards(TYPE_TRIP, byGrade, byCount);

	wAttach &= ~(1 << byGrade); //��ײ

	if (wAttach != 0)
		return TYPE_TRIP_S;

	byGrade = GRADE_INIT;
	byCount = 0;
	return TYPE_GVP;
}

byte CRPlayer::_MakeTripP(byte &byGrade, byte &byCount, word &wAttach)
{
	if ((m_byFriend & (1 << m_byChair1)) != 0) //�Ƕ���
		return TYPE_GVP;

	if (_MakeTrip(byGrade, byCount) == TYPE_GVP)
	{
		byGrade = GRADE_INIT;
		byCount = 0;
		return TYPE_GVP;
	}
	OutCards(TYPE_TRIP, byGrade, byCount);
	_MakeAttach1P(wAttach);
	InCards(TYPE_TRIP, byGrade, byCount);

	if (wAttach != 0)
		return TYPE_TRIP_P;

	byGrade = GRADE_INIT;
	byCount = 0;
	return TYPE_GVP;
}

byte CRPlayer::_MakePlane(byte &byGrade, byte &byCount)
{
	if ((m_byFriend & (1 << m_byChair1)) != 0) //�Ƕ���
		return TYPE_GVP;

	byte byRtnType = TYPE_GVP;

	word _bitmap = bitmap[2] | bitmap[3];
	_bitmap &= MASK_SHUNZI;
	_bitmap &= TblContraryMask[byGrade]; //�������

	word mask;
	while (mask = TblShunZi2[_bitmap])
	{
		if (WORD_BIT_COUNT(mask) >= byCount)
		{
			byGrade = TblBitIndex[(mask & -mask) % 37];
			if (IsStupidPut(TYPE_PLANE, byGrade, byCount))
			{
				_bitmap &= TblContraryMask[byGrade];
				continue;
			}
			return TYPE_PLANE;
		}
		_bitmap ^= mask; //���Գ��Ȳ���
	}
	byGrade = GRADE_INIT;
	byCount = 0;
	return TYPE_GVP;
}

byte CRPlayer::_MakePlaneS(byte &byGrade, byte &byCount, word &wAttach)
{
	if ((m_byFriend & (1 << m_byChair1)) != 0) //�Ƕ���
		return TYPE_GVP;

	if (_MakePlane(byGrade, byCount) == TYPE_GVP)
	{
		byGrade = GRADE_INIT;
		byCount = 0;
		return TYPE_GVP;
	}
	OutCards(TYPE_PLANE, byGrade, byCount);
	_MakeAttach2S(wAttach);
	InCards(TYPE_PLANE, byGrade, byCount);

	wAttach &= ~(SHIFT_1(byGrade + byCount) ^ SHIFT_1(byGrade)); //��ײ

	if (wAttach != 0)
		return TYPE_PLANE_S;

	byGrade = GRADE_INIT;
	byCount = 0;
	return TYPE_GVP;
}

byte CRPlayer::_MakePlaneP(byte &byGrade, byte &byCount, word &wAttach)
{
	if ((m_byFriend & (1 << m_byChair1)) != 0) //�Ƕ���
		return TYPE_GVP;

	if (_MakePlane(byGrade, byCount) == TYPE_GVP)
	{
		byGrade = GRADE_INIT;
		byCount = 0;
		return TYPE_GVP;
	}
	OutCards(TYPE_PLANE, byGrade, byCount);
	_MakeAttach2P(wAttach);
	InCards(TYPE_PLANE, byGrade, byCount);

	if (wAttach != 0)
		return TYPE_PLANE_P;

	byGrade = GRADE_INIT;
	byCount = 0;
	return TYPE_GVP;
}

byte CRPlayer::_MakeQuadS(byte &byGrade, byte &byCount, word &wAttach)
{
	if ((m_byFriend & (1 << m_byChair1)) != 0) //�Ƕ���
		return TYPE_GVP;

	if (_MakeBomb(byGrade, byCount) == TYPE_GVP)
	{
		byGrade = GRADE_INIT;
		byCount = 0;
		return TYPE_GVP;
	}
	OutCards(TYPE_BOMB, byGrade, byCount);
	_MakeAttach2S(wAttach);
	InCards(TYPE_BOMB, byGrade, byCount);

	if (wAttach != 0)
		return TYPE_QUAD_S;

	byGrade = GRADE_INIT;
	byCount = 0;
	return TYPE_GVP;
}

byte CRPlayer::_MakeQuadP(byte &byGrade, byte &byCount, word &wAttach)
{
	if ((m_byFriend & (1 << m_byChair1)) != 0) //�Ƕ���
		return TYPE_GVP;

	if (_MakeBomb(byGrade, byCount) == TYPE_GVP)
	{
		byGrade = GRADE_INIT;
		byCount = 0;
		return TYPE_GVP;
	}
	OutCards(TYPE_BOMB, byGrade, byCount);
	_MakeAttach2P(wAttach);
	InCards(TYPE_BOMB, byGrade, byCount);

	if (wAttach != 0)
		return TYPE_QUAD_P;

	byGrade = GRADE_INIT;
	byCount = 0;
	return TYPE_GVP;
}

byte CRPlayer::_MakeShunZi(byte &byGrade, byte &byCount)
{
	//�Լ�û�л�ʤ���࣬�Լ��л�ʤ���࣬�ƽϴ�ʱ������
	if (ReleasePlayer(m_byChair1) || (m_byFriend & (1 << m_byChair1)) != 0 && byGrade >= 7 && byGrade != GRADE_INIT)
		return TYPE_GVP;

	word _bitmap = bitmap[0] | bitmap[1] | bitmap[2] | bitmap[3];
	_bitmap &= MASK_SHUNZI;
	_bitmap &= TblContraryMask[byGrade]; //�������

	word mask;
	while (mask = TblShunZi5[_bitmap])
	{
		if (WORD_BIT_COUNT(mask) >= byCount)
		{
			byGrade = TblBitIndex[(mask & -mask) % 37];
			if (IsStupidPut(TYPE_SHUNZI, byGrade, byCount))
			{
				_bitmap &= TblContraryMask[byGrade];
				continue;
			}
			return TYPE_SHUNZI;
		}
		_bitmap ^= mask; //˳�ӳ��Ȳ���
	}
	byGrade = GRADE_INIT;
	byCount = 0;
	return TYPE_GVP;
}

byte CRPlayer::_MakeLianDui(byte &byGrade, byte &byCount)
{
	if ((m_byFriend & (1 << m_byChair1)) != 0) //�Ƕ���
		return TYPE_GVP;

	byte byRtnType = TYPE_GVP;

	word _bitmap = bitmap[1] | bitmap[2] | bitmap[3];
	_bitmap &= MASK_SHUNZI;
	_bitmap &= TblContraryMask[byGrade]; //�������

	word mask;
	while (mask = TblShunZi3[_bitmap])
	{
		if (WORD_BIT_COUNT(mask) >= byCount)
		{
			byGrade = TblBitIndex[(mask & -mask) % 37];
			if (IsStupidPut(TYPE_LIANDUI, byGrade, byCount))
			{
				_bitmap &= TblContraryMask[byGrade];
				continue;
			}
			return TYPE_LIANDUI;
		}
		_bitmap ^= mask; //���Գ��Ȳ���
	}
	byGrade = GRADE_INIT;
	byCount = 0;
	return TYPE_GVP;
}

byte CRPlayer::_MakeBomb(byte &byGrade, byte &byCount)
{
	if (!SuppressPlayer(m_byChair1))
		return TYPE_GVP;

	word _bitmap = bitmap[3];
	_bitmap &= TblContraryMask[byGrade];
	while (_bitmap)
	{
		byGrade = TblBitIndex[(_bitmap & -_bitmap) % 37];
		if (IsStupidPut(TYPE_BOMB, byGrade, byCount))
		{
			_bitmap &= TblContraryMask[byGrade];
			continue;
		}
		return TYPE_BOMB;
	}
	byGrade = GRADE_INIT;
	byCount = 0;
	return TYPE_GVP;
}

byte CRPlayer::_MakeRocket(byte &byGrade, byte &byCount)
{
	if (!SuppressPlayer(m_byChair1))
		return TYPE_GVP;

	if ((bitmap[0] & MASK_ROCKET) == MASK_ROCKET)
		return TYPE_ROCKET;

	return TYPE_GVP;
}

byte CRPlayer::_MakeAnySingle(byte &byGrade, byte &byCount)
{
	//�Լ�û�л�ʤ���࣬�Լ��л�ʤ���࣬�ƽϴ�ʱ������
	if (ReleasePlayer(m_byChair1) || (m_byFriend & (1 << m_byChair1)) != 0 && byGrade >= 9 && byGrade != GRADE_INIT)
		return TYPE_GVP;

	//��˳����Q�����ɲ����Բ��ɲ𣬸���Բ��ɲ�
	word prhbmp = (TblShun5PrhH[bitmap[0] & MASK_SHUNZI]
				| TblShun3PrhS[bitmap[1] & MASK_SHUNZI]
				| bitmap[1] & ~TblShun5PrhS[(bitmap[0] | bitmap[1]) & MASK_SHUNZI]) & MASK_SHUNZI_Q;

	word _bitmap = bitmap[0] | bitmap[1]/* | bitmap[2] | bitmap[3]*/; //Ӧ���Ų���ը��
	_bitmap ^= prhbmp;
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

byte CRPlayer::_MakeAnyPair(byte &byGrade, byte &byCount)
{
	//�Լ�û�л�ʤ���࣬�Լ��л�ʤ���࣬�ƽϴ�ʱ������
	if (ReleasePlayer(m_byChair1) || (m_byFriend & (1 << m_byChair1)) != 0 && byGrade >= 8 && byGrade != GRADE_INIT)
		return TYPE_GVP;

	word _bitmap = bitmap[1] | bitmap[2]/* | bitmap[3]*/; //Ӧ����Ӳ�����

	word _bitmap4 = bitmap[0] | bitmap[1] | bitmap[2] | bitmap[3];
	_bitmap4 &= MASK_SHUNZI;
	_bitmap ^= ~TblShun5PrhS[_bitmap4] & bitmap[2]; //��������ը��
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

byte CRPlayer::_MakeAnyShunZi(byte &byGrade, byte &byCount)
{
	word _bitmap = bitmap[0] | bitmap[1] | bitmap[2] | bitmap[3];
	_bitmap &= MASK_SHUNZI;
	if (byCount == 0) //��һ�μ���˳��
		byCount = 12;
	for ( ; byCount >= 5; byCount--)
	{
		word __bitmap = _bitmap;
		while (__bitmap != 0)
		{
			__bitmap &= TblContraryMask[byGrade];
			word mask = TblShunZi5[__bitmap];
			if (WORD_BIT_COUNT(mask) >= byCount)
			{
				byGrade = TblBitIndex[(mask & -mask) % 37];
				if (IsStupidPut(TYPE_SHUNZI, byGrade, byCount))
				{
					byGrade++;
					continue;
				}
				return TYPE_SHUNZI;
			}
			break;
		}
		byGrade = GRADE_INIT; //û���㹻������˳��
	}
	byGrade = GRADE_INIT;
	byCount = 0;
	return TYPE_GVP;
}

byte CRPlayer::_MakeAnyLianDui(byte &byGrade, byte &byCount)
{
	word _bitmap = bitmap[1] | bitmap[2] | bitmap[3];
	_bitmap &= MASK_SHUNZI;
	if (byCount == 0) //��һ�μ�������
		byCount = 8;
	for ( ; byCount >= 3; byCount--)
	{
		word __bitmap = _bitmap;
		while (__bitmap != 0)
		{
			__bitmap &= TblContraryMask[byGrade];
			word mask = TblShunZi3[__bitmap];
			if (WORD_BIT_COUNT(mask) >= byCount)
			{
				byGrade = TblBitIndex[(mask & -mask) % 37];
				if (IsStupidPut(TYPE_LIANDUI, byGrade, byCount))
				{
					byGrade++;
					continue;
				}
				return TYPE_LIANDUI;
			}
			break;
		}
		byGrade = GRADE_INIT; //û���㹻����������
	}
	byGrade = GRADE_INIT;
	byCount = 0;
	return TYPE_GVP;
}

void CRPlayer::_MakeAttach1S(word &wAttach)
{
	word _bitmap = bitmap[0] | bitmap[1] | bitmap[2] | bitmap[3];
	_bitmap &= MASK_SHUNZI;

	word _single = bitmap[0] & ~TblShun5PrhS[_bitmap];
	_single &= MASK_SHUNZI; //������˳�ӵĵ��ţ�3..A)
	if (_single)
	{
		wAttach = _single & -_single; //ֻ����С�ĵ���
		return;
	}

	//˳���еĶ������
	word prhbmp = 0;
	word shunzi = TblShunZi5[_bitmap];
	if (shunzi)
	{
		word __bitmap0 = bitmap[0] & shunzi;
		word __bitmap1 = bitmap[1] & shunzi;
		word __bitmap2 = bitmap[2] & shunzi;

		//���Բ�˳��
		if (__bitmap0)
			prhbmp |= ~(__bitmap0 & -__bitmap0) & shunzi;

		//�жԳ��Բ�ԣ�û�Գ��Բ�����
		if (__bitmap1)
			prhbmp |= ~(__bitmap1 & -__bitmap1) & shunzi;
		else if (__bitmap2)
			prhbmp |= ~(__bitmap2 & -__bitmap2) & shunzi;

		//���Բ�˳�������ĵ���
		word highbit = shunzi ^ TblShun5PrhH[shunzi];
		if (highbit & bitmap[0])
			prhbmp &= ~highbit;
	}

	//��ֹ�������ĵ���
	prhbmp = TblFiveShun[bitmap[0] & MASK_SHUNZI];

	//��ֹ����
	if ((bitmap[0] & MASK_ROCKET) == MASK_ROCKET)
		prhbmp |= MASK_ROCKET;

	//��ֹ���2
	if (bitmap[1] & CARD_2)
		prhbmp |= CARD_2;

	//��ֹ����˳�ӵ����ţ�����
	prhbmp |= (bitmap[2] | bitmap[3]) & ~TblShun5PrhS[_bitmap];

	wAttach = bitmap[0] | bitmap[1] | bitmap[2] | bitmap[3];
	wAttach ^= prhbmp;

	//ȡ��С�ĵ���
	wAttach &= -wAttach;
}

void CRPlayer::_MakeAttach1P(word &wAttach)
{
	word _bitmap = bitmap[0] | bitmap[1] | bitmap[2] | bitmap[3];
	_bitmap &= MASK_SHUNZI;

	word _pair = bitmap[1] & ~TblShun5PrhS[_bitmap];
	_pair &= MASK_SHUNZI; //������˳�ӵĶ��ӣ�3..A)
	if (_pair)
	{
		wAttach = _pair & -_pair; //ֻ����С�Ķ���
		return;
	}

	word prhbmp = 0;

	//��ֹ�𲻹���˳�ӵ����ţ�����
	prhbmp |= (bitmap[2] | bitmap[3]) & ~TblShun5PrhS[_bitmap];
	word shunzi = TblShunZi5[_bitmap];
	if (shunzi)
	{
		//��ֹ��˳���е�ը��
		if (bitmap[3] & shunzi)
			prhbmp |= bitmap[3] & shunzi;
	}

	wAttach = bitmap[1] | bitmap[2] | bitmap[3];
	wAttach ^= prhbmp;

	//ȡ��С�Ķ���
	wAttach &= -wAttach;
}

void CRPlayer::_MakeAttach2S(word &wAttach)
{
	word _bitmap = bitmap[0] | bitmap[1] | bitmap[2] | bitmap[3];
	_bitmap &= MASK_SHUNZI;

	word _single = bitmap[0] & ~TblShun5PrhS[_bitmap];
	_single &= MASK_SHUNZI; //������˳�ӵĵ��ţ�3..A)

	word shunzi = TblShunZi5[_bitmap];
	if (shunzi)
	{
		//��˳��������Ų������
		if (bitmap[1] & shunzi)
			_single |= bitmap[1] & shunzi;
	}

	int nSingleCount = WORD_BIT_COUNT(_single);
	if (nSingleCount < 2)
		_single = bitmap[0] | bitmap[1] | bitmap[2]; //����ģʽ

	if (WORD_BIT_COUNT(_single) < 2)
		wAttach = 0;
	else
	{
		word temp1 = _single & -_single;
		_single ^= temp1;
		word temp2 = _single & -_single;
		wAttach = temp1 | temp2;
	}

	//������ͬʱ����С��
	if ((wAttach & MASK_ROCKET) == MASK_ROCKET)
		wAttach = 0;
}

void CRPlayer::_MakeAttach2P(word &wAttach)
{
	word _bitmap = bitmap[0] | bitmap[1] | bitmap[2] | bitmap[3];
	_bitmap &= MASK_SHUNZI;

	word _pair = bitmap[1] & ~TblShun5PrhS[_bitmap];
	_pair &= MASK_SHUNZI; //������˳�ӵĶ��ӣ�3..A)

	word shunzi = TblShunZi5[_bitmap];
	if (shunzi)
	{
		//��˳��������Ų������
		if (bitmap[2] & shunzi)
			_pair |= bitmap[2] & shunzi;

		//���������ϵı߽�������
		if (TblFiveShun[shunzi] == 0)
		{
			if (bitmap[1] & TblShun5PrhE[shunzi])
				_pair |= bitmap[1] & TblShun5PrhE[shunzi];
		}
	}

	int nPairCount = WORD_BIT_COUNT(_pair);
	if (nPairCount < 2)
		_pair = bitmap[1] | bitmap[2]; //����ģʽ

	if (WORD_BIT_COUNT(_pair) < 2)
		wAttach = 0;
	else
	{
		word temp1 = _pair & -_pair;
		_pair ^= temp1;
		word temp2 = _pair & -_pair;
		wAttach = temp1 | temp2;
	}
}

void CRPlayer::_MakeAttachNS(byte byCount, word &wAttach)
{
	_MakeAttach2S(wAttach);
	if (wAttach == 0)
		return;

	//����N-2������
	word _bitmap = bitmap[0] | bitmap[1] | bitmap[2];
	_bitmap ^= wAttach;
	for (int i = 0; i < byCount - 2; i++)
	{
		word temp = _bitmap & -_bitmap;
		wAttach |= temp;
		_bitmap ^= temp;
	}

	//������ͬʱ����С��
	if ((wAttach & MASK_ROCKET) == MASK_ROCKET)
		wAttach = 0;
}

void CRPlayer::_MakeAttachNP(byte byCount, word &wAttach)
{
	_MakeAttach2P(wAttach);
	if (wAttach == 0)
		return;

	//����N-2������
	word _bitmap = bitmap[1] | bitmap[2] | bitmap[3];
	_bitmap ^= wAttach;
	for (int i = 0; i < byCount - 2; i++)
	{
		word temp = _bitmap & -_bitmap;
		wAttach |= temp;
		_bitmap ^= temp;
	}
}
