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
	//坚决不能拆火箭
	bool bHasRocket = HasRocket();

	if (bHasRocket)
		OutCards(TYPE_ROCKET, 0, 0);

	//屏蔽火箭影响后的计算
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

	//不连续的单张、对子数（除2，王）
	word _mask = ~mask & (bitmap[0] | bitmap[1]);
	_mask &= MASK_SHUNZI;
	byWinRate = WORD_BIT_COUNT(_mask);

	//检视顺子的状况
	while (mask)
	{
		word shunzi = TblShunZi[mask];
		mask ^= shunzi;

		//是连对
		if (TblShunZi[shunzi & (bitmap[1] | bitmap[2] | bitmap[3])])
		{
			//不是纯粹的连对
			if ((shunzi & bitmap[1]) != shunzi)
				byWinRate++;
			continue;
		}
		//不是以K或A终结的顺子
		if ((shunzi & CARD_K) == 0)
			byWinRate++;

		//不是纯粹的顺子
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
	//坚决不能拆火箭
	m_bShieldRocket = HasRocket();

	if (m_bShieldRocket)
		OutCards(TYPE_ROCKET, 0, 0);

	//屏蔽火箭影响后的检测
	bool bStupidPut = _CheckStupid(byType, byGrade, byCount);

	if (m_bShieldRocket)
		InCards(TYPE_ROCKET, 0, 0);

	return bStupidPut;
}

bool CRPlayer::_CheckStupid(byte byType, byte byGrade, byte byCount)
{
	//旧的零散单张，对子
	word old = GetOddmentMask();

	//新的零散单张，对子
	OutCards(byType, byGrade, byCount);
	word new1 = GetOddmentMask();
	InCards(byType, byGrade, byCount);

	if (_CheckStupid_Oddment(byType, byGrade, byCount, old, new1)) //零散牌检测
		return true;
	if (_CheckStupid_OnesOwn(byType, byGrade, byCount, old, new1)) //按类型独立检测
		return true;
	if (_CheckStupid_Together(byType, byGrade, byCount, old, new1)) //按类型共同检测
		return true;

	//TODO:其他检测
	return false;
}

bool CRPlayer::_CheckStupid_Oddment(byte byType, byte byGrade, byte byCount, word old, word new1)
{
	//比较新旧零散单张，对子的数量
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
	//分别对单张，对子，顺子，炸弹做单独检测
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
	//如果出的是单张或对子，拆了炸弹，要看拆的是否合理
	if (byType == TYPE_SINGLE || byType == TYPE_PAIR || byType == TYPE_CHA)
	{
		if (bitmap[2] & (1 << byGrade))
		{
			//新的零散单张，对子
			bitmap[2] ^= 1 << byGrade;
			word new2 = GetOddmentMask();

			bitmap[2] ^= 1 << byGrade;

			//叉炸一致时，优先选择叉
			if (byType == TYPE_CHA)
			{
				if (WORD_BIT_COUNT(new2) == WORD_BIT_COUNT(new1))
					return false;
			}

			//三连的顺子
			word _bitmap = bitmap[0] | bitmap[1] | bitmap[2] | bitmap[3];
			_bitmap &= MASK_SHUNZI;
			if (TblSanShun[_bitmap] & (1 << byGrade))
			{
				//去除两阶以下的牌
				_bitmap &= TblContraryMask[byGrade <= 2 ? GRADE_INIT : byGrade - 2];

				//得到顺子的余码（挖去炸弹）
				word mask = TblShunZi[_bitmap];
				mask ^= 1 << byGrade;

				//两单张必须叉
				if ((bitmap[0] & mask) == mask)
					return false;
				//一个为单张，一个为对子，叉炸差不多
				if ((bitmap[0] & mask) && (bitmap[1] & mask))
					return false;
				//两对叉不如炸
				if ((bitmap[1] & mask) == mask)
					return true;
				//一个为单张or对子，一个为炸弹or炮，选择炸
				if (((bitmap[0] | bitmap[1]) & mask) && ((bitmap[2] | bitmap[3]) & mask))
					return true;
			}

			//优先选择炸
			if (WORD_BIT_COUNT(new2) < WORD_BIT_COUNT(new1))
				return true;
		}
	}
	return false;
}

