#include "StdAfx.h"
#include "RPlayer.h"

#define SHIFT_1(n)				((1 << (n)) - 1)

bool CRPlayer::CheckBeat(byte byType, byte byGrade, byte byCount, word wAttach)
{
	//非严格检查
	if (bitmap[3] != 0 || (bitmap[0] & MASK_ROCKET) == MASK_ROCKET)
		return true;

	word _bitmap = 0;
	switch (byType)
	{
	case TYPE_GVP:
		return true;
	case TYPE_SINGLE:
		return ((bitmap[0] | bitmap[1] | bitmap[2] | bitmap[3]) & TblContraryMask[byGrade]) != 0;
	case TYPE_PAIR:
		return ((bitmap[1] | bitmap[2] | bitmap[3]) & TblContraryMask[byGrade]) != 0;
	case TYPE_TRIP:
	case TYPE_TRIP_S:
	case TYPE_TRIP_P:
		return ((bitmap[2] | bitmap[3]) & TblContraryMask[byGrade]) != 0;
	case TYPE_PLANE:
	case TYPE_PLANE_S:
	case TYPE_PLANE_P:
		_bitmap = bitmap[2] | bitmap[3];
		_bitmap &= MASK_SHUNZI;
		_bitmap &= TblContraryMask[byGrade];
		return WORD_BIT_COUNT(TblShunZi2[_bitmap]) >= byCount;
	case TYPE_BOMB:
	case TYPE_QUAD_S:
	case TYPE_QUAD_P:
		return (bitmap[3] & TblContraryMask[byGrade]) != 0;
	case TYPE_SHUNZI:
		_bitmap = bitmap[0] | bitmap[1] | bitmap[2] | bitmap[3];
		_bitmap &= MASK_SHUNZI;
		_bitmap &= TblContraryMask[byGrade];
		return WORD_BIT_COUNT(TblShunZi5[_bitmap]) >= byCount;
	case TYPE_LIANDUI:
		_bitmap = bitmap[1] | bitmap[2] | bitmap[3];
		_bitmap &= MASK_SHUNZI;
		_bitmap &= TblContraryMask[byGrade];
		return WORD_BIT_COUNT(TblShunZi3[_bitmap]) >= byCount;
	case TYPE_ROCKET:
		return false;
	}
	return true;
}

byte CRPlayer::RspCards(byte byType, byte byRspType, byte &byGrade, byte &byCount, word &wAttach, word &wChoice)
{
	switch (byType)
	{
	case TYPE_GVP:
		return _RspGvp(byRspType, byGrade, byCount, wAttach, wChoice);
	case TYPE_SINGLE:
		return _RspSingle(byRspType, byGrade, byCount);
	case TYPE_PAIR:
		return _RspPair(byRspType, byGrade, byCount);
	case TYPE_TRIP:
		return _RspTrip(byRspType, byGrade, byCount);
	case TYPE_TRIP_S:
		return _RspTripS(byRspType, byGrade, byCount, wAttach, wChoice);
	case TYPE_TRIP_P:
		return _RspTripP(byRspType, byGrade, byCount, wAttach, wChoice);
	case TYPE_PLANE:
		return _RspPlane(byRspType, byGrade, byCount);
	case TYPE_PLANE_S:
		return _RspPlaneS(byRspType, byGrade, byCount, wAttach, wChoice);
	case TYPE_PLANE_P:
		return _RspPlaneP(byRspType, byGrade, byCount, wAttach, wChoice);
	case TYPE_QUAD_S:
		return _RspQuadS(byRspType, byGrade, byCount, wAttach, wChoice);
	case TYPE_QUAD_P:
		return _RspQuadP(byRspType, byGrade, byCount, wAttach, wChoice);
	case TYPE_SHUNZI:
		return _RspShunZi(byRspType, byGrade, byCount);
	case TYPE_LIANDUI:
		return _RspLianDui(byRspType, byGrade, byCount);
	case TYPE_BOMB:
		return _RspBomb(byRspType, byGrade, byCount);
	case TYPE_ROCKET:
		return _RspRocket(byRspType, byGrade, byCount);
	}
	return TYPE_GVP;
}

