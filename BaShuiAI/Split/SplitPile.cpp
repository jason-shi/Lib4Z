#include "StdAfx.h"
#include "Split.h"

byte CSplit::_SplitSister(byte &byGrade)
{
	word _bitmap = bitmap[0] | bitmap[1] | bitmap[2] | bitmap[3];
	_bitmap &= MASK_SHUNZI;

	word __bitmap;
	while (__bitmap = TblSister[_bitmap & TblContraryMask[byGrade]])
	{
		byGrade = TblBitIndex[(__bitmap & -__bitmap) % 37];

		OutCards(INNER_TYPE_SISTER, byGrade); //打出顺子
		return INNER_TYPE_SISTER;
	}
	byGrade = GRADE_INIT;
	return INNER_TYPE_GVP;
}

byte CSplit::_SplitA2(byte &byGrade)
{
	word _bitmap = bitmap[0] | bitmap[1] | bitmap[2] | bitmap[3];
	if ((_bitmap & MASK_A2) == MASK_A2)
	{
		OutCards(INNER_TYPE_A2, 12);
		return INNER_TYPE_A2;
	}
	byGrade = GRADE_INIT;
	return INNER_TYPE_GVP;
}

byte CSplit::_SplitKA(byte &byGrade)
{
	word _bitmap = bitmap[0] | bitmap[1] | bitmap[2] | bitmap[3];
	if ((_bitmap & MASK_KA) == MASK_KA)
	{
		OutCards(INNER_TYPE_KA, 11);
		return INNER_TYPE_KA;
	}
	byGrade = GRADE_INIT;
	return INNER_TYPE_GVP;
}

byte CSplit::_SplitPair(byte &byGrade)
{
	word _bitmap = bitmap[1] | bitmap[2] | bitmap[3];
	_bitmap &= TblContraryMask[byGrade];
	if (_bitmap)
	{
		byGrade = TblBitIndex[(_bitmap & -_bitmap) % 37];

		OutCards(INNER_TYPE_PAIR, byGrade); //打出对子
		return INNER_TYPE_PAIR;
	}
	byGrade = GRADE_INIT;
	return INNER_TYPE_GVP;
}

byte CSplit::_SplitShunZi(byte &byGrade)
{
	word _bitmap = bitmap[0] | bitmap[1] | bitmap[2] | bitmap[3];
	_bitmap &= MASK_SHUNZI;

	word __bitmap;
	while (__bitmap = TblShunZi[_bitmap & TblContraryMask[byGrade]])
	{
		byGrade = TblBitIndex[(__bitmap & -__bitmap) % 37];
		word mask = SHIFT_1(byGrade + 3) ^ SHIFT_1(byGrade);

		//确保不是同花顺
		if ((m_wFlushBitmap & mask) != mask
			|| (bitmap[0] & mask) != mask)
		{
			OutCards(INNER_TYPE_SHUNZI, byGrade); //打出顺子
			return INNER_TYPE_SHUNZI;
		}
	}
	byGrade = GRADE_INIT;
	return INNER_TYPE_GVP;
}

byte CSplit::_SplitA23(byte &byGrade)
{
	word _bitmap = bitmap[0] | bitmap[1] | bitmap[2] | bitmap[3];
	if ((_bitmap & MASK_A23) == MASK_A23)
	{
		//确保不是同花顺
		if ((m_wFlushBitmap & MASK_A23) != MASK_A23
			|| (bitmap[0] & MASK_A23) != MASK_A23)
		{
			OutCards(INNER_TYPE_A23, 12); //打出A23
			return INNER_TYPE_A23;
		}
	}
	byGrade = GRADE_INIT;
	return INNER_TYPE_GVP;
}

byte CSplit::_SplitQKA(byte &byGrade)
{
	word _bitmap = bitmap[0] | bitmap[1] | bitmap[2] | bitmap[3];
	if ((_bitmap & MASK_QKA) == MASK_QKA)
	{
		//确保不是同花顺
		if ((m_wFlushBitmap & MASK_QKA) != MASK_QKA
			|| (bitmap[0] & MASK_QKA) != MASK_QKA)
		{
			OutCards(INNER_TYPE_QKA, 10); //打出QKA
			return INNER_TYPE_QKA;
		}
	}
	byGrade = GRADE_INIT;
	return INNER_TYPE_GVP;
}

byte CSplit::_SplitBomb(byte &byGrade)
{
	word _bitmap = bitmap[2];
	_bitmap &= TblContraryMask[byGrade];
	if (_bitmap)
	{
		byGrade = TblBitIndex[(_bitmap & -_bitmap) % 37];

		OutCards(INNER_TYPE_BOMB, byGrade); //打出炸弹
		return INNER_TYPE_BOMB;
	}
	byGrade = GRADE_INIT;
	return INNER_TYPE_GVP;
}