bool CRPlayer::_CheckStupid_Single(byte byGrade, byte byCount, word old, word new1)
{
	//两端单张禁止
	word _bitmap = bitmap[0] | bitmap[1] | bitmap[2] | bitmap[3];
	_bitmap &= MASK_SHUNZI;
	if (TblShunPrhE[_bitmap] & (1 << byGrade))
	{
		if (bitmap[0] & (1 << byGrade))
			return true;
	}
	//夹于单张中间禁止
	if (bitmap[2] & (1 << byGrade))
	{
		word mask = CARD_3 | CARD_5;
		if (byGrade != 0 && byGrade <= 10) //A可乱拆
			mask <<= byGrade - 1;
		if ((bitmap[0] & mask) == mask)
			return true;
	}
	return false;
}

bool CRPlayer::_CheckStupid_Pair(byte byGrade, byte byCount, word old, word new1)
{
	//夹于对子中间禁止
	if (bitmap[2] & (1 << byGrade))
	{
		word mask = CARD_3 | CARD_5;
		if (byGrade != 0 && byGrade <= 10) //A可乱拆
			mask <<= byGrade - 1;
		if ((bitmap[1] & mask) == mask)
			return true;
	}
	return false;
}

bool CRPlayer::_CheckStupid_ShunZi(byte byGrade, byte byCount, word old, word new1)
{
	//顺子同时是双龙时不拆
	word mask = SHIFT_1(byGrade + byCount) ^ SHIFT_1(byGrade);
	if ((bitmap[1] & mask) == mask)
		return true;

	if (byCount == 3)
	{
		//三张顺子含炮
		if (bitmap[3] & mask)
		{
			//不是两个单张
			if (WORD_BIT_COUNT(bitmap[0] & mask) != 2)
				return true;
		}
	}

	//首末端炸弹或炮
	if ((bitmap[2] | bitmap[3]) & TblShunPrhE[mask])
		return true;

	//拆散两个以上炸弹
	if (WORD_BIT_COUNT(bitmap[2] & mask) >= 2)
		return true;

	return false;
}

bool CRPlayer::_CheckStupid_Bomb(byte byGrade, byte byCount, word old, word new1)
{
	if (bitmap[3] & (1 << byGrade))
	{
		//新的零散单张，对子
		bitmap[3] ^= 1 << byGrade;
		word new2 = GetOddmentMask();

		bitmap[3] ^= 1 << byGrade;

		word _bitmap = bitmap[0] | bitmap[1] | bitmap[2] | bitmap[3];
		_bitmap &= MASK_SHUNZI;
		if (TblSanShun[_bitmap] & (1 << byGrade))
		{
			//去除两阶以下的牌
			_bitmap &= TblContraryMask[byGrade <= 2 ? GRADE_INIT : byGrade - 2];

			//得到顺子的余码（挖去炮）
			word mask = TblShunZi[_bitmap];
			mask ^= 1 << byGrade;

			//两单张必须出炸弹
			if ((bitmap[0] & mask) == mask)
				return false;
			//一个为单张，一个为对子，炮炸弹差不多
			if ((bitmap[0] & mask) && (bitmap[1] & mask))
				return true;
			//两对炸弹不如炮
			if ((bitmap[1] & mask) == mask)
				return true;
			//一个为单张or对子，一个为炸弹or炮，选择炮
			if (((bitmap[0] | bitmap[1]) & mask) && ((bitmap[2] | bitmap[3]) & mask))
				return true;
		}

		//优先选择炮
		if (WORD_BIT_COUNT(new2) < WORD_BIT_COUNT(new1))
			return true;
	}
	return false;
}

