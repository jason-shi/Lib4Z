#include "StdAfx.h"
#include "RPlayer.h"

byte CRPlayer::RspCards(byte byType, byte byRspType, byte &byGrade, byte &byCount)
{
	switch (byType)
	{
	case TYPE_GVP:
		return _RspGvp(byRspType, byGrade, byCount);
	case TYPE_PAIR:
		return _RspPair(byRspType, byGrade, byCount);
	case TYPE_SINGLE:
		return _RspSingle(byRspType, byGrade, byCount);
	case TYPE_BOMB:
		return _RspBomb(byRspType, byGrade, byCount);
	case TYPE_CANNON:
		return _RspCannon(byRspType, byGrade, byCount);
	case TYPE_REDFLAG:
		return _RspRedFlag(byRspType, byGrade, byCount);
	case TYPE_ROCKET:
		return _RspRocket(byRspType, byGrade, byCount);
	case TYPE_SHUNZI:
		return _RspShunZi(byRspType, byGrade, byCount);
	case TYPE_LIANDUI:
		return _RspLianDui(byRspType, byGrade, byCount);
	case TYPE_CHA:
		return _RspCha(byRspType, byGrade, byCount);
	case TYPE_GOU:
		return _RspGou(byRspType, byGrade, byCount);
	}
	return TYPE_GVP;
}

byte CRPlayer::_RspGvp(byte byRspType, byte &byGrade, byte &byCount)
{
	byte byRtnType = TYPE_GVP;
	switch (byRspType)
	{
	case TYPE_GVP:
		byGrade = GRADE_INIT;
		byCount = 0;
	case TYPE_SHUNZI:
		byRtnType = _TryShunZi(byGrade, byCount); CHECK_RETURN(byRtnType);
	case TYPE_LIANDUI:
		byRtnType = _TryLianDui(byGrade, byCount); CHECK_RETURN(byRtnType);
	case TYPE_PAIR:
		byRtnType = _TryPair(byGrade, byCount); CHECK_RETURN(byRtnType);
	case TYPE_SINGLE:
		byRtnType = _TrySingle(byGrade, byCount); CHECK_RETURN(byRtnType);
	case TYPE_BOMB:
		byRtnType = _TryBomb(byGrade, byCount); CHECK_RETURN(byRtnType);
	case TYPE_CANNON:
		byRtnType = _TryCannon(byGrade, byCount); CHECK_RETURN(byRtnType);
		byRtnType = _TryRedFlag(byGrade, byCount); CHECK_RETURN(byRtnType);
	case TYPE_REDFLAG:
		byRtnType = _TryRocket(byGrade, byCount); CHECK_RETURN(byRtnType);
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
		byRtnType = _TryBomb(byGrade, byCount); CHECK_RETURN(byRtnType);
	case TYPE_CANNON:
		byRtnType = _TryCannon(byGrade, byCount); CHECK_RETURN(byRtnType);
		byRtnType = _TryRedFlag(byGrade, byCount); CHECK_RETURN(byRtnType);
	case TYPE_REDFLAG:
		byRtnType = _TryRocket(byGrade, byCount); CHECK_RETURN(byRtnType);
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
		byRtnType = _TryBomb(byGrade, byCount); CHECK_RETURN(byRtnType);
	case TYPE_CANNON:
		byRtnType = _TryCannon(byGrade, byCount); CHECK_RETURN(byRtnType);
		byRtnType = _TryRedFlag(byGrade, byCount); CHECK_RETURN(byRtnType);
	case TYPE_REDFLAG:
		byRtnType = _TryRocket(byGrade, byCount); CHECK_RETURN(byRtnType);
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
		byRtnType = _TryBomb(byGrade, byCount); CHECK_RETURN(byRtnType);
	case TYPE_CANNON:
		byRtnType = _TryCannon(byGrade, byCount); CHECK_RETURN(byRtnType);
		byRtnType = _TryRedFlag(byGrade, byCount); CHECK_RETURN(byRtnType);
	case TYPE_REDFLAG:
		byRtnType = _TryRocket(byGrade, byCount); CHECK_RETURN(byRtnType);
	}
	return TYPE_GVP;
}

byte CRPlayer::_RspCannon(byte byRspType, byte &byGrade, byte &byCount)
{
	byte byRtnType = TYPE_GVP;
	switch (byRspType)
	{
	case TYPE_GVP:
	case TYPE_CANNON:
		byRtnType = _TryCannon(byGrade, byCount); CHECK_RETURN(byRtnType);
		byRtnType = _TryRedFlag(byGrade, byCount); CHECK_RETURN(byRtnType);
	case TYPE_REDFLAG:
		byRtnType = _TryRocket(byGrade, byCount); CHECK_RETURN(byRtnType);
	}
	return TYPE_GVP;
}

byte CRPlayer::_RspRedFlag(byte byRspType, byte &byGrade, byte &byCount)
{
	switch (byRspType)
	{
	case TYPE_REDFLAG:
		word _bitmap = bitmap[0] | bitmap[1] | bitmap[2] | bitmap[3];
		if (_bitmap & CARD_A)
		{
			word _bitmap2 = bitmap[1] | bitmap[2] | bitmap[3];
			if (_bitmap2 & CARD_4)
				return TYPE_ROCKET;
		}
		break;
	}
	return TYPE_GVP;
}