byte CSplit::_SplitSameShunZi(byte &byGrade)
{
	word _bitmap = bitmap[0] | bitmap[1] | bitmap[2] | bitmap[3];
	_bitmap &= MASK_SHUNZI;
	_bitmap &= TblContraryMask[byGrade];
	_bitmap &= m_wFlushBitmap;
	_bitmap = TblShunZi[_bitmap];
	if (_bitmap)
	{
		byGrade = TblBitIndex[(_bitmap & -_bitmap) % 37];

		OutCards(INNER_TYPE_S_SHUNZI, byGrade); //打出同花顺
		return INNER_TYPE_S_SHUNZI;
	}
	byGrade = GRADE_INIT;
	return INNER_TYPE_GVP;
}

byte CSplit::_SplitSameA23(byte &byGrade)
{
	word _bitmap = bitmap[0] | bitmap[1] | bitmap[2] | bitmap[3];
	_bitmap &= m_wFlushBitmap;
	if ((_bitmap & MASK_A23) == MASK_A23)
	{
		OutCards(INNER_TYPE_S_A23, 12); //打出同花顺A23
		return INNER_TYPE_S_A23;
	}
	byGrade = GRADE_INIT;
	return INNER_TYPE_GVP;
}

byte CSplit::_SplitSameQKA(byte &byGrade)
{
	word _bitmap = bitmap[0] | bitmap[1] | bitmap[2] | bitmap[3];
	_bitmap &= m_wFlushBitmap;
	if ((_bitmap & MASK_QKA) == MASK_QKA)
	{
		OutCards(INNER_TYPE_S_QKA, 10); //打出同花顺QKA
		return INNER_TYPE_S_QKA;
	}
	byGrade = GRADE_INIT;
	return INNER_TYPE_GVP;
}

byte CSplit::SplitlPile(byte byType, byte &byGrade)
{
	byte byRtnType = INNER_TYPE_GVP;
	switch (byType)
	{
	case INNER_TYPE_GVP:
	case INNER_TYPE_PAIR:
		byRtnType = _SplitPair(byGrade); CHECK_RETURN(byRtnType);
	case INNER_TYPE_SHUNZI:
		byRtnType = _SplitShunZi(byGrade); CHECK_RETURN(byRtnType);
		byRtnType = _SplitA23(byGrade); CHECK_RETURN(byRtnType);
	case INNER_TYPE_A23:
		byRtnType = _SplitQKA(byGrade); CHECK_RETURN(byRtnType);
	case INNER_TYPE_QKA:
	case INNER_TYPE_BOMB:
		byRtnType = _SplitBomb(byGrade); CHECK_RETURN(byRtnType);
	case INNER_TYPE_S_SHUNZI:
		byRtnType = _SplitSameShunZi(byGrade); CHECK_RETURN(byRtnType);
		byRtnType = _SplitSameA23(byGrade); CHECK_RETURN(byRtnType);
	case INNER_TYPE_S_A23:
		byRtnType = _SplitSameQKA(byGrade); CHECK_RETURN(byRtnType);
	}
	return INNER_TYPE_GVP;
}

byte CSplit::SplitlPile_Sister(byte byType, byte &byGrade)
{
	byte byRtnType = INNER_TYPE_GVP;
	switch (byType)
	{
	case INNER_TYPE_GVP:
	case INNER_TYPE_SISTER:
		byRtnType = _SplitSister(byGrade); CHECK_RETURN(byRtnType);
		byRtnType = _SplitA2(byGrade); CHECK_RETURN(byRtnType);
	case INNER_TYPE_A2:
		byRtnType = _SplitKA(byGrade); CHECK_RETURN(byRtnType);
	}
	return INNER_TYPE_GVP;
}

byte CSplit::SplitlPile_ShunZi(byte byType, byte &byGrade)
{
	byte byRtnType = INNER_TYPE_GVP;
	switch (byType)
	{
	case INNER_TYPE_GVP:
	case INNER_TYPE_SHUNZI:
		byRtnType = _SplitShunZi(byGrade); CHECK_RETURN(byRtnType);
		byRtnType = _SplitA23(byGrade); CHECK_RETURN(byRtnType);
	case INNER_TYPE_A23:
		byRtnType = _SplitQKA(byGrade); CHECK_RETURN(byRtnType);
	case INNER_TYPE_QKA:
	case INNER_TYPE_S_SHUNZI:
		byRtnType = _SplitSameShunZi(byGrade); CHECK_RETURN(byRtnType);
		byRtnType = _SplitSameA23(byGrade); CHECK_RETURN(byRtnType);
	case INNER_TYPE_S_A23:
		byRtnType = _SplitSameQKA(byGrade); CHECK_RETURN(byRtnType);
	}
	return INNER_TYPE_GVP;
}