byte CRPlayer::_RspGvp(byte byRspType, byte &byGrade, byte &byCount, word &wAttach, word &wChoice)
{
	byte byRtnType = TYPE_GVP;
	switch (byRspType)
	{
	case TYPE_GVP:
		byGrade = GRADE_INIT;
		byCount = 0;
	case TYPE_PLANE_S:
		byRtnType = _RspAnyPlaneS(byGrade, byCount, wAttach, wChoice); CHECK_RETURN(byRtnType);
	case TYPE_PLANE_P:
		byRtnType = _RspAnyPlaneP(byGrade, byCount, wAttach, wChoice); CHECK_RETURN(byRtnType);
	case TYPE_LIANDUI:
		byRtnType = _RspAnyLianDui(byGrade, byCount); CHECK_RETURN(byRtnType);
	case TYPE_SHUNZI:
		byRtnType = _RspAnyShunZi(byGrade, byCount); CHECK_RETURN(byRtnType);
	case TYPE_TRIP_S:
		byRtnType = _RspAnyTripS(byGrade, byCount, wAttach, wChoice); CHECK_RETURN(byRtnType);
	case TYPE_TRIP_P:
		byRtnType = _RspAnyTripP(byGrade, byCount, wAttach, wChoice); CHECK_RETURN(byRtnType);
	case TYPE_PLANE:
		byRtnType = _RspAnyPlane(byGrade, byCount); CHECK_RETURN(byRtnType);
	case TYPE_QUAD_S:
		byRtnType = _RspAnyQuadS(byGrade, byCount, wAttach, wChoice); CHECK_RETURN(byRtnType);
	case TYPE_QUAD_P:
		byRtnType = _RspAnyQuadP(byGrade, byCount, wAttach, wChoice); CHECK_RETURN(byRtnType);
	case TYPE_TRIP:
		byRtnType = _RspAnyTrip(byGrade, byCount); CHECK_RETURN(byRtnType);
	case TYPE_PAIR:
		byRtnType = _RspAnyPair(byGrade, byCount); CHECK_RETURN(byRtnType);
	case TYPE_SINGLE:
		byRtnType = _RspAnySingle(byGrade, byCount); CHECK_RETURN(byRtnType);
	case TYPE_BOMB:
		byRtnType = _RspAnyBomb(byGrade, byCount); CHECK_RETURN(byRtnType);
		byRtnType = _RspAnyRocket(byGrade, byCount); CHECK_RETURN(byRtnType);
	}
	return TYPE_GVP;
}

byte CRPlayer::_RspSingle(byte byRspType, byte &byGrade, byte &byCount)
{
	byte byRtnType = TYPE_GVP;
	switch (byRspType)
	{
	case TYPE_GVP:
	case TYPE_SINGLE:
		byRtnType = _RspSingleBySingle(byGrade, byCount); CHECK_RETURN(byRtnType);
	case TYPE_BOMB:
		byRtnType = _RspAnyBomb(byGrade, byCount); CHECK_RETURN(byRtnType);
		byRtnType = _RspAnyRocket(byGrade, byCount); CHECK_RETURN(byRtnType);
	}
	return TYPE_GVP;
}

byte CRPlayer::_RspPair(byte byRspType, byte &byGrade, byte &byCount)
{
	byte byRtnType = TYPE_GVP;
	switch (byRspType)
	{
	case TYPE_GVP:
	case TYPE_PAIR:
		byRtnType = _RspPairByPair(byGrade, byCount); CHECK_RETURN(byRtnType);
	case TYPE_BOMB:
		byRtnType = _RspAnyBomb(byGrade, byCount); CHECK_RETURN(byRtnType);
		byRtnType = _RspAnyRocket(byGrade, byCount); CHECK_RETURN(byRtnType);
	}
	return TYPE_GVP;
}

byte CRPlayer::_RspTrip(byte byRspType, byte &byGrade, byte &byCount)
{
	byte byRtnType = TYPE_GVP;
	switch (byRspType)
	{
	case TYPE_GVP:
	case TYPE_TRIP:
		byRtnType = _RspAnyTrip(byGrade, byCount); CHECK_RETURN(byRtnType);
	case TYPE_BOMB:
		byRtnType = _RspAnyBomb(byGrade, byCount); CHECK_RETURN(byRtnType);
		byRtnType = _RspAnyRocket(byGrade, byCount); CHECK_RETURN(byRtnType);
	}
	return TYPE_GVP;
}

byte CRPlayer::_RspTripS(byte byRspType, byte &byGrade, byte &byCount, word &wAttach, word &wChoice)
{
	byte byRtnType = TYPE_GVP;
	switch (byRspType)
	{
	case TYPE_GVP:
	case TYPE_TRIP_S:
		byRtnType = _RspAnyTripS(byGrade, byCount, wAttach, wChoice); CHECK_RETURN(byRtnType);
	case TYPE_BOMB:
		byRtnType = _RspAnyBomb(byGrade, byCount); CHECK_RETURN(byRtnType);
		byRtnType = _RspAnyRocket(byGrade, byCount); CHECK_RETURN(byRtnType);
	}
	return TYPE_GVP;
}

