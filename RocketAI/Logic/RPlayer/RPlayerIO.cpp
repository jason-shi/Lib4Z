/******************************************************************************
 *                                顺子更新说明
 *                                                        ——周运栋
 *
 * 张数1 2 3 4   mask
 *     0 0 1 0   0                 0 0 1 0                 0 0 1 0
 *     0 1 0 0   1  bitmap ^= Cx   0 0 0 0  bitmap |= Cx-1 1 0 0 0
 *     1 0 0 0   1    =======>     0 0 0 0    =======>     0 0 0 0
 *     0 0 0 1   1                 0 0 0 0                 0 0 1 0
 *     1 0 0 0   0                 1 0 0 0                 1 0 0 0
 *     0 0 0 0   0                 0 0 0 0                 0 0 0 0
 *
 * 1.bitmap[3] & mask得到 000100(记为C3)，然后bitmap[3] ^= C3
 * 2.bitmap[2] & mask得到 000000(记为C2)，然后bitmap[2] ^= C2
 * 3.bitmap[1] & mask得到 010000(记为C1)，然后bitmap[1] ^= C1
 * 4.bitmap[0] & mask得到 001000(记为C0)，然后bitmap[0] ^= C0
 *
 * 5.bitmap[2] |= C3
 * 6.bitmap[1] |= C2
 * 7.bitmap[0] |= C1
 *
 *****************************************************************************/
#include "StdAfx.h"
#include "RPlayer.h"

using namespace std;

#define SHIFT_1(n)				((1 << (n)) - 1)

void CRPlayer::OutCards(byte byType, byte byGrade, byte byCount)
{
	switch (byType)
	{
	case TYPE_SINGLE:
	case TYPE_GOU:
		_OutSingle(1 << byGrade);
		break;
	case TYPE_PAIR:
	case TYPE_CHA:
		_OutPair(1 << byGrade);
		break;
	case TYPE_BOMB:
		_OutBomb(1 << byGrade);
		break;
	case TYPE_CANNON:
		_OutCannon(1 << byGrade);
		break;
	case TYPE_REDFLAG:
		_OutRedFlag();
		break;
	case TYPE_ROCKET:
		_OutRocket();
		break;
	case TYPE_SHUNZI:
		_OutShunZi(SHIFT_1(byGrade + byCount) ^ SHIFT_1(byGrade));
		break;
	case TYPE_LIANDUI:
		_OutLianDui(SHIFT_1(byGrade + byCount) ^ SHIFT_1(byGrade));
		break;
	}
}

void CRPlayer::InCards(byte byType, byte byGrade, byte byCount)
{
	switch (byType)
	{
	case TYPE_SINGLE:
	case TYPE_GOU:
		_InSingle(1 << byGrade);
		break;
	case TYPE_PAIR:
	case TYPE_CHA:
		_InPair(1 << byGrade);
		break;
	case TYPE_BOMB:
		_InBomb(1 << byGrade);
		break;
	case TYPE_CANNON:
		_InCannon(1 << byGrade);
		break;
	case TYPE_REDFLAG:
		_InRedFlag();
		break;
	case TYPE_ROCKET:
		_InRocket();
		break;
	case TYPE_SHUNZI:
		_InShunZi(SHIFT_1(byGrade + byCount) ^ SHIFT_1(byGrade));
		break;
	case TYPE_LIANDUI:
		_InLianDui(SHIFT_1(byGrade + byCount) ^ SHIFT_1(byGrade));
		break;
	}
}

void CRPlayer::_OutSingle(word mask)
{
	if (bitmap[0] & mask) //1张变0张
		bitmap[0] ^= mask;
	else if (bitmap[1] & mask) //2张变1张
	{
		bitmap[1] ^= mask;
		bitmap[0] |= mask;
	}
	else if (bitmap[2] & mask) //3张变2张
	{
		bitmap[2] ^= mask;
		bitmap[1] |= mask;
	}
	else //4张变3张
	{
		bitmap[3] ^= mask;
		bitmap[2] |= mask;
	}
}

