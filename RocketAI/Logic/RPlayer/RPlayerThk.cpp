#include "StdAfx.h"
#include "RPlayer.h"

#define SHIFT_1(n)				((1 << (n)) - 1)

void CRPlayer::_CalcSinglePhobia(byte& bySinglePhobia)
{
	word _bitmap = bitmap[0] | bitmap[1] | bitmap[2];

	byte bySingleCount = 0;
	for (byte byGrade = 0; byGrade < 12; byGrade++)
	{
		if ((_bitmap & (1 << byGrade)) == 0)
			continue;
		if (!_CheckStupid(TYPE_SINGLE, byGrade, 1))
			bySingleCount++;
	}
	bySinglePhobia = bySingleCount;
}

void CRPlayer::_CalcPairPhobia(byte& byPairPhobia)
{
	word _bitmap = bitmap[1] | bitmap[2];

	byte byPairCount = 0;
	for (byte byGrade = 0; byGrade < 12; byGrade++)
	{
		if ((_bitmap & (1 << byGrade)) == 0)
			continue;
		if (!_CheckStupid(TYPE_PAIR, byGrade, 1))
			byPairCount++;
	}
	byPairPhobia = byPairCount;
}

void CRPlayer::_CalcShunZiPhobia(byte& byShunZiPhobia)
{
	word _bitmap = bitmap[0] | bitmap[1] | bitmap[2] | bitmap[3];
	_bitmap &= MASK_SHUNZI;

	word mask = TblShunPrhS[_bitmap];

	byShunZiPhobia = WORD_BIT_COUNT(mask);
}

void CRPlayer::GetTypePhobia(byte byTypePhobia[3])
{
	//������ܲ���
	bool bHasRocket = HasRocket();

	if (bHasRocket)
		OutCards(TYPE_ROCKET, 0, 0);

	//���λ��Ӱ���ļ���
	_CalcSinglePhobia(byTypePhobia[PI_SINGLE_PHOBIA]);
	_CalcPairPhobia(byTypePhobia[PI_PAIR_PHOBIA]);
	_CalcShunZiPhobia(byTypePhobia[PI_SHUNZI_PHOBIA]);

	if (bHasRocket)
		InCards(TYPE_ROCKET, 0, 0);
}

void CRPlayer::GetWinRate(byte& byWinRate)
{
	bool bHasRocket = false;
	if (HasRocket())
	{
		bHasRocket = true;
		OutCards(TYPE_ROCKET, 0, 0);
	}

	word _bitmap = bitmap[0] | bitmap[1] | bitmap[2] | bitmap[3];
	_bitmap &= MASK_SHUNZI;

	word mask = TblShunPrhS[_bitmap];

	//�������ĵ��š�����������2������
	word _mask = ~mask & (bitmap[0] | bitmap[1]);
	_mask &= MASK_SHUNZI;
	byWinRate = WORD_BIT_COUNT(_mask);

	//����˳�ӵ�״��
	while (mask)
	{
		word shunzi = TblShunZi[mask];
		mask ^= shunzi;

		//������
		if (TblShunZi[shunzi & (bitmap[1] | bitmap[2] | bitmap[3])])
		{
			//���Ǵ��������
			if ((shunzi & bitmap[1]) != shunzi)
				byWinRate++;
			continue;
		}
		//������K��A�ս��˳��
		if ((shunzi & CARD_K) == 0)
			byWinRate++;

		//���Ǵ����˳��
		if ((shunzi & bitmap[0]) != shunzi)
			byWinRate++;
	}

	if (bHasRocket)
	{
		if (byWinRate > 0)
			byWinRate--;
		InCards(TYPE_ROCKET, 0, 0);
	}
}

void CRPlayer::SetThkParam(byte byPhobiaAttack, byte byWinRhythm)
{
	m_byPhobiaAttack = byPhobiaAttack;
	m_byWinRhythm = byWinRhythm;
}

word CRPlayer::GetOddmentMask()
{
	word _bitmap = bitmap[0] | bitmap[1] | bitmap[2] | bitmap[3];
	_bitmap &= MASK_SHUNZI;

	return ~TblShunPrhS[_bitmap] & (bitmap[0] | bitmap[1]);
}