byte CRPlayer::_RspTripP(byte byRspType, byte &byGrade, byte &byCount, word &wAttach, word &wChoice)
{
	byte byRtnType = TYPE_GVP;
	switch (byRspType)
	{
	case TYPE_GVP:
	case TYPE_TRIP_P:
		byRtnType = _RspAnyTripP(byGrade, byCount, wAttach, wChoice); CHECK_RETURN(byRtnType);
	case TYPE_BOMB:
		byRtnType = _RspAnyBomb(byGrade, byCount); CHECK_RETURN(byRtnType);
		byRtnType = _RspAnyRocket(byGrade, byCount); CHECK_RETURN(byRtnType);
	}
	return TYPE_GVP;
}

byte CRPlayer::_RspPlane(byte byRspType, byte &byGrade, byte &byCount)
{
	byte byRtnType = TYPE_GVP;
	switch (byRspType)
	{
	case TYPE_GVP:
	case TYPE_PLANE:
		byRtnType = _RspAnyPlane(byGrade, byCount); CHECK_RETURN(byRtnType);
	case TYPE_BOMB:
		byRtnType = _RspAnyBomb(byGrade, byCount); CHECK_RETURN(byRtnType);
		byRtnType = _RspAnyRocket(byGrade, byCount); CHECK_RETURN(byRtnType);
	}
	return TYPE_GVP;
}

byte CRPlayer::_RspPlaneS(byte byRspType, byte &byGrade, byte &byCount, word &wAttach, word &wChoice)
{
	byte byRtnType = TYPE_GVP;
	switch (byRspType)
	{
	case TYPE_GVP:
	case TYPE_PLANE_S:
		byRtnType = _RspAnyPlaneS(byGrade, byCount, wAttach, wChoice); CHECK_RETURN(byRtnType);
	case TYPE_BOMB:
		byRtnType = _RspAnyBomb(byGrade, byCount); CHECK_RETURN(byRtnType);
		byRtnType = _RspAnyRocket(byGrade, byCount); CHECK_RETURN(byRtnType);
	}
	return TYPE_GVP;
}

byte CRPlayer::_RspPlaneP(byte byRspType, byte &byGrade, byte &byCount, word &wAttach, word &wChoice)
{
	byte byRtnType = TYPE_GVP;
	switch (byRspType)
	{
	case TYPE_GVP:
	case TYPE_PLANE_P:
		byRtnType = _RspAnyPlaneP(byGrade, byCount, wAttach, wChoice); CHECK_RETURN(byRtnType);
	case TYPE_BOMB:
		byRtnType = _RspAnyBomb(byGrade, byCount); CHECK_RETURN(byRtnType);
		byRtnType = _RspAnyRocket(byGrade, byCount); CHECK_RETURN(byRtnType);
	}
	return TYPE_GVP;
}

byte CRPlayer::_RspQuadS(byte byRspType, byte &byGrade, byte &byCount, word &wAttach, word &wChoice)
{
	byte byRtnType = TYPE_GVP;
	switch (byRspType)
	{
	case TYPE_GVP:
	case TYPE_QUAD_S:
		byRtnType = _RspAnyQuadS(byGrade, byCount, wAttach, wChoice); CHECK_RETURN(byRtnType);
	case TYPE_BOMB:
		byRtnType = _RspAnyBomb(byGrade, byCount); CHECK_RETURN(byRtnType);
		byRtnType = _RspAnyRocket(byGrade, byCount); CHECK_RETURN(byRtnType);
	}
	return TYPE_GVP;
}

byte CRPlayer::_RspQuadP(byte byRspType, byte &byGrade, byte &byCount, word &wAttach, word &wChoice)
{
	byte byRtnType = TYPE_GVP;
	switch (byRspType)
	{
	case TYPE_GVP:
	case TYPE_QUAD_P:
		byRtnType = _RspAnyQuadP(byGrade, byCount, wAttach, wChoice); CHECK_RETURN(byRtnType);
	case TYPE_BOMB:
		byRtnType = _RspAnyBomb(byGrade, byCount); CHECK_RETURN(byRtnType);
		byRtnType = _RspAnyRocket(byGrade, byCount); CHECK_RETURN(byRtnType);
	}
	return TYPE_GVP;
}