byte CRPlayer::MakeCards(byte byChair1, byte byType, byte &byGrade, byte &byCount)
{
	//坚决不能拆火箭
	m_bShieldRocket = HasRocket();

	if (m_bShieldRocket)
		OutCards(TYPE_ROCKET, 0, 0);

	//屏蔽火箭影响后的出牌
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
	//--------------------------------战略进攻---------------------------------
	//尝试攻顺子
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
	//尝试攻单张
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
	//尝试攻对子
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

	//--------------------------------战略防御---------------------------------
	//尝试出小顺子
	byGrade = GRADE_INIT;
	byCount = 0;
	if (_TryMakeShunZi(byGrade, byCount) != TYPE_GVP)
	{
		if (byGrade + byCount - 1 <= 7) //3-10
			return TYPE_SHUNZI;
	}
	//尝试出小连对
	byGrade = GRADE_INIT;
	byCount = 0;
	if (_TryMakeLianDui(byGrade, byCount) != TYPE_GVP)
	{
		if (byGrade + byCount <= 7) //3-10
			return TYPE_LIANDUI;
	}
	//尝试出小单张
	byGrade = GRADE_INIT;
	byCount = 0;
	if (_MakeSingle(byChair1, byGrade, byCount) != TYPE_GVP)
	{
		if (byGrade <= 7) //3-10
			return TYPE_SINGLE;
	}
	//尝试出小对子
	byGrade = GRADE_INIT;
	byCount = 0;
	if (_MakePair(byChair1, byGrade, byCount) != TYPE_GVP)
	{
		if (byGrade <= 7) //3-10
			return TYPE_PAIR;
	}
	//-------------------------------------------------------------------------
	//尝试出中顺子
	byGrade = GRADE_INIT;
	byCount = 0;
	if (_TryMakeShunZi(byGrade, byCount) != TYPE_GVP)
	{
		if (byGrade + byCount - 1 <= 9) //3-Q
			return TYPE_SHUNZI;
	}
	//尝试出中连对
	byGrade = GRADE_INIT;
	byCount = 0;
	if (_TryMakeLianDui(byGrade, byCount) != TYPE_GVP)
	{
		if (byGrade + byCount <= 9) //3-Q
			return TYPE_LIANDUI;
	}
	//尝试出中单张
	byGrade = GRADE_INIT;
	byCount = 0;
	if (_MakeSingle(byChair1, byGrade, byCount) != TYPE_GVP)
	{
		if (byGrade <= 9) //3-Q
			return TYPE_SINGLE;
	}
	//尝试出中对子
	byGrade = GRADE_INIT;
	byCount = 0;
	if (_MakePair(byChair1, byGrade, byCount) != TYPE_GVP)
	{
		if (byGrade <= 9) //3-Q
			return TYPE_PAIR;
	}
	//-------------------------------------------------------------------------
	//尝试出大顺子
	byGrade = GRADE_INIT;
	byCount = 0;
	if (_TryMakeShunZi(byGrade, byCount) != TYPE_GVP)
		return TYPE_SHUNZI;
	//尝试出大连对
	byGrade = GRADE_INIT;
	byCount = 0;
	if (_TryMakeLianDui(byGrade, byCount) != TYPE_GVP)
		return TYPE_LIANDUI;
	//尝试出大对子
	byGrade = GRADE_INIT;
	byCount = 0;
	if (_MakePair(byChair1, byGrade, byCount) != TYPE_GVP)
		return TYPE_PAIR;
	//尝试出大单张
	byGrade = GRADE_INIT;
	byCount = 0;
	if (_MakeSingle(byChair1, byGrade, byCount) != TYPE_GVP)
	{
		if (byGrade <= 12) //3-2
			return TYPE_SINGLE;
	}
	//尝试出炸弹
	byGrade = GRADE_INIT;
	byCount = 0;
	if (_MakeBomb(byChair1, byGrade, byCount) != TYPE_GVP)
		return TYPE_BOMB;
	//尝试出炮
	byGrade = GRADE_INIT;
	byCount = 0;
	if (_MakeCannon(byChair1, byGrade, byCount) != TYPE_GVP)
		return TYPE_CANNON;
	//尝试出红旗
	if (_MakeRedFlag(byChair1, byGrade, byCount) != TYPE_GVP)
		return TYPE_REDFLAG;
	//尝试出火箭
	if (_MakeRocket(byChair1, byGrade, byCount) != TYPE_GVP)
		return TYPE_ROCKET;

	//------------------晕头转向
	word _bitmap = bitmap[0] | bitmap[1] | bitmap[2] | bitmap[3];
	byGrade = TblBitIndex[(_bitmap & -_bitmap) % 37];
	byCount = 1;
	return TYPE_SINGLE;
}

