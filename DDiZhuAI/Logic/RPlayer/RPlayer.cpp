#include "StdAfx.h"
#include "RPlayer.h"

CRPlayer::CRPlayer()
{
}

CRPlayer::~CRPlayer()
{
}

void CRPlayer::SetBitmap(word bitmap[4])
{
	//���úͱ���
	memcpy(this->bitmap, bitmap, sizeof(word) * 4);
	memcpy(bitmap_bak,   bitmap, sizeof(word) * 4);
}

void CRPlayer::RestoreBitmap()
{
	memcpy(bitmap, bitmap_bak, sizeof(word) * 4);
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

bool CRPlayer::HasLianDui()
{
	word _bitmap = bitmap[1] | bitmap[2] | bitmap[3];
	return TblShunZi3[_bitmap] != 0;
}

int CRPlayer::GetCardsCount() const
{
	return WORD_BIT_COUNT(bitmap[0]) + (WORD_BIT_COUNT(bitmap[1]) << 1)
		+ (WORD_BIT_COUNT(bitmap[2]) * 3) + (WORD_BIT_COUNT(bitmap[3]) << 2);
}

bool CRPlayer::SuppressPlayer(byte byChair1) const
{
	if ((m_byFriend & (1 << byChair1)) != 0) //�Ƕ���
		return false;
	return m_byWinRhythm & (1 << byChair1) //�����Ƶ������ʤ������
		|| m_byWinRhythm & 0x01; //�Լ���ʤ������
}

bool CRPlayer::ReleasePlayer(byte byChair1) const
{
	if ((m_byFriend & (1 << byChair1)) == 0) //���Ƕ���
		return false;
	return (m_byWinRhythm & 0x01) == 0 //�Լ�û��ʤ������
		&& (m_byWinRhythm & m_byFriend) != 0; //������ʤ������
}