void CRPlayer::_InSingle(word mask)
{
	if (bitmap[0] & mask) //1张变2张
	{
		bitmap[0] ^= mask;
		bitmap[1] |= mask;
	}
	else if (bitmap[1] & mask) //2张变3张
	{
		bitmap[1] ^= mask;
		bitmap[2] |= mask;
	}
	else if (bitmap[2] & mask) //3张变4张
	{
		bitmap[2] ^= mask;
		bitmap[3] |= mask;
	}
	else //0张变1张
	{
		bitmap[0] |= mask;
	}
}

void CRPlayer::_OutPair(word mask)
{
	if (bitmap[1] & mask) //2张变0张
		bitmap[1] ^= mask;
	else if (bitmap[2] & mask) //3张变1张
	{
		bitmap[2] ^= mask;
		bitmap[0] |= mask;
	}
	else //4张变2张
	{
		bitmap[3] ^= mask;
		bitmap[1] |= mask;
	}
}

void CRPlayer::_InPair(word mask)
{
	if (bitmap[0] & mask) //1张变3张
	{
		bitmap[0] ^= mask;
		bitmap[2] |= mask;
	}
	else if (bitmap[1] & mask) //2张变4张
	{
		bitmap[1] ^= mask;
		bitmap[3] |= mask;
	}
	else //0张变2张
	{
		bitmap[1] |= mask;
	}
}

void CRPlayer::_OutBomb(word mask)
{
	if (bitmap[2] & mask) //3张变0张
		bitmap[2] ^= mask;
	else //4张变1张
	{
		bitmap[3] ^= mask;
		bitmap[0] |= mask;
	}
}

void CRPlayer::_InBomb(word mask)
{
	if (bitmap[0] & mask) //1张变4张
	{
		bitmap[0] ^= mask;
		bitmap[3] |= mask;
	}
	else //0张变3张
	{
		bitmap[2] |= mask;
	}
}

void CRPlayer::_OutCannon(word mask)
{
	bitmap[3] ^= mask;
}

void CRPlayer::_InCannon(word mask)
{
	bitmap[3] |= mask;
}

void CRPlayer::_OutRedFlag()
{
	bitmap[0] ^= MASK_REDFLAG;
}

void CRPlayer::_InRedFlag()
{
	bitmap[0] |= MASK_REDFLAG;
}

void CRPlayer::_OutRocket()
{
	_OutSingle(CARD_A);
	_OutPair(CARD_4);
}

void CRPlayer::_InRocket()
{
	_InSingle(CARD_A);
	_InPair(CARD_4);
}

void CRPlayer::_OutShunZi(word mask)
{
	int C3 = bitmap[3] & mask; bitmap[3] ^= C3;
	int C2 = bitmap[2] & mask; bitmap[2] ^= C2;
	int C1 = bitmap[1] & mask; bitmap[1] ^= C1;
	int C0 = bitmap[0] & mask; bitmap[0] ^= C0;

	bitmap[2] |= C3;
	bitmap[1] |= C2;
	bitmap[0] |= C1;
}

void CRPlayer::_InShunZi(word mask)
{
	int C2 = bitmap[2] & mask; bitmap[2] ^= C2;
	int C1 = bitmap[1] & mask; bitmap[1] ^= C1;
	int C0 = bitmap[0] & mask; bitmap[0] ^= C0;

	bitmap[3] |= C2;
	bitmap[2] |= C1;
	bitmap[1] |= C0;
	bitmap[0] |= mask ^ (C2 | C1 | C0);
}

void CRPlayer::_OutLianDui(word mask)
{
	int C3 = bitmap[3] & mask; bitmap[3] ^= C3;
	int C2 = bitmap[2] & mask; bitmap[2] ^= C2;
	int C1 = bitmap[1] & mask; bitmap[1] ^= C1;

	bitmap[1] |= C3;
	bitmap[0] |= C2;
}

void CRPlayer::_InLianDui(word mask)
{
	int C1 = bitmap[1] & mask; bitmap[1] ^= C1;
	int C0 = bitmap[0] & mask; bitmap[0] ^= C0;

	bitmap[3] |= C1;
	bitmap[2] |= C0;
	bitmap[1] |= mask ^ (C1 | C0);
}