byte CRPlayer::_RspShunZi(byte byRspType, byte &byGrade, byte &byCount)
{
	byte byRtnType = TYPE_GVP;
	word _bitmap = 0;
	switch (byRspType)
	{
	case TYPE_GVP:
	case TYPE_SHUNZI:
		_bitmap = bitmap[0] | bitmap[1] | bitmap[2] | bitmap[3];
		_bitmap &= MASK_SHUNZI;
		_bitmap &= TblContraryMask[byGrade]; //更大的牌

		word mask;
		while (mask = TblShunZi5[_bitmap])
		{
			if (WORD_BIT_COUNT(mask) >= byCount)
			{
				byGrade = TblBitIndex[(mask & -mask) % 37];
				return TYPE_SHUNZI;
			}
			_bitmap ^= mask; //顺子长度不够
		}
		byGrade = GRADE_INIT;
		byCount = 0;
	case TYPE_BOMB:
		byRtnType = _RspAnyBomb(byGrade, byCount); CHECK_RETURN(byRtnType);
		byRtnType = _RspAnyRocket(byGrade, byCount); CHECK_RETURN(byRtnType);
	}
	return TYPE_GVP;
}

byte CRPlayer::_RspLianDui(byte byRspType, byte &byGrade, byte &byCount)
{
	byte byRtnType = TYPE_GVP;
	word _bitmap = 0;
	switch (byRspType)
	{
	case TYPE_GVP:
	case TYPE_LIANDUI:
		_bitmap = bitmap[1] | bitmap[2] | bitmap[3];
		_bitmap &= MASK_SHUNZI;
		_bitmap &= TblContraryMask[byGrade]; //更大的牌

		word mask;
		while (mask = TblShunZi3[_bitmap])
		{
			if (WORD_BIT_COUNT(mask) >= byCount)
			{
				byGrade = TblBitIndex[(mask & -mask) % 37];
				return TYPE_LIANDUI;
			}
			_bitmap ^= mask; //连对长度不够
		}
		byGrade = GRADE_INIT;
		byCount = 0;
	case TYPE_BOMB:
		byRtnType = _RspAnyBomb(byGrade, byCount); CHECK_RETURN(byRtnType);
		byRtnType = _RspAnyRocket(byGrade, byCount); CHECK_RETURN(byRtnType);
	}
	return TYPE_GVP;
}

byte CRPlayer::_RspBomb(byte byRspType, byte &byGrade, byte &byCount)
{
	byte byRtnType = TYPE_GVP;
	switch (byRspType)
	{
	case TYPE_GVP:
	case TYPE_BOMB:
		byRtnType = _RspAnyBomb(byGrade, byCount); CHECK_RETURN(byRtnType);
		byRtnType = _RspAnyRocket(byGrade, byCount); CHECK_RETURN(byRtnType);
	}
	return TYPE_GVP;
}

byte CRPlayer::_RspRocket(byte byRspType, byte &byGrade, byte &byCount)
{
	return TYPE_GVP;
}

byte CRPlayer::_RspSingleBySingle(byte &byGrade, byte &byCount)
{
	//五张顺不可拆，连对不可拆（到Q）
	word prhbmp = (TblFiveShun[bitmap[0] & MASK_SHUNZI]
				| TblSanShun[bitmap[1] & MASK_SHUNZI]) & MASK_SHUNZI_Q;

	word _bitmap = bitmap[0] | bitmap[1] | bitmap[2];
	_bitmap ^= prhbmp;
	_bitmap &= TblContraryMask[byGrade];
	if (_bitmap)
	{
		byGrade = TblBitIndex[(_bitmap & -_bitmap) % 37];
		return TYPE_SINGLE;
	}
	byGrade = GRADE_INIT;
	byCount = 0;
	return TYPE_GVP;
}

byte CRPlayer::_RspPairByPair(byte &byGrade, byte &byCount)
{
	word _bitmap = bitmap[1] | bitmap[2];
	_bitmap &= TblContraryMask[byGrade];
	if (_bitmap)
	{
		byGrade = TblBitIndex[(_bitmap & -_bitmap) % 37];
		return TYPE_PAIR;
	}
	byGrade = GRADE_INIT;
	byCount = 0;
	return TYPE_GVP;
}

byte CRPlayer::_RspAnySingle(byte &byGrade, byte &byCount)
{
	//单顺（到Q）不可拆，连对不可拆，隔离对不可拆
	word prhbmp = (TblFiveShun[bitmap[0] & MASK_SHUNZI]
				| TblSanShun[bitmap[1] & MASK_SHUNZI]
				| bitmap[1] & ~TblShun5PrhS[(bitmap[0] | bitmap[1]) & MASK_SHUNZI]) & MASK_SHUNZI_Q;

	word _bitmap = bitmap[0] | bitmap[1]/* | bitmap[2] | bitmap[3]*/; //主动出牌不拆炸弹
	_bitmap ^= prhbmp;
	if ((bitmap[0] & MASK_ROCKET) == MASK_ROCKET) //主动出牌不拆红旗
		_bitmap &= ~MASK_ROCKET;
	_bitmap &= TblMiddlePrh[_bitmap]; //出牌中间禁止

	_bitmap &= TblContraryMask[byGrade];
	if (_bitmap)
	{
		byGrade = TblBitIndex[(_bitmap & -_bitmap) % 37];
		return TYPE_SINGLE;
	}
	byGrade = GRADE_INIT;
	byCount = 0;
	return TYPE_GVP;
}

