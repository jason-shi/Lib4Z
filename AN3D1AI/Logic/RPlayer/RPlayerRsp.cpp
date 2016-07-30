#include "StdAfx.h"
#include "RPlayer.h"

#define SHIFT_1(n)				((1 << (n)) - 1)

byte CRPlayer::RspGvp(byte byRspType, byte &byGrade)
{
	if (byRspType == TYPE_GVP)
		byRspType = TYPE_SPADE;

	while (byRspType != TYPE_ERROR)
	{
		word _bitmap = bitmap[byRspType - 1];
		_bitmap &= TblContraryMask[byGrade];
		if (_bitmap == 0)
		{
			byRspType++;
			continue;
		}
		byGrade = _bitmap & 0xFFF ? TblMinorGrade[_bitmap & 0xFFF] : TblMajorGrade[_bitmap >> 12];
		return byRspType;
	}
	return TYPE_GVP;
}

byte CRPlayer::RspPut(byte byType, byte byRspType, byte &byGrade)
{
	word _bitmap = bitmap[byType - 1];
	if (_bitmap != 0)
	{
		return RspPutSame(byType, byRspType, byGrade);
	}
	else
	{
		if (byRspType == TYPE_GVP)
			byRspType = TYPE_SPADE;
		return RspPutDiff(byType, byRspType, byGrade);
	}
}

byte CRPlayer::RspPutSame(byte byType, byte byRspType, byte &byGrade)
{
	word _bitmap = bitmap[byType - 1];
	_bitmap &= TblContraryMask[byGrade];
	if (_bitmap == 0)
		return TYPE_GVP;

	byGrade = _bitmap & 0xFFF ? TblMinorGrade[_bitmap & 0xFFF] : TblMajorGrade[_bitmap >> 12];
	return byType;
}

byte CRPlayer::RspPutDiff(byte byType, byte byRspType, byte &byGrade)
{
	word _bitmap = bitmap[byType - 1];
	while (byRspType != TYPE_ERROR)
	{
		_bitmap = bitmap[byRspType - 1];
		_bitmap &= TblContraryMask[byGrade];
		if (_bitmap == 0)
		{
			byRspType++;
			continue;
		}
		byGrade = _bitmap & 0xFFF ? TblMinorGrade[_bitmap & 0xFFF] : TblMajorGrade[_bitmap >> 12];
		return byRspType;
	}
	return TYPE_GVP;
}