bool CRPlayer::IsStupidPut(byte byType, byte byGrade, byte byCount)
{
	//������ܲ���
	m_bShieldRocket = HasRocket();

	if (m_bShieldRocket)
		OutCards(TYPE_ROCKET, 0, 0);

	//���λ��Ӱ���ļ��
	bool bStupidPut = _CheckStupid(byType, byGrade, byCount);

	if (m_bShieldRocket)
		InCards(TYPE_ROCKET, 0, 0);

	return bStupidPut;
}

bool CRPlayer::_CheckStupid(byte byType, byte byGrade, byte byCount)
{
	//�ɵ���ɢ���ţ�����
	word old = GetOddmentMask();

	//�µ���ɢ���ţ�����
	OutCards(byType, byGrade, byCount);
	word new1 = GetOddmentMask();
	InCards(byType, byGrade, byCount);

	if (_CheckStupid_Oddment(byType, byGrade, byCount, old, new1)) //��ɢ�Ƽ��
		return true;
	if (_CheckStupid_OnesOwn(byType, byGrade, byCount, old, new1)) //�����Ͷ������
		return true;
	if (_CheckStupid_Together(byType, byGrade, byCount, old, new1)) //�����͹�ͬ���
		return true;

	//TODO:�������
	return false;
}

bool CRPlayer::_CheckStupid_Oddment(byte byType, byte byGrade, byte byCount, word old, word new1)
{
	//�Ƚ��¾���ɢ���ţ����ӵ�����
	if (byType == TYPE_SINGLE || byType == TYPE_GOU)
	{
		if (WORD_BIT_COUNT(new1) - WORD_BIT_COUNT(old) >= 1)
			return true;
	}
	else
	{
		if (WORD_BIT_COUNT(new1) - WORD_BIT_COUNT(old) >= 2)
			return true;
	}
	return false;
}

bool CRPlayer::_CheckStupid_OnesOwn(byte byType, byte byGrade, byte byCount, word old, word new1)
{
	//�ֱ�Ե��ţ����ӣ�˳�ӣ�ը�����������
	switch (byType)
	{
	case TYPE_SINGLE:
		return _CheckStupid_Single(byGrade, byCount, old, new1);
	case TYPE_PAIR:
		return _CheckStupid_Pair(byGrade, byCount, old, new1);
	case TYPE_SHUNZI: 
		return _CheckStupid_ShunZi(byGrade, byCount, old, new1);
	case TYPE_BOMB:
		return _CheckStupid_Bomb(byGrade, byCount, old, new1);
	}
	return false;
}

bool CRPlayer::_CheckStupid_Together(byte byType, byte byGrade, byte byCount, word old, word new1)
{
	//��������ǵ��Ż���ӣ�����ը����Ҫ������Ƿ����
	if (byType == TYPE_SINGLE || byType == TYPE_PAIR || byType == TYPE_CHA)
	{
		if (bitmap[2] & (1 << byGrade))
		{
			//�µ���ɢ���ţ�����
			bitmap[2] ^= 1 << byGrade;
			word new2 = GetOddmentMask();

			bitmap[2] ^= 1 << byGrade;

			//��ըһ��ʱ������ѡ���
			if (byType == TYPE_CHA)
			{
				if (WORD_BIT_COUNT(new2) == WORD_BIT_COUNT(new1))
					return false;
			}

			//������˳��
			word _bitmap = bitmap[0] | bitmap[1] | bitmap[2] | bitmap[3];
			_bitmap &= MASK_SHUNZI;
			if (TblSanShun[_bitmap] & (1 << byGrade))
			{
				//ȥ���������µ���
				_bitmap &= TblContraryMask[byGrade <= 2 ? GRADE_INIT : byGrade - 2];

				//�õ�˳�ӵ����루��ȥը����
				word mask = TblShunZi[_bitmap];
				mask ^= 1 << byGrade;

				//�����ű����
				if ((bitmap[0] & mask) == mask)
					return false;
				//һ��Ϊ���ţ�һ��Ϊ���ӣ���ը���
				if ((bitmap[0] & mask) && (bitmap[1] & mask))
					return false;
				//���Բ治��ը
				if ((bitmap[1] & mask) == mask)
					return true;
				//һ��Ϊ����or���ӣ�һ��Ϊը��or�ڣ�ѡ��ը
				if (((bitmap[0] | bitmap[1]) & mask) && ((bitmap[2] | bitmap[3]) & mask))
					return true;
			}

			//����ѡ��ը
			if (WORD_BIT_COUNT(new2) < WORD_BIT_COUNT(new1))
				return true;
		}
	}
	return false;
}