byte CRPlayer::_RspAnyPair(byte &byGrade, byte &byCount)
{
	//连对不可拆
	word prhbmp = TblShun3PrhS[bitmap[1] & MASK_SHUNZI];

	//不含炸弹的顺子中的对子不能拆
	word shunzi = bitmap[0] | bitmap[1] | bitmap[2];
	shunzi &= MASK_SHUNZI;
	shunzi = TblShunZi5[shunzi];
	prhbmp |= bitmap[1] & (shunzi ^ TblShun5PrhE[shunzi]);

	word _bitmap = bitmap[1]/* | bitmap[2] | bitmap[3]*/; //主动出牌不拆炸弹
	_bitmap ^= prhbmp;
	_bitmap &= TblContraryMask[byGrade];
	if (_bitmap)
	{
		byGrade = TblBitIndex[(_bitmap & -_bitmap) % 37];
		return TYPE_PAIR;
	}
	byGrade = GRADE_INIT;
	byCount = 0;
	return TYPE_GVP;
}

byte CRPlayer::_RspAnyTrip(byte &byGrade, byte &byCount)
{
	word _bitmap = bitmap[2]/* | bitmap[3]*/;
	_bitmap &= TblContraryMask[byGrade];
	if (_bitmap)
	{
		byGrade = TblBitIndex[(_bitmap & -_bitmap) % 37];
		return TYPE_TRIP;
	}
	byGrade = GRADE_INIT;
	byCount = 0;
	return TYPE_GVP;
}

byte CRPlayer::_RspAnyTripS(byte &byGrade, byte &byCount, word &wAttach, word &wChoice)
{
	while (true)
	{
		if (wChoice == 0)
		{
			if (_RspAnyTrip(byGrade, byCount) == TYPE_GVP)
			{
				byGrade = GRADE_INIT;
				byCount = 0;
				return TYPE_GVP;
			}

			//初始化附加牌
			OutCards(TYPE_TRIP, byGrade, byCount);
			_InitChoice1S(wChoice);
			wChoice &= ~(1 << byGrade); //防撞
			InCards(TYPE_TRIP, byGrade, byCount);
		}

		//改变附加牌
		if (_ChangeAttach1S(wAttach, wChoice))
			return TYPE_TRIP_S;
	}
}

byte CRPlayer::_RspAnyTripP(byte &byGrade, byte &byCount, word &wAttach, word &wChoice)
{
	while (true)
	{
		if (wChoice == 0)
		{
			if (_RspAnyTrip(byGrade, byCount) == TYPE_GVP)
			{
				byGrade = GRADE_INIT;
				byCount = 0;
				return TYPE_GVP;
			}

			//初始化附加牌
			OutCards(TYPE_TRIP, byGrade, byCount);
			_InitChoice1P(wChoice);
			InCards(TYPE_TRIP, byGrade, byCount);
		}

		//改变附加牌
		if (_ChangeAttach1P(wAttach, wChoice))
			return TYPE_TRIP_P;
	}
}

byte CRPlayer::_RspAnyPlane(byte &byGrade, byte &byCount)
{
	word _bitmap = bitmap[2] | bitmap[3];
	_bitmap &= MASK_SHUNZI;
	if (byCount == 0) //第一次计算飞机
		byCount = 4;
	for ( ; byCount >= 2; byCount--)
	{
		word __bitmap = _bitmap;
		__bitmap &= TblContraryMask[byGrade];
		word mask = TblShunZi2[__bitmap];
		if (WORD_BIT_COUNT(mask) >= byCount)
		{
			byGrade = TblBitIndex[(mask & -mask) % 37];
			return TYPE_PLANE;
		}
		byGrade = GRADE_INIT; //没有足够张数的飞机
	}
	byGrade = GRADE_INIT;
	byCount = 0;
	return TYPE_GVP;
}

