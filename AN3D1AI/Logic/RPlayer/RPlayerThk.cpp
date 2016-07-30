#include "StdAfx.h"
#include "RPlayer.h"

#define SHIFT_1(n)				((1 << (n)) - 1)

byte CRPlayer::MakeCards(byte byType, byte &byGrade)
{
	return byType == TYPE_GVP ? MakeGvp(byGrade)
							  : MakePut(byType, byGrade);
}

byte CRPlayer::MakeGvp(byte &byGrade)
{
	for (byte byType = TYPE_SPADE; byType != TYPE_ERROR; byType++)
	{
		word _bitmap = bitmap[byType - 1];
		if (_bitmap == 0)
			continue;

		byGrade = _bitmap & 0xFFF ? TblMinorGrade[_bitmap & 0xFFF] : TblMajorGrade[_bitmap >> 12];
		return byType;
	}
	return TYPE_GVP;
}

byte CRPlayer::MakePut(byte byType, byte &byGrade)
{
	return bitmap[byType - 1] != 0 ? MakePutSame(byType, byGrade)
								   : MakePutDiff(byType, byGrade);
}

byte CRPlayer::MakePutSame(byte byType, byte &byGrade)
{
	word _bitmap = bitmap[byType - 1];

	//庄家大，不要加分
	//庄家小，要加分

	byGrade = _bitmap & 0xFFF ? TblMinorGrade[_bitmap & 0xFFF] : TblMajorGrade[_bitmap >> 12];
	return byType;
}

byte CRPlayer::MakePutDiff(byte byType, byte &byGrade)
{
	for (byte byRspType = TYPE_SPADE; byRspType != TYPE_ERROR; byRspType++)
	{
		word _bitmap = bitmap[byRspType - 1];
		if (_bitmap == 0)
			continue;

		byGrade = _bitmap & 0xFFF ? TblMinorGrade[_bitmap & 0xFFF] : TblMajorGrade[_bitmap >> 12];
		return byRspType;
	}
	return TYPE_GVP;
}

byte CRPlayer::MakeWithScore(byte byType, byte &byGrade)
{
	word _bitmap = bitmap[byType - 1];
	if (_bitmap != 0)
	{
		byGrade = TblPutScore[_bitmap & MASK_5TK];
		if (byGrade != 0)
			return byType;
	}
	else
	{
		for (byte byRspType = TYPE_SPADE; byRspType != TYPE_ERROR; byRspType++)
		{
			_bitmap = bitmap[byRspType - 1];
			if (_bitmap == 0)
				continue;

			byGrade = TblPutScore[_bitmap & MASK_5TK];
			if (byGrade != 0)
				return byRspType;
		}
	}
	return TYPE_GVP;
}

byte CRPlayer::MakeWithoutScore(byte byType, byte &byGrade)
{
	word _bitmap = bitmap[byType - 1];
	if (_bitmap != 0)
	{
		_bitmap &= ~MASK_5TK;
		if (_bitmap == 0)
			return TYPE_GVP;

		byGrade = _bitmap & 0xFFF ? TblMinorGrade[_bitmap & 0xFFF] : TblMajorGrade[_bitmap >> 12];
		if (byGrade != 0)
			return byType;
	}
	else
	{
		for (byte byRspType = TYPE_SPADE; byRspType != TYPE_ERROR; byRspType++)
		{
			_bitmap = bitmap[byRspType - 1] & ~MASK_5TK;
			if (_bitmap == 0)
				continue;

			byGrade = _bitmap & 0xFFF ? TblMinorGrade[_bitmap & 0xFFF] : TblMajorGrade[_bitmap >> 12];
			if (byGrade != 0)
				return byRspType;
		}
	}
	return TYPE_GVP;
}

byte CRPlayer::MakeByLittle(byte byType, byte &byGrade)
{
	word _bitmap = bitmap[byType - 1];
	if (_bitmap != 0)
	{
		byGrade = _bitmap & 0xFFF ? TblMinorGrade[_bitmap & 0xFFF] : TblMajorGrade[_bitmap >> 12];
		return byType;
	}
	else
	{
		for (byte byRspType = TYPE_HEART; byRspType != TYPE_ERROR; byRspType++)
		{
			_bitmap = bitmap[byRspType - 1];
			if (_bitmap == 0)
				continue;

			byGrade = _bitmap & 0xFFF ? TblMinorGrade[_bitmap & 0xFFF] : TblMajorGrade[_bitmap >> 12];
			return byRspType;
		}

		//最后选择主牌
		_bitmap = bitmap[TYPE_SPADE - 1];
		if (_bitmap == 0)
			return TYPE_GVP;

		byGrade = _bitmap & 0xFFF ? TblMinorGrade[_bitmap & 0xFFF] : TblMajorGrade[_bitmap >> 12];
		return TYPE_SPADE;
	}
	return TYPE_GVP;
}

byte CRPlayer::MakeJustOver(byte byType, byte &byGrade, byte byTypeM, byte byGradeM)
{
	word _bitmap = bitmap[byType - 1];
	if (_bitmap != 0)
	{
		if (byTypeM == byType) //同花色
		{
			_bitmap &= TblContraryMask[byGradeM];
			if (_bitmap == 0)
				return TYPE_GVP;

			byGrade = _bitmap & 0xFFF ? TblMinorGrade[_bitmap & 0xFFF] : TblMajorGrade[_bitmap >> 12];
			return byType;
		}
	}
	else
	{
		if (byTypeM == TYPE_SPADE) //盖杀
		{
			if (byType != TYPE_SPADE)
			{
				_bitmap = bitmap[0];
				if (_bitmap != 0)
				{
					byGrade = _bitmap & 0xFFF ? TblMinorGrade[_bitmap & 0xFFF] : TblMajorGrade[_bitmap >> 12];
					return TYPE_SPADE;
				}
			}
		}
	}
	return TYPE_GVP;
}
