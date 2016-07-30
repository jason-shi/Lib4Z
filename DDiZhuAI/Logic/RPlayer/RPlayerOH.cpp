#include "StdAfx.h"
#include "RPlayer.h"

byte CRPlayer::OneHand_GetType()
{
	word _bitmap = bitmap[0] | bitmap[1] | bitmap[2] | bitmap[3];

	int nBitCount = WORD_BIT_COUNT(_bitmap);
	//检测顺子
	if (nBitCount >= 5)
	{
		if (TblShunZi5[_bitmap & MASK_SHUNZI] == _bitmap && (bitmap[1] | bitmap[2] | bitmap[3]) == 0)
			return TYPE_SHUNZI;
	}
	//检测连对
	if (nBitCount >= 3)
	{
		if (TblShunZi3[_bitmap & MASK_SHUNZI] == _bitmap && (bitmap[0] | bitmap[2] | bitmap[3]) == 0)
			return TYPE_LIANDUI;
	}
	//检测飞机
	if (nBitCount >= 2)
	{
		if (TblShunZi2[_bitmap & MASK_SHUNZI] == _bitmap && (bitmap[0] | bitmap[1] | bitmap[3]) == 0)
			return TYPE_PLANE;
	}
	//至此，不会是顺子，不会是连对，不会是飞机
	if (nBitCount == 1)
	{
		if (bitmap[0] != 0) return TYPE_SINGLE;
		if (bitmap[1] != 0) return TYPE_PAIR;
		if (bitmap[2] != 0) return TYPE_TRIP;
		if (bitmap[3] != 0) return TYPE_BOMB;
	}
	else if (nBitCount == 2)
	{
		if (_bitmap == MASK_ROCKET)
			return TYPE_ROCKET;
		if (bitmap[2] != 0)
		{
			if (bitmap[0] != 0) return TYPE_TRIP_S;
			if (bitmap[1] != 0) return TYPE_TRIP_P;
		}
	}
	else if (nBitCount == 3)
	{
		if (WORD_BIT_COUNT(bitmap[3]) == 1)
		{
			if (WORD_BIT_COUNT(bitmap[0]) == 2 && (bitmap[0] & MASK_ROCKET) != MASK_ROCKET) return TYPE_QUAD_S;
			if (WORD_BIT_COUNT(bitmap[1]) == 2) return TYPE_QUAD_P;
		}
	}
	else if (nBitCount >= 4 && (nBitCount & 1) == 0)
	{
		int nHalf = nBitCount / 2;
		if (TblShunZi2[bitmap[2]] == bitmap[2] && WORD_BIT_COUNT(bitmap[2]) == nHalf)
		{
			if (WORD_BIT_COUNT(bitmap[0]) == nHalf && (bitmap[0] & MASK_ROCKET) != MASK_ROCKET) return TYPE_PLANE_S;
			if (WORD_BIT_COUNT(bitmap[1]) == nHalf) return TYPE_PLANE_P;
		}
	}
	return TYPE_ERROR;
}

byte CRPlayer::OneHand_GetGrade(byte byRspType)
{
	word _bitmap = 0;
	switch (byRspType)
	{
	case TYPE_SINGLE:  _bitmap = bitmap[0]; break;
	case TYPE_PAIR:    _bitmap = bitmap[1]; break;
	case TYPE_TRIP:    _bitmap = bitmap[2]; break;
	case TYPE_TRIP_S:  _bitmap = bitmap[2]; break;
	case TYPE_TRIP_P:  _bitmap = bitmap[2]; break;
	case TYPE_PLANE:   _bitmap = bitmap[2]; break;
	case TYPE_PLANE_S: _bitmap = bitmap[2]; break;
	case TYPE_PLANE_P: _bitmap = bitmap[2]; break;
	case TYPE_QUAD_S:  _bitmap = bitmap[3]; break;
	case TYPE_QUAD_P:  _bitmap = bitmap[3]; break;
	case TYPE_SHUNZI:  _bitmap = bitmap[0]; break;
	case TYPE_LIANDUI: _bitmap = bitmap[1]; break;
	case TYPE_BOMB:    _bitmap = bitmap[3]; break;
	case TYPE_ROCKET:  _bitmap = bitmap[0]; break;
	}
	return TblBitIndex[(_bitmap & -_bitmap) % 37];
}

byte CRPlayer::OneHand_GetCount(byte byRspType)
{
	switch (byRspType)
	{
	case TYPE_SHUNZI:
		return WORD_BIT_COUNT(bitmap[0]);
	case TYPE_LIANDUI:
		return WORD_BIT_COUNT(bitmap[1]);
	case TYPE_PLANE:
	case TYPE_PLANE_S:
	case TYPE_PLANE_P:
		return WORD_BIT_COUNT(bitmap[2]);
	default:
		return 0;
	}
}

word CRPlayer::OneHand_GetAttach(byte byRspType)
{
	switch (byRspType)
	{
	case TYPE_TRIP_S:
	case TYPE_QUAD_S:
	case TYPE_PLANE_S:
		return bitmap[0];
	case TYPE_TRIP_P:
	case TYPE_PLANE_P:
	case TYPE_QUAD_P:
		return bitmap[1];
	default:
		return 0;
	}
}

byte CRPlayer::OneHand(byte byType, byte &byRspType, byte &byGrade, byte &byCount, word &wAttach)
{
	if (byType == TYPE_GVP)
	{
		byte _byRspType = OneHand_GetType();
		if (_byRspType == TYPE_ERROR)
			return CONTRACT_FAIL;

		byRspType = _byRspType;
		byGrade = OneHand_GetGrade(byRspType);
		byCount = OneHand_GetCount(byRspType);
		wAttach = OneHand_GetAttach(byRspType);
		return CONTRACT_YES;
	}
	if (byType != TYPE_ROCKET)
	{
		if (bitmap[1] == 0 && bitmap[2] == 0 && bitmap[3] == 0 && bitmap[0] == MASK_ROCKET)
		{
			byRspType = TYPE_ROCKET;
			return CONTRACT_YES;
		}
	}
	if (byType != TYPE_BOMB)
	{
		if (bitmap[0] == 0 && bitmap[1] == 0 && bitmap[2] == 0 && WORD_BIT_COUNT(bitmap[3]) == 1)
		{
			byRspType = TYPE_BOMB;
			byGrade = TblBitIndex[bitmap[3] % 37];
			return CONTRACT_YES;
		}
	}
	switch (byType)
	{
	case TYPE_SINGLE:
		if (bitmap[1] == 0 && bitmap[2] == 0 && bitmap[3] == 0 && WORD_BIT_COUNT(bitmap[0]) == 1)
		{
			if (bitmap[0] > (1 << byGrade))
			{
				byRspType = TYPE_SINGLE;
				byGrade = TblBitIndex[bitmap[0] % 37];
				return CONTRACT_YES;
			}
		}
		break;
	case TYPE_PAIR:
		if (bitmap[0] == 0 && bitmap[2] == 0 && bitmap[3] == 0 && WORD_BIT_COUNT(bitmap[1]) == 1)
		{
			if (bitmap[1] > (1 << byGrade))
			{
				byRspType = TYPE_PAIR;
				byGrade = TblBitIndex[bitmap[1] % 37];
				return CONTRACT_YES;
			}
		}
		break;
	}
	return CONTRACT_FAIL;
}