byte CRPlayer::_RspAnyPlaneS(byte &byGrade, byte &byCount, word &wAttach, word &wChoice)
{
	while (true)
	{
		if (wChoice == 0)
		{
			if (_RspAnyPlane(byGrade, byCount) == TYPE_GVP)
			{
				byGrade = GRADE_INIT;
				byCount = 0;
				return TYPE_GVP;
			}

			//初始化附加牌
			OutCards(TYPE_PLANE, byGrade, byCount);
			if (byCount == 2)
				_InitChoice2S(wChoice);
			else
				_InitChoiceNS(byCount, wChoice);
			wChoice &= ~(SHIFT_1(byGrade + byCount) ^ SHIFT_1(byGrade)); //防撞
			InCards(TYPE_PLANE, byGrade, byCount);
		}

		//改变附加牌
		if (byCount == 2 ? _ChangeAttach2S(wAttach, wChoice) : _ChangeAttachNS(wAttach, wChoice))
			return TYPE_PLANE_S;
	}
}

byte CRPlayer::_RspAnyPlaneP(byte &byGrade, byte &byCount, word &wAttach, word &wChoice)
{
	while (true)
	{
		if (wChoice == 0)
		{
			if (_RspAnyPlane(byGrade, byCount) == TYPE_GVP)
			{
				byGrade = GRADE_INIT;
				byCount = 0;
				return TYPE_GVP;
			}

			//初始化附加牌
			OutCards(TYPE_PLANE, byGrade, byCount);
			if (byCount == 2)
				_InitChoice2P(wChoice);
			else
				_InitChoiceNP(byCount, wChoice);
			InCards(TYPE_PLANE, byGrade, byCount);
		}

		//改变附加牌
		if (byCount == 2 ? _ChangeAttach2P(wAttach, wChoice) : _ChangeAttachNP(wAttach, wChoice))
			return TYPE_PLANE_P;
	}
}

byte CRPlayer::_RspAnyQuadS(byte &byGrade, byte &byCount, word &wAttach, word &wChoice)
{
	while (true)
	{
		if (wChoice == 0)
		{
			if (_RspAnyBomb(byGrade, byCount) == TYPE_GVP)
			{
				byGrade = GRADE_INIT;
				byCount = 0;
				return TYPE_GVP;
			}

			//初始化附加牌
			OutCards(TYPE_BOMB, byGrade, byCount);
			_InitChoice2S(wChoice);
			InCards(TYPE_BOMB, byGrade, byCount);
		}

		//改变附加牌
		if (_ChangeAttach2S(wAttach, wChoice))
			return TYPE_QUAD_S;
	}
}

byte CRPlayer::_RspAnyQuadP(byte &byGrade, byte &byCount, word &wAttach, word &wChoice)
{
	while (true)
	{
		if (wChoice == 0)
		{
			if (_RspAnyBomb(byGrade, byCount) == TYPE_GVP)
			{
				byGrade = GRADE_INIT;
				byCount = 0;
				return TYPE_GVP;
			}

			//初始化附加牌
			OutCards(TYPE_BOMB, byGrade, byCount);
			_InitChoice2P(wChoice);
			InCards(TYPE_BOMB, byGrade, byCount);
		}

		//改变附加牌
		if (_ChangeAttach2P(wAttach, wChoice))
			return TYPE_QUAD_P;
	}
}

byte CRPlayer::_RspAnyBomb(byte &byGrade, byte &byCount)
{
	word _bitmap = bitmap[3];
	_bitmap &= TblContraryMask[byGrade];
	if (_bitmap)
	{
		byGrade = TblBitIndex[(_bitmap & -_bitmap) % 37];
		return TYPE_BOMB;
	}
	byGrade = GRADE_INIT;
	byCount = 0;
	return TYPE_GVP;
}

byte CRPlayer::_RspAnyRocket(byte &byGrade, byte &byCount)
{
	if ((bitmap[0] & MASK_ROCKET) == MASK_ROCKET)
		return TYPE_ROCKET;

	byGrade = GRADE_INIT;
	byCount = 0;
	return TYPE_GVP;
}

byte CRPlayer::_RspAnyShunZi(byte &byGrade, byte &byCount)
{
	word _bitmap = bitmap[0] | bitmap[1] | bitmap[2] | bitmap[3];
	_bitmap &= MASK_SHUNZI;
	if (byCount == 0) //第一次计算顺子
		byCount = 12;
	for ( ; byCount >= 5; byCount--)
	{
		word __bitmap = _bitmap;
		__bitmap &= TblContraryMask[byGrade];
		word mask = TblShunZi5[__bitmap];
		if (WORD_BIT_COUNT(mask) >= byCount)
		{
			byGrade = TblBitIndex[(mask & -mask) % 37];
			return TYPE_SHUNZI;
		}
		byGrade = GRADE_INIT; //没有足够张数的顺子
	}
	byGrade = GRADE_INIT;
	byCount = 0;
	return TYPE_GVP;
}

