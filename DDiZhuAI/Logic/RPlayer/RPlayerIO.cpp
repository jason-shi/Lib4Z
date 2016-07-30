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

void CRPlayer::OutCards(byte byType, byte byGrade, byte byCount, word wAttach)
{
	switch (byType)
	{
	case TYPE_SINGLE:
		_OutSingle(1 << byGrade);
		break;
	case TYPE_PAIR:
		_OutPair(1 << byGrade);
		break;
	case TYPE_TRIP:
		_OutTrip(1 << byGrade);
		break;
	case TYPE_TRIP_S:
		_OutTripS(1 << byGrade, wAttach);
		break;
	case TYPE_TRIP_P:
		_OutTripP(1 << byGrade, wAttach);
		break;
	case TYPE_PLANE:
		_OutPlane(SHIFT_1(byGrade + byCount) ^ SHIFT_1(byGrade));
		break;
	case TYPE_PLANE_S:
		_OutPlaneS(SHIFT_1(byGrade + byCount) ^ SHIFT_1(byGrade), wAttach);
		break;
	case TYPE_PLANE_P:
		_OutPlaneP(SHIFT_1(byGrade + byCount) ^ SHIFT_1(byGrade), wAttach);
		break;
	case TYPE_QUAD_S:
		_OutQuadS(1 << byGrade, wAttach);
		break;
	case TYPE_QUAD_P:
		_OutQuadP(1 << byGrade, wAttach);
		break;
	case TYPE_BOMB:
		_OutBomb(1 << byGrade);
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

void CRPlayer::InCards(byte byType, byte byGrade, byte byCount, word wAttach)
{
	switch (byType)
	{
	case TYPE_SINGLE:
		_InSingle(1 << byGrade);
		break;
	case TYPE_PAIR:
		_InPair(1 << byGrade);
		break;
	case TYPE_TRIP:
		_InTrip(1 << byGrade);
		break;
	case TYPE_TRIP_S:
		_InTripS(1 << byGrade, wAttach);
		break;
	case TYPE_TRIP_P:
		_InTripP(1 << byGrade, wAttach);
		break;
	case TYPE_PLANE:
		_InPlane(SHIFT_1(byGrade + byCount) ^ SHIFT_1(byGrade));
		break;
	case TYPE_PLANE_S:
		_InPlaneS(SHIFT_1(byGrade + byCount) ^ SHIFT_1(byGrade), wAttach);
		break;
	case TYPE_PLANE_P:
		_InPlaneP(SHIFT_1(byGrade + byCount) ^ SHIFT_1(byGrade), wAttach);
		break;
	case TYPE_QUAD_S:
		_InQuadS(1 << byGrade, wAttach);
		break;
	case TYPE_QUAD_P:
		_InQuadP(1 << byGrade, wAttach);
		break;
	case TYPE_BOMB:
		_InBomb(1 << byGrade);
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

void CRPlayer::_OutTrip(word mask)
{
	if (bitmap[2] & mask) //3张变0张
		bitmap[2] ^= mask;
	else //4张变1张
	{
		bitmap[3] ^= mask;
		bitmap[0] |= mask;
	}
}

void CRPlayer::_InTrip(word mask)
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

void CRPlayer::_OutTripS(word mask, word wAttach)
{
	_OutTrip(mask);
	_OutSingle(wAttach);
}

void CRPlayer::_InTripS(word mask, word wAttach)
{
	_InTrip(mask);
	_InSingle(wAttach);
}

void CRPlayer::_OutTripP(word mask, word wAttach)
{
	_OutTrip(mask);
	_OutPair(wAttach);
}

void CRPlayer::_InTripP(word mask, word wAttach)
{
	_InTrip(mask);
	_InPair(wAttach);
}

void CRPlayer::_OutPlane(word mask)
{
	int C3 = bitmap[3] & mask; bitmap[3] ^= C3;
	int C2 = bitmap[2] & mask; bitmap[2] ^= C2;

	bitmap[0] |= C3;
}

void CRPlayer::_InPlane(word mask)
{
	int C0 = bitmap[0] & mask; bitmap[0] ^= C0;

	bitmap[3] |= C0;
	bitmap[2] |= mask ^ C0;
}

void CRPlayer::_OutPlaneS(word mask, word wAttach)
{
	_OutPlane(mask);
	_OutShunZi(wAttach);
}

void CRPlayer::_InPlaneS(word mask, word wAttach)
{
	_InPlane(mask);
	_InShunZi(wAttach);
}

void CRPlayer::_OutPlaneP(word mask, word wAttach)
{
	_OutPlane(mask);
	_OutLianDui(wAttach);
}

void CRPlayer::_InPlaneP(word mask, word wAttach)
{
	_InPlane(mask);
	_InLianDui(wAttach);
}

void CRPlayer::_OutQuadS(word mask, word wAttach)
{
	_OutBomb(mask);
	_OutShunZi(wAttach);
}

void CRPlayer::_InQuadS(word mask, word wAttach)
{
	_InBomb(mask);
	_InShunZi(wAttach);
}

void CRPlayer::_OutQuadP(word mask, word wAttach)
{
	_OutBomb(mask);
	_OutLianDui(wAttach);
}

void CRPlayer::_InQuadP(word mask, word wAttach)
{
	_InBomb(mask);
	_InLianDui(wAttach);
}

void CRPlayer::_OutBomb(word mask)
{
	bitmap[3] ^= mask;
}

void CRPlayer::_InBomb(word mask)
{
	bitmap[3] |= mask;
}

void CRPlayer::_OutRocket()
{
	bitmap[0] ^= MASK_ROCKET;
}

void CRPlayer::_InRocket()
{
	bitmap[0] |= MASK_ROCKET;
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