byte CRPlayer::_MakeSingle(byte byChair1, byte &byGrade, byte &byCount)
{
	if (byChair1 == 2)
	{
		//自己没有获胜节奏，对家有获胜节奏，牌较大时，放弃
		if (ReleasePlayer(byChair1) || (1 << byGrade) >= CARD_Q)
			return TYPE_GVP;
	}

	byte byRtnType = TYPE_GVP;

	//单顺（到Q）不可拆，双龙不可拆，隔离对不可拆
	word prhbmp = TblShunPrhH[bitmap[0] & MASK_SHUNZI]
				| TblShunPrhH[bitmap[1] & MASK_SHUNZI]
				| bitmap[1] & ~TblShunPrhS[(bitmap[0] | bitmap[1]) & MASK_SHUNZI] & MASK_SHUNZI_Q;

	word _bitmap = bitmap[0] | bitmap[1]/* | bitmap[2] | bitmap[3]*/; //应答单张不拆炸弹
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
		//自己没有获胜节奏，对家有获胜节奏，牌较大时，放弃
		if (ReleasePlayer(byChair1) || (1 << byGrade) >= CARD_J)
			return TYPE_GVP;
	}

	byte byRtnType = TYPE_GVP;

	word _bitmap = bitmap[1] | bitmap[2]/* | bitmap[3]*/; //应答对子不拆炮

	word _bitmap4 = bitmap[0] | bitmap[1] | bitmap[2] | bitmap[3];
	_bitmap4 &= MASK_SHUNZI;
	_bitmap ^= ~TblShunPrhS[_bitmap4] & bitmap[2]; //不拆不连的炸弹
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
		//自己没有获胜节奏，对家有获胜节奏，牌较大时，放弃
		if ((m_byWinRhythm & 0x01) == 0 || (m_byWinRhythm & 0x04) != 0 || (1 << byGrade) >= CARD_10)
			return TYPE_GVP;
	}

	byte byRtnType = TYPE_GVP;

	word _bitmap = bitmap[0] | bitmap[1] | bitmap[2] | bitmap[3];
	_bitmap &= MASK_SHUNZI;
	_bitmap &= TblContraryMask[byGrade]; //更大的牌

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
		_bitmap ^= mask; //顺子长度不够
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
	_bitmap &= TblContraryMask[byGrade]; //更大的牌

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
		_bitmap ^= mask; //连对长度不够
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
	//因为屏蔽火箭导致的不能叉
	word _bitmap = bitmap[1] | bitmap[2];
	if ((_bitmap & (1 << byGrade)) == 0)
		return TYPE_GVP;

	if (_CheckStupid(TYPE_CHA, byGrade, 2))
		return TYPE_GVP;

	return TYPE_CHA;
}

byte CRPlayer::_MakeGou(byte byChair1, byte &byGrade, byte &byCount)
{
	//因为屏蔽火箭导致的不能勾
	if ((bitmap[0] & (1 << byGrade)) == 0)
		return TYPE_GVP;

	if (_CheckStupid(TYPE_GOU, byGrade, 1))
		return TYPE_GVP;

	if (byChair1 == 2)
	{
		//对家的A或2不勾
		if (byGrade == 11 || byGrade == 12)
			return TYPE_GVP;
		//顺子末端不勾
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
	if (byCount == 0) //第一次计算顺子
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
		byGrade = GRADE_INIT; //没有足够张数的顺子
	}
	byGrade = GRADE_INIT;
	byCount = 0;
	return TYPE_GVP;
}

byte CRPlayer::_TryMakeLianDui(byte &byGrade, byte &byCount)
{
	word _bitmap = bitmap[1] | bitmap[2] | bitmap[3];
	_bitmap &= MASK_SHUNZI;
	if (byCount == 0) //第一次计算连对
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
		byGrade = GRADE_INIT; //没有足够张数的连对
	}
	byGrade = GRADE_INIT;
	byCount = 0;
	return TYPE_GVP;
}