byte CRPlayer::_RspAnyLianDui(byte &byGrade, byte &byCount)
{
	word _bitmap = bitmap[1] | bitmap[2] | bitmap[3];
	_bitmap &= MASK_SHUNZI;
	if (byCount == 0) //第一次计算连对
		byCount = 8;
	for ( ; byCount >= 3; byCount--)
	{
		word __bitmap = _bitmap;
		__bitmap &= TblContraryMask[byGrade];
		word mask = TblShunZi3[__bitmap];
		if (WORD_BIT_COUNT(mask) >= byCount)
		{
			byGrade = TblBitIndex[(mask & -mask) % 37];
			return TYPE_LIANDUI;
		}
		byGrade = GRADE_INIT; //没有足够张数的连对
	}
	byGrade = GRADE_INIT;
	byCount = 0;
	return TYPE_GVP;
}

void CRPlayer::_InitChoice1S(word &wChoice)
{
	word _bitmap = bitmap[0] | bitmap[1] | bitmap[2] | bitmap[3];
	_bitmap &= MASK_SHUNZI;

	word _single = bitmap[0] & ~TblShun5PrhS[_bitmap];
	_single &= MASK_SHUNZI; //不构成顺子的单张（3..A)
	if (_single)
	{
		wChoice = _single & -_single; //只带最小的单张
		return;
	}

	//顺子中的多余的牌
	word prhbmp = 0;
	word shunzi = TblShunZi5[_bitmap];
	if (shunzi)
	{
		word __bitmap0 = bitmap[0] & shunzi;
		word __bitmap1 = bitmap[1] & shunzi;
		word __bitmap2 = bitmap[2] & shunzi;

		//尝试拆顺子
		if (__bitmap0)
			prhbmp |= ~(__bitmap0 & -__bitmap0) & shunzi;

		//有对尝试拆对，没对尝试拆三张
		if (__bitmap1)
			prhbmp |= ~(__bitmap1 & -__bitmap1) & shunzi;
		else if (__bitmap2)
			prhbmp |= ~(__bitmap2 & -__bitmap2) & shunzi;

		//尝试拆顺子里最大的单张
		word highbit = shunzi ^ TblShun5PrhH[shunzi];
		if (highbit & bitmap[0])
			prhbmp &= ~highbit;
	}

	//禁止拆五连的单张
	prhbmp |= TblFiveShun[bitmap[0] & MASK_SHUNZI];

	//禁止拆纯的连对
	prhbmp |= TblShunZi3[bitmap[1] & MASK_SHUNZI];

	//禁止拆火箭
	if ((bitmap[0] & MASK_ROCKET) == MASK_ROCKET)
		prhbmp |= MASK_ROCKET;

	//禁止拆对2
	if (bitmap[1] & CARD_2)
		prhbmp |= CARD_2;

	//禁止拆不是顺子的三张，四张
	prhbmp |= (bitmap[2] | bitmap[3]) & ~TblShun5PrhS[_bitmap];

	wChoice = bitmap[0] | bitmap[1] | bitmap[2] | bitmap[3];
	wChoice ^= prhbmp;
}

void CRPlayer::_InitChoice1P(word &wChoice)
{
	word _bitmap = bitmap[0] | bitmap[1] | bitmap[2] | bitmap[3];
	_bitmap &= MASK_SHUNZI;

	word _pair = bitmap[1] & ~TblShun5PrhS[_bitmap];
	_pair &= MASK_SHUNZI; //不构成顺子的对子（3..A)
	if (_pair)
	{
		wChoice = _pair & -_pair; //只带最小的对子
		return;
	}

	word prhbmp = 0;

	//禁止拆不构成顺子的三张，四张
	prhbmp |= (bitmap[2] | bitmap[3]) & ~TblShun5PrhS[_bitmap];
	word shunzi = TblShunZi5[_bitmap];
	if (shunzi)
	{
		//禁止拆顺子中的炸弹
		if (bitmap[3] & shunzi)
			prhbmp |= bitmap[3] & shunzi;
	}

	//禁止拆纯的连对
	word liandui = TblShunZi3[bitmap[1] & MASK_SHUNZI];
	if (WORD_BIT_COUNT(liandui) == 3)
		prhbmp |= liandui;
	else if (WORD_BIT_COUNT(liandui) > 3)
	{
		liandui &= liandui - 1;
		prhbmp |= liandui;
	}

	wChoice = bitmap[1] | bitmap[2] | bitmap[3];
	wChoice ^= prhbmp;
}

