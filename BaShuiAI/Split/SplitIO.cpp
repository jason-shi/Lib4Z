#include "StdAfx.h"
#include "Split.h"

void CSplit::OutCards(byte byType, byte byGrade)
{
	switch (byType)
	{
	case INNER_TYPE_SISTER:
		_OutSister(SHIFT_1(byGrade + 2) ^ SHIFT_1(byGrade));
		break;
	case INNER_TYPE_A2:
		_OutSister(MASK_A2);
		break;
	case INNER_TYPE_KA:
		_OutSister(MASK_KA);
		break;
	case INNER_TYPE_PAIR:
		_OutPair(1 << byGrade);
		break;
	case INNER_TYPE_SHUNZI:
		_OutShunZi(SHIFT_1(byGrade + 3) ^ SHIFT_1(byGrade));
		break;
	case INNER_TYPE_A23:
		_OutShunZi(MASK_A23);
		break;
	case INNER_TYPE_QKA:
		_OutShunZi(MASK_QKA);
		break;
	case INNER_TYPE_BOMB:
		_OutBomb(1 << byGrade);
		break;
	case INNER_TYPE_S_SHUNZI:
		_OutSameShunZi(SHIFT_1(byGrade + 3) ^ SHIFT_1(byGrade));
		break;
	case INNER_TYPE_S_A23:
		_OutSameShunZi(MASK_A23);
		break;
	case INNER_TYPE_S_QKA:
		_OutSameShunZi(MASK_QKA);
		break;
	}
}

void CSplit::InCards(byte byType, byte byGrade)
{
	switch (byType)
	{
	case INNER_TYPE_SISTER:
		_InSister(SHIFT_1(byGrade + 2) ^ SHIFT_1(byGrade));
		break;
	case INNER_TYPE_A2:
		_InSister(MASK_A2);
		break;
	case INNER_TYPE_KA:
		_InSister(MASK_KA);
		break;
	case INNER_TYPE_PAIR:
		_InPair(1 << byGrade);
		break;
	case INNER_TYPE_SHUNZI:
		_InShunZi(SHIFT_1(byGrade + 3) ^ SHIFT_1(byGrade));
		break;
	case INNER_TYPE_A23:
		_InShunZi(MASK_A23);
		break;
	case INNER_TYPE_QKA:
		_InShunZi(MASK_QKA);
		break;
	case INNER_TYPE_BOMB:
		_InBomb(1 << byGrade);
		break;
	case INNER_TYPE_S_SHUNZI:
		_InSameShunZi(SHIFT_1(byGrade + 3) ^ SHIFT_1(byGrade));
		break;
	case INNER_TYPE_S_A23:
		_InSameShunZi(MASK_A23);
		break;
	case INNER_TYPE_S_QKA:
		_InSameShunZi(MASK_QKA);
		break;
	}
}

void CSplit::_OutSingle(word mask)
{
	if (bitmap[0] & mask) //1�ű�0��
		bitmap[0] ^= mask;
	else if (bitmap[1] & mask) //2�ű�1��
	{
		bitmap[1] ^= mask;
		bitmap[0] |= mask;
	}
	else if (bitmap[2] & mask) //3�ű�2��
	{
		bitmap[2] ^= mask;
		bitmap[1] |= mask;
	}
	else //4�ű�3��
	{
		bitmap[3] ^= mask;
		bitmap[2] |= mask;
	}
}

void CSplit::_InSingle(word mask)
{
	if (bitmap[0] & mask) //1�ű�2��
	{
		bitmap[0] ^= mask;
		bitmap[1] |= mask;
	}
	else if (bitmap[1] & mask) //2�ű�3��
	{
		bitmap[1] ^= mask;
		bitmap[2] |= mask;
	}
	else if (bitmap[2] & mask) //3�ű�4��
	{
		bitmap[2] ^= mask;
		bitmap[3] |= mask;
	}
	else //0�ű�1��
	{
		bitmap[0] |= mask;
	}
}

void CSplit::_OutSister(word mask)
{
	int C3 = bitmap[3] & mask; bitmap[3] ^= C3;
	int C2 = bitmap[2] & mask; bitmap[2] ^= C2;
	int C1 = bitmap[1] & mask; bitmap[1] ^= C1;
	int C0 = bitmap[0] & mask; bitmap[0] ^= C0;

	bitmap[2] |= C3;
	bitmap[1] |= C2;
	bitmap[0] |= C1;
}

void CSplit::_InSister(word mask)
{
	int C2 = bitmap[2] & mask; bitmap[2] ^= C2;
	int C1 = bitmap[1] & mask; bitmap[1] ^= C1;
	int C0 = bitmap[0] & mask; bitmap[0] ^= C0;

	bitmap[3] |= C2;
	bitmap[2] |= C1;
	bitmap[1] |= C0;
	bitmap[0] |= mask ^ (C2 | C1 | C0);
}

void CSplit::_OutPair(word mask)
{
	if (bitmap[1] & mask) //2�ű�0��
		bitmap[1] ^= mask;
	else if (bitmap[2] & mask) //3�ű�1��
	{
		bitmap[2] ^= mask;
		bitmap[0] |= mask;
	}
	else //4�ű�2��
	{
		bitmap[3] ^= mask;
		bitmap[1] |= mask;
	}
}

void CSplit::_InPair(word mask)
{
	if (bitmap[0] & mask) //1�ű�3��
	{
		bitmap[0] ^= mask;
		bitmap[2] |= mask;
	}
	else if (bitmap[1] & mask) //2�ű�4��
	{
		bitmap[1] ^= mask;
		bitmap[3] |= mask;
	}
	else //0�ű�2��
	{
		bitmap[1] |= mask;
	}
}

void CSplit::_OutBomb(word mask)
{
	if (bitmap[2] & mask) //3�ű�0��
		bitmap[2] ^= mask;
	else //4�ű�1��
	{
		bitmap[3] ^= mask;
		bitmap[0] |= mask;
	}
}

void CSplit::_InBomb(word mask)
{
	if (bitmap[0] & mask) //1�ű�4��
	{
		bitmap[0] ^= mask;
		bitmap[3] |= mask;
	}
	else //0�ű�3��
	{
		bitmap[2] |= mask;
	}
}

void CSplit::_OutShunZi(word mask)
{
	int C3 = bitmap[3] & mask; bitmap[3] ^= C3;
	int C2 = bitmap[2] & mask; bitmap[2] ^= C2;
	int C1 = bitmap[1] & mask; bitmap[1] ^= C1;
	int C0 = bitmap[0] & mask; bitmap[0] ^= C0;

	bitmap[2] |= C3;
	bitmap[1] |= C2;
	bitmap[0] |= C1;
}

void CSplit::_InShunZi(word mask)
{
	int C2 = bitmap[2] & mask; bitmap[2] ^= C2;
	int C1 = bitmap[1] & mask; bitmap[1] ^= C1;
	int C0 = bitmap[0] & mask; bitmap[0] ^= C0;

	bitmap[3] |= C2;
	bitmap[2] |= C1;
	bitmap[1] |= C0;
	bitmap[0] |= mask ^ (C2 | C1 | C0);
}

void CSplit::_OutSameShunZi(word mask)
{
	_OutShunZi(mask);
	m_wFlushBitmap ^= mask;
}

void CSplit::_InSameShunZi(word mask)
{
	_InShunZi(mask);
	m_wFlushBitmap |= mask;
}