byte CRPlayer::_RspRocket(byte byRspType, byte &byGrade, byte &byCount)
{
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
		while (mask = TblShunZi[_bitmap])
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
		byRtnType = _TryBomb(byGrade, byCount); CHECK_RETURN(byRtnType);
	case TYPE_CANNON:
		byRtnType = _TryCannon(byGrade, byCount); CHECK_RETURN(byRtnType);
		byRtnType = _TryRedFlag(byGrade, byCount); CHECK_RETURN(byRtnType);
	case TYPE_REDFLAG:
		byRtnType = _TryRocket(byGrade, byCount); CHECK_RETURN(byRtnType);
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
		while (mask = TblShunZi[_bitmap])
		{
			if (WORD_BIT_COUNT(mask) >= byCount)
			{
				byGrade = TblBitIndex[(mask & -mask) % 37];
				return TYPE_LIANDUI;
			}
			_bitmap ^= mask; //连对长度不够
		}
		byRtnType = _TryRocket(byGrade, byCount); CHECK_RETURN(byRtnType);
	}
	return TYPE_GVP;
}

byte CRPlayer::_RspCha(byte byRspType, byte &byGrade, byte &byCount)
{
	int _bitmap = 0;
	switch (byRspType)
	{
	case TYPE_GVP:
		_bitmap = bitmap[1] | bitmap[2] | bitmap[3];
		if (_bitmap & (1 << byGrade))
			return TYPE_CHA;
	}
	return TYPE_GVP;
}

byte CRPlayer::_RspGou(byte byRspType, byte &byGrade, byte &byCount)
{
	switch (byRspType)
	{
	case TYPE_GVP:
		if (bitmap[0] & (1 << byGrade))
			return TYPE_GOU;
	}
	return TYPE_GVP;
}

byte CRPlayer::_RspSingleBySingle(byte &byGrade, byte &byCount)
{
	//单顺（到Q）不可拆，双龙不可拆，隔离对不可拆
	word prhbmp = TblShunPrhH[bitmap[0] & MASK_SHUNZI]
				| TblShunPrhH[bitmap[1] & MASK_SHUNZI]
				| bitmap[1] & ~TblShunPrhS[(bitmap[0] | bitmap[1]) & MASK_SHUNZI] & MASK_SHUNZI_Q;

	word _bitmap = bitmap[0] | bitmap[1]/* | bitmap[2] | bitmap[3]*/; //应答单张不拆炸弹
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
	word _bitmap = bitmap[1] | bitmap[2]/* | bitmap[3]*/; //应答对子不拆炮

	word _bitmap4 = bitmap[0] | bitmap[1] | bitmap[2] | bitmap[3];
	_bitmap4 &= MASK_SHUNZI;
	_bitmap ^= ~TblShunPrhS[_bitmap4] & bitmap[2]; //不拆不连的炸弹
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

byte CRPlayer::_TrySingle(byte &byGrade, byte &byCount)
{
	//单顺（到Q）不可拆，双龙不可拆，隔离对不可拆
	word prhbmp = TblShunPrhL[bitmap[0] & MASK_SHUNZI]
				| TblShunPrhL[bitmap[1] & MASK_SHUNZI]
				| bitmap[1] & ~TblShunPrhS[(bitmap[0] | bitmap[1]) & MASK_SHUNZI] & MASK_SHUNZI_Q;

	word _bitmap = bitmap[0] | bitmap[1]/* | bitmap[3] | bitmap[4]*/; //主动出牌不拆炸弹
	_bitmap ^= prhbmp;
	if ((bitmap[0] & MASK_REDFLAG) == MASK_REDFLAG) //主动出牌不拆红旗
		_bitmap &= ~MASK_REDFLAG;
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

byte CRPlayer::_TryPair(byte &byGrade, byte &byCount)
{
	//双龙不可拆
	word prhbmp = TblShunPrhL[bitmap[1] & MASK_SHUNZI];

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

byte CRPlayer::_TryBomb(byte &byGrade, byte &byCount)
{
	word _bitmap = bitmap[2]/* | bitmap[3]*/;
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

byte CRPlayer::_TryCannon(byte &byGrade, byte &byCount)
{
	word _bitmap = bitmap[3];
	_bitmap &= TblContraryMask[byGrade];
	if (_bitmap)
	{
		byGrade = TblBitIndex[(_bitmap & -_bitmap) % 37];
		return TYPE_CANNON;
	}
	byGrade = GRADE_INIT;
	byCount = 0;
	return TYPE_GVP;
}

byte CRPlayer::_TryRedFlag(byte &byGrade, byte &byCount)
{
	if ((bitmap[0] & MASK_REDFLAG) == MASK_REDFLAG)
		return TYPE_REDFLAG;

	byGrade = GRADE_INIT;
	byCount = 0;
	return TYPE_GVP;
}

byte CRPlayer::_TryRocket(byte &byGrade, byte &byCount)
{
	word _bitmap = bitmap[0] | bitmap[1] | bitmap[2] | bitmap[3];
	if (_bitmap & CARD_A)
	{
		int _bitmap2 = _bitmap ^ bitmap[0];
		if (_bitmap2 & CARD_4)
			return TYPE_ROCKET;
	}
	byGrade = GRADE_INIT;
	byCount = 0;
	return TYPE_GVP;
}

byte CRPlayer::_TryShunZi(byte &byGrade, byte &byCount)
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
			return TYPE_SHUNZI;
		}
		byGrade = GRADE_INIT; //没有足够张数的顺子
	}
	byGrade = GRADE_INIT;
	byCount = 0;
	return TYPE_GVP;
}

byte CRPlayer::_TryLianDui(byte &byGrade, byte &byCount)
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
			return TYPE_LIANDUI;
		}
		byGrade = GRADE_INIT; //没有足够张数的连对
	}
	byGrade = GRADE_INIT;
	byCount = 0;
	return TYPE_GVP;
}