void CRPlayer::_InitChoice2S(word &wChoice)
{
	word _bitmap = bitmap[0] | bitmap[1] | bitmap[2] | bitmap[3];
	_bitmap &= MASK_SHUNZI;

	word _single = bitmap[0] & ~TblShun5PrhS[_bitmap];
	_single &= MASK_SHUNZI; //不构成顺子的单张（3..A)

	word shunzi = TblShunZi5[_bitmap];
	if (shunzi)
	{
		//从顺子里的两张拆出单张
		if (bitmap[1] & shunzi)
			_single |= bitmap[1] & shunzi;
	}

	int nSingleCount = WORD_BIT_COUNT(_single);
	if (nSingleCount < 2)
		_single = bitmap[0] | bitmap[1] | bitmap[2]; //暴力模式

	if (WORD_BIT_COUNT(_single) < 2)
		wChoice = 0;
	else
	{
		word temp1 = _single & -_single;
		_single ^= temp1;
		word temp2 = _single & -_single;
		wChoice = temp1 | temp2;
	}

	//不允许同时带大小王
	if ((wChoice & MASK_ROCKET) == MASK_ROCKET)
		wChoice = 0;
}

void CRPlayer::_InitChoice2P(word &wChoice)
{
	word _bitmap = bitmap[0] | bitmap[1] | bitmap[2] | bitmap[3];
	_bitmap &= MASK_SHUNZI;

	word _pair = bitmap[1] & ~TblShun5PrhS[_bitmap];
	_pair &= MASK_SHUNZI; //不构成顺子的对子（3..A)

	word shunzi = TblShunZi5[_bitmap];
	if (shunzi)
	{
		//从顺子里的三张拆出对子
		if (bitmap[2] & shunzi)
			_pair |= bitmap[2] & shunzi;

		//从六连以上的边界拆出对子
		if (TblFiveShun[shunzi] == 0)
		{
			if (bitmap[1] & TblShun5PrhE[shunzi])
				_pair |= bitmap[1] & TblShun5PrhE[shunzi];
		}
	}

	int nPairCount = WORD_BIT_COUNT(_pair);
	if (nPairCount < 2)
		_pair = bitmap[1] | bitmap[2]; //暴力模式

	if (WORD_BIT_COUNT(_pair) < 2)
		wChoice = 0;
	else
	{
		word temp1 = _pair & -_pair;
		_pair ^= temp1;
		word temp2 = _pair & -_pair;
		wChoice = temp1 | temp2;
	}
}

void CRPlayer::_InitChoiceNS(byte byCount, word &wChoice)
{
	word _bitmap = bitmap[0] | bitmap[1] | bitmap[2] | bitmap[3];
	_bitmap &= MASK_SHUNZI;

	word _single = bitmap[0] & ~TblShun5PrhS[_bitmap];
	_single &= MASK_SHUNZI; //不构成顺子的单张（3..A)

	word shunzi = TblShunZi5[_bitmap];
	if (shunzi)
	{
		//从顺子里的两张拆出单张
		if (bitmap[1] & shunzi)
			_single |= bitmap[1] & shunzi;
	}

	int nSingleCount = WORD_BIT_COUNT(_single);
	if (nSingleCount < byCount)
		_single = bitmap[0] | bitmap[1] | bitmap[2]; //暴力模式

	if (WORD_BIT_COUNT(_single) < byCount)
		wChoice = 0;
	else
	{
		for (int i = 0; i < byCount; i++)
		{
			word temp = _single & -_single;
			_single ^= temp;
			wChoice |= temp;
		}
	}

	//不允许同时带大小王
	if ((wChoice & MASK_ROCKET) == MASK_ROCKET)
		wChoice = 0;
}

void CRPlayer::_InitChoiceNP(byte byCount, word &wChoice)
{
	word _bitmap = bitmap[0] | bitmap[1] | bitmap[2] | bitmap[3];
	_bitmap &= MASK_SHUNZI;

	word _pair = bitmap[1] & ~TblShun5PrhS[_bitmap];
	_pair &= MASK_SHUNZI; //不构成顺子的对子（3..A)

	word shunzi = TblShunZi5[_bitmap];
	if (shunzi)
	{
		//从顺子里的三张拆出对子
		if (bitmap[2] & shunzi)
			_pair |= bitmap[2] & shunzi;

		//从六连以上的边界拆出对子
		if (TblFiveShun[shunzi] == 0)
		{
			if (bitmap[1] & TblShun5PrhE[shunzi])
				_pair |= bitmap[1] & TblShun5PrhE[shunzi];
		}
	}

	int nPairCount = WORD_BIT_COUNT(_pair);
	if (nPairCount < byCount)
		_pair = bitmap[1] | bitmap[2]; //暴力模式

	if (WORD_BIT_COUNT(_pair) < byCount)
		wChoice = 0;
	else
	{
		for (int i = 0; i < byCount; i++)
		{
			word temp = _pair & -_pair;
			_pair ^= temp;
			wChoice |= temp;
		}
	}
}