bool CRPlayer::_CheckStupid_Single(byte byGrade, byte byCount, word old, word new1)
{
	//���˵��Ž�ֹ
	word _bitmap = bitmap[0] | bitmap[1] | bitmap[2] | bitmap[3];
	_bitmap &= MASK_SHUNZI;
	if (TblShunPrhE[_bitmap] & (1 << byGrade))
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

bool CRPlayer::_CheckStupid_Pair(byte byGrade, byte byCount, word old, word new1)
{
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

bool CRPlayer::_CheckStupid_ShunZi(byte byGrade, byte byCount, word old, word new1)
{
	//˳��ͬʱ��˫��ʱ����
	word mask = SHIFT_1(byGrade + byCount) ^ SHIFT_1(byGrade);
	if ((bitmap[1] & mask) == mask)
		return true;

	if (byCount == 3)
	{
		//����˳�Ӻ���
		if (bitmap[3] & mask)
		{
			//������������
			if (WORD_BIT_COUNT(bitmap[0] & mask) != 2)
				return true;
		}
	}

	//��ĩ��ը������
	if ((bitmap[2] | bitmap[3]) & TblShunPrhE[mask])
		return true;

	//��ɢ��������ը��
	if (WORD_BIT_COUNT(bitmap[2] & mask) >= 2)
		return true;

	return false;
}

bool CRPlayer::_CheckStupid_Bomb(byte byGrade, byte byCount, word old, word new1)
{
	if (bitmap[3] & (1 << byGrade))
	{
		//�µ���ɢ���ţ�����
		bitmap[3] ^= 1 << byGrade;
		word new2 = GetOddmentMask();

		bitmap[3] ^= 1 << byGrade;

		word _bitmap = bitmap[0] | bitmap[1] | bitmap[2] | bitmap[3];
		_bitmap &= MASK_SHUNZI;
		if (TblSanShun[_bitmap] & (1 << byGrade))
		{
			//ȥ���������µ���
			_bitmap &= TblContraryMask[byGrade <= 2 ? GRADE_INIT : byGrade - 2];

			//�õ�˳�ӵ����루��ȥ�ڣ�
			word mask = TblShunZi[_bitmap];
			mask ^= 1 << byGrade;

			//�����ű����ը��
			if ((bitmap[0] & mask) == mask)
				return false;
			//һ��Ϊ���ţ�һ��Ϊ���ӣ���ը�����
			if ((bitmap[0] & mask) && (bitmap[1] & mask))
				return true;
			//����ը��������
			if ((bitmap[1] & mask) == mask)
				return true;
			//һ��Ϊ����or���ӣ�һ��Ϊը��or�ڣ�ѡ����
			if (((bitmap[0] | bitmap[1]) & mask) && ((bitmap[2] | bitmap[3]) & mask))
				return true;
		}

		//����ѡ����
		if (WORD_BIT_COUNT(new2) < WORD_BIT_COUNT(new1))
			return true;
	}
	return false;
}

byte CRPlayer::MakeCards(byte byChair1, byte byType, byte &byGrade, byte &byCount)
{
	//������ܲ���
	m_bShieldRocket = HasRocket();

	if (m_bShieldRocket)
		OutCards(TYPE_ROCKET, 0, 0);

	//���λ��Ӱ���ĳ���
	byte byRtnType = _MakeCards(byChair1, byType, byGrade, byCount);

	if (m_bShieldRocket)
		InCards(TYPE_ROCKET, 0, 0);

	return byRtnType;
}

byte CRPlayer::_MakeCards(byte byChair1, byte byType, byte &byGrade, byte &byCount)
{
	byte byRtnType = TYPE_GVP;
	switch (byType)
	{
	case TYPE_GVP:
		byRtnType = _MakeGvp(byChair1, byGrade, byCount); CHECK_RETURN(byRtnType);
	case TYPE_SINGLE:
		byRtnType = _MakeSingle(byChair1, byGrade, byCount); CHECK_RETURN(byRtnType);
		return _MakeBCRR(byChair1, byGrade, byCount);
	case TYPE_PAIR:
		byRtnType = _MakePair(byChair1, byGrade, byCount); CHECK_RETURN(byRtnType);
		return _MakeBCRR(byChair1, byGrade, byCount);
	case TYPE_SHUNZI:
		byRtnType = _MakeShunZi(byChair1, byGrade, byCount); CHECK_RETURN(byRtnType);
		return _MakeBCRR(byChair1, byGrade, byCount);
	case TYPE_LIANDUI:
		byRtnType = _MakeLianDui(byChair1, byGrade, byCount); CHECK_RETURN(byRtnType);
		return _MakeRocket(byChair1, byGrade, byCount);
	case TYPE_BOMB:
		byRtnType = _MakeBomb(byChair1, byGrade, byCount); CHECK_RETURN(byRtnType);
	case TYPE_CANNON:
		byRtnType = _MakeCannon(byChair1, byGrade, byCount); CHECK_RETURN(byRtnType);
		byRtnType = _MakeRedFlag(byChair1, byGrade, byCount); CHECK_RETURN(byRtnType);
	case TYPE_REDFLAG:
		return _MakeRocket(byChair1, byGrade, byCount);
	case TYPE_CHA:
		return _MakeCha(byChair1, byGrade, byCount);
	case TYPE_GOU:
		return _MakeGou(byChair1, byGrade, byCount);
	}
	return TYPE_GVP;
}

byte CRPlayer::_MakeGvp(byte byChair1, byte &byGrade, byte &byCount)
{
	//--------------------------------ս�Խ���---------------------------------
	//���Թ�˳��
	byGrade = GRADE_INIT;
	byCount = 0;
	if (m_byPhobiaAttack & (1 << PI_SHUNZI_PHOBIA))
	{
		if (_TryMakeShunZi(byGrade, byCount) != TYPE_GVP)
		{
			if (byGrade + byCount <= 10) //3-K
				return TYPE_SHUNZI;
		}
	}
	//���Թ�����
	byGrade = GRADE_INIT;
	byCount = 0;
	if (m_byPhobiaAttack & (1 << PI_SINGLE_PHOBIA))
	{
		if (_MakeSingle(byChair1, byGrade, byCount) != TYPE_GVP)
		{
			if (byGrade <= 10)
				return TYPE_SINGLE;
		}
	}
	//���Թ�����
	byGrade = GRADE_INIT;
	byCount = 0;
	if (m_byPhobiaAttack & (1 << PI_PAIR_PHOBIA))
	{
		if (_MakePair(byChair1, byGrade, byCount) != TYPE_GVP)
		{
			if (byGrade <= 10)
				return TYPE_PAIR;
		}
	}

	//--------------------------------ս�Է���---------------------------------
	//���Գ�С˳��
	byGrade = GRADE_INIT;
	byCount = 0;
	if (_TryMakeShunZi(byGrade, byCount) != TYPE_GVP)
	{
		if (byGrade + byCount - 1 <= 7) //3-10
			return TYPE_SHUNZI;
	}
	//���Գ�С����
	byGrade = GRADE_INIT;
	byCount = 0;
	if (_TryMakeLianDui(byGrade, byCount) != TYPE_GVP)
	{
		if (byGrade + byCount <= 7) //3-10
			return TYPE_LIANDUI;
	}
	//���Գ�С����
	byGrade = GRADE_INIT;
	byCount = 0;
	if (_MakeSingle(byChair1, byGrade, byCount) != TYPE_GVP)
	{
		if (byGrade <= 7) //3-10
			return TYPE_SINGLE;
	}
	//���Գ�С����
	byGrade = GRADE_INIT;
	byCount = 0;
	if (_MakePair(byChair1, byGrade, byCount) != TYPE_GVP)
	{
		if (byGrade <= 7) //3-10
			return TYPE_PAIR;
	}
	//-------------------------------------------------------------------------
	//���Գ���˳��
	byGrade = GRADE_INIT;
	byCount = 0;
	if (_TryMakeShunZi(byGrade, byCount) != TYPE_GVP)
	{
		if (byGrade + byCount - 1 <= 9) //3-Q
			return TYPE_SHUNZI;
	}
	//���Գ�������
	byGrade = GRADE_INIT;
	byCount = 0;
	if (_TryMakeLianDui(byGrade, byCount) != TYPE_GVP)
	{
		if (byGrade + byCount <= 9) //3-Q
			return TYPE_LIANDUI;
	}
	//���Գ��е���
	byGrade = GRADE_INIT;
	byCount = 0;
	if (_MakeSingle(byChair1, byGrade, byCount) != TYPE_GVP)
	{
		if (byGrade <= 9) //3-Q
			return TYPE_SINGLE;
	}
	//���Գ��ж���
	byGrade = GRADE_INIT;
	byCount = 0;
	if (_MakePair(byChair1, byGrade, byCount) != TYPE_GVP)
	{
		if (byGrade <= 9) //3-Q
			return TYPE_PAIR;
	}
	//-------------------------------------------------------------------------
	//���Գ���˳��
	byGrade = GRADE_INIT;
	byCount = 0;
	if (_TryMakeShunZi(byGrade, byCount) != TYPE_GVP)
		return TYPE_SHUNZI;
	//���Գ�������
	byGrade = GRADE_INIT;
	byCount = 0;
	if (_TryMakeLianDui(byGrade, byCount) != TYPE_GVP)
		return TYPE_LIANDUI;
	//���Գ������
	byGrade = GRADE_INIT;
	byCount = 0;
	if (_MakePair(byChair1, byGrade, byCount) != TYPE_GVP)
		return TYPE_PAIR;
	//���Գ�����
	byGrade = GRADE_INIT;
	byCount = 0;
	if (_MakeSingle(byChair1, byGrade, byCount) != TYPE_GVP)
	{
		if (byGrade <= 12) //3-2
			return TYPE_SINGLE;
	}
	//���Գ�ը��
	byGrade = GRADE_INIT;
	byCount = 0;
	if (_MakeBomb(byChair1, byGrade, byCount) != TYPE_GVP)
		return TYPE_BOMB;
	//���Գ���
	byGrade = GRADE_INIT;
	byCount = 0;
	if (_MakeCannon(byChair1, byGrade, byCount) != TYPE_GVP)
		return TYPE_CANNON;
	//���Գ�����
	if (_MakeRedFlag(byChair1, byGrade, byCount) != TYPE_GVP)
		return TYPE_REDFLAG;
	//���Գ����
	if (_MakeRocket(byChair1, byGrade, byCount) != TYPE_GVP)
		return TYPE_ROCKET;

	//------------------��ͷת��
	word _bitmap = bitmap[0] | bitmap[1] | bitmap[2] | bitmap[3];
	byGrade = TblBitIndex[(_bitmap & -_bitmap) % 37];
	byCount = 1;
	return TYPE_SINGLE;
}

byte CRPlayer::_MakeSingle(byte byChair1, byte &byGrade, byte &byCount)
{
	if (byChair1 == 2)
	{
		//�Լ�û�л�ʤ���࣬�Լ��л�ʤ���࣬�ƽϴ�ʱ������
		if (ReleasePlayer(byChair1) || (1 << byGrade) >= CARD_Q)
			return TYPE_GVP;
	}

	byte byRtnType = TYPE_GVP;

	//��˳����Q�����ɲ�˫�����ɲ𣬸���Բ��ɲ�
	word prhbmp = TblShunPrhH[bitmap[0] & MASK_SHUNZI]
				| TblShunPrhH[bitmap[1] & MASK_SHUNZI]
				| bitmap[1] & ~TblShunPrhS[(bitmap[0] | bitmap[1]) & MASK_SHUNZI] & MASK_SHUNZI_Q;

	word _bitmap = bitmap[0] | bitmap[1]/* | bitmap[2] | bitmap[3]*/; //Ӧ���Ų���ը��
	_bitmap ^= prhbmp;
	_bitmap &= TblContraryMask[byGrade];
	while (_bitmap)
	{
		byGrade = TblBitIndex[(_bitmap & -_bitmap) % 37];
		if (_CheckStupid(TYPE_SINGLE, byGrade, byCount))
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

byte CRPlayer::_MakePair(byte byChair1, byte &byGrade, byte &byCount)
{
	if (byChair1 == 2)
	{
		//�Լ�û�л�ʤ���࣬�Լ��л�ʤ���࣬�ƽϴ�ʱ������
		if (ReleasePlayer(byChair1) || (1 << byGrade) >= CARD_J)
			return TYPE_GVP;
	}

	byte byRtnType = TYPE_GVP;

	word _bitmap = bitmap[1] | bitmap[2]/* | bitmap[3]*/; //Ӧ����Ӳ�����

	word _bitmap4 = bitmap[0] | bitmap[1] | bitmap[2] | bitmap[3];
	_bitmap4 &= MASK_SHUNZI;
	_bitmap ^= ~TblShunPrhS[_bitmap4] & bitmap[2]; //��������ը��
	_bitmap &= TblContraryMask[byGrade];
	while (_bitmap)
	{
		byGrade = TblBitIndex[(_bitmap & -_bitmap) % 37];
		if (_CheckStupid(TYPE_PAIR, byGrade, byCount))
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

byte CRPlayer::_MakeBomb(byte byChair1, byte &byGrade, byte &byCount)
{
	if (!SuppressPlayer(byChair1))
		return TYPE_GVP;

	if (byChair1 == 2)
		return TYPE_GVP;

	word _bitmap = bitmap[2] | bitmap[3];
	_bitmap &= TblContraryMask[byGrade];
	while (_bitmap)
	{
		byGrade = TblBitIndex[(_bitmap & -_bitmap) % 37];
		if (_CheckStupid(TYPE_BOMB, byGrade, byCount))
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

byte CRPlayer::_MakeCannon(byte byChair1, byte &byGrade, byte &byCount)
{
	if (!SuppressPlayer(byChair1))
		return TYPE_GVP;

	if (byChair1 == 2)
		return TYPE_GVP;

	word _bitmap = bitmap[3];
	_bitmap &= TblContraryMask[byGrade];
	while (_bitmap)
	{
		byGrade = TblBitIndex[(_bitmap & -_bitmap) % 37];
		if (_CheckStupid(TYPE_CANNON, byGrade, byCount))
		{
			_bitmap &= TblContraryMask[byGrade];
			continue;
		}
		return TYPE_CANNON;
	}
	byGrade = GRADE_INIT;
	byCount = 0;
	return TYPE_GVP;
}

byte CRPlayer::_MakeRedFlag(byte byChair1, byte &byGrade, byte &byCount)
{
	if (!SuppressPlayer(byChair1))
		return TYPE_GVP;

	if (byChair1 == 2)
		return TYPE_GVP;

	if ((bitmap[0] & MASK_REDFLAG) == MASK_REDFLAG)
		return TYPE_REDFLAG;

	return TYPE_GVP;
}

byte CRPlayer::_MakeRocket(byte byChair1, byte &byGrade, byte &byCount)
{
	if (!SuppressPlayer(byChair1))
		return TYPE_GVP;

	if (byChair1 == 2)
		return TYPE_GVP;

	if (m_bShieldRocket)
		return TYPE_ROCKET;

	return TYPE_GVP;
}

byte CRPlayer::_MakeShunZi(byte byChair1, byte &byGrade, byte &byCount)
{
	if (byChair1 == 2)
	{
		//�Լ�û�л�ʤ���࣬�Լ��л�ʤ���࣬�ƽϴ�ʱ������
		if ((m_byWinRhythm & 0x01) == 0 || (m_byWinRhythm & 0x04) != 0 || (1 << byGrade) >= CARD_10)
			return TYPE_GVP;
	}

	byte byRtnType = TYPE_GVP;

	word _bitmap = bitmap[0] | bitmap[1] | bitmap[2] | bitmap[3];
	_bitmap &= MASK_SHUNZI;
	_bitmap &= TblContraryMask[byGrade]; //�������

	word mask;
	while (mask = TblShunZi[_bitmap])
	{
		if (WORD_BIT_COUNT(mask) >= byCount)
		{
			byGrade = TblBitIndex[(mask & -mask) % 37];
			if (_CheckStupid(TYPE_SHUNZI, byGrade, byCount))
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

byte CRPlayer::_MakeLianDui(byte byChair1, byte &byGrade, byte &byCount)
{
	byte byRtnType = TYPE_GVP;

	word _bitmap = bitmap[1] | bitmap[2] | bitmap[3];
	_bitmap &= MASK_SHUNZI;
	_bitmap &= TblContraryMask[byGrade]; //�������

	word mask;
	while (mask = TblShunZi[_bitmap])
	{
		if (WORD_BIT_COUNT(mask) >= byCount)
		{
			byGrade = TblBitIndex[(mask & -mask) % 37];
			if (_CheckStupid(TYPE_LIANDUI, byGrade, byCount))
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

byte CRPlayer::_MakeBCRR(byte byChair1, byte &byGrade, byte &byCount)
{
	byte byRtnType = TYPE_GVP;

	byRtnType = _MakeBomb(byChair1, byGrade, byCount); CHECK_RETURN(byRtnType);
	byRtnType = _MakeCannon(byChair1, byGrade, byCount); CHECK_RETURN(byRtnType);
	byRtnType = _MakeRedFlag(byChair1, byGrade, byCount); CHECK_RETURN(byRtnType);
	byRtnType = _MakeRocket(byChair1, byGrade, byCount); CHECK_RETURN(byRtnType);

	return TYPE_GVP;
}

byte CRPlayer::_MakeCha(byte byChair1, byte &byGrade, byte &byCount)
{
	//��Ϊ���λ�����µĲ��ܲ�
	word _bitmap = bitmap[1] | bitmap[2];
	if ((_bitmap & (1 << byGrade)) == 0)
		return TYPE_GVP;

	if (_CheckStupid(TYPE_CHA, byGrade, 2))
		return TYPE_GVP;

	return TYPE_CHA;
}

byte CRPlayer::_MakeGou(byte byChair1, byte &byGrade, byte &byCount)
{
	//��Ϊ���λ�����µĲ��ܹ�
	if ((bitmap[0] & (1 << byGrade)) == 0)
		return TYPE_GVP;

	if (_CheckStupid(TYPE_GOU, byGrade, 1))
		return TYPE_GVP;

	if (byChair1 == 2)
	{
		//�Լҵ�A��2����
		if (byGrade == 11 || byGrade == 12)
			return TYPE_GVP;
		//˳��ĩ�˲���
		word _bitmap = bitmap[0] | bitmap[1] | bitmap[2] | bitmap[3];
		_bitmap &= MASK_SHUNZI;
		if (TblShunPrhE[_bitmap] & (1 << byGrade))
			return TYPE_GVP;
	}

	return TYPE_GOU;
}

byte CRPlayer::_TryMakeShunZi(byte &byGrade, byte &byCount)
{
	word _bitmap = bitmap[0] | bitmap[1] | bitmap[2] | bitmap[3];
	_bitmap &= MASK_SHUNZI;
	if (byCount == 0) //��һ�μ���˳��
		byCount = 12;
	for ( ; byCount >= 3; byCount--)
	{
		word __bitmap = _bitmap;
		while (__bitmap != 0)
		{
			__bitmap &= TblContraryMask[byGrade];
			word mask = TblShunZi[__bitmap];
			if (WORD_BIT_COUNT(mask) >= byCount)
			{
				byGrade = TblBitIndex[(mask & -mask) % 37];
				if (_CheckStupid(TYPE_SHUNZI, byGrade, byCount))
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

byte CRPlayer::_TryMakeLianDui(byte &byGrade, byte &byCount)
{
	word _bitmap = bitmap[1] | bitmap[2] | bitmap[3];
	_bitmap &= MASK_SHUNZI;
	if (byCount == 0) //��һ�μ�������
		byCount = 12;
	for ( ; byCount >= 3; byCount--)
	{
		word __bitmap = _bitmap;
		while (__bitmap != 0)
		{
			__bitmap &= TblContraryMask[byGrade];
			word mask = TblShunZi[__bitmap];
			if (WORD_BIT_COUNT(mask) >= byCount)
			{
				byGrade = TblBitIndex[(mask & -mask) % 37];
				if (_CheckStupid(TYPE_LIANDUI, byGrade, byCount))
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
