#include "StdAfx.h"
#include "RPlayer.h"

CRPlayer::CRPlayer()
{
	m_byPhobiaAttack = 0;
	m_byWinRhythm = 0;
}

CRPlayer::~CRPlayer()
{
}

void CRPlayer::SetBitmap(word bitmap[4])
{
	bitmap_bak[0] = bitmap[0];
	bitmap_bak[1] = bitmap[1];
	bitmap_bak[2] = bitmap[2];
	bitmap_bak[3] = bitmap[3];
}

void CRPlayer::RestoreBitmap()
{
	bitmap[0] = bitmap_bak[0];
	bitmap[1] = bitmap_bak[1];
	bitmap[2] = bitmap_bak[2];
	bitmap[3] = bitmap_bak[3];
}

bool CRPlayer::HasCards(byte byGrade, byte byCount)
{
	word _bitmap = 0;
	switch (byCount)
	{
	case 1: _bitmap |= bitmap[0];
	case 2: _bitmap |= bitmap[1];
	case 3: _bitmap |= bitmap[2];
	case 4: _bitmap |= bitmap[3];
	}
	return (_bitmap & (1 << byGrade)) != 0;
}

bool CRPlayer::HasRocket()
{
	word _bitmap = bitmap[0] | bitmap[1] | bitmap[2] | bitmap[3];
	if (_bitmap & CARD_A)
	{
		int _bitmap2 = _bitmap ^ bitmap[0];
		if (_bitmap2 & CARD_4)
			return true;
	}
	return false;
}

bool CRPlayer::HasLianDui()
{
	word _bitmap = bitmap[1] | bitmap[2] | bitmap[3];
	return TblShunZi[_bitmap] != 0;
}
