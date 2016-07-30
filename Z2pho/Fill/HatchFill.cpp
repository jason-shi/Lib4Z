#include "StdAfx.h"
#include "HatchFill.h"

CHatchFill::CHatchFill()
{
	m_xOrigin.x = 0;
	m_xOrigin.y = 0;

	//贴图扩展为64 * 8，以提高绘制效率
	m_pHatch = new CImage(64, 8);
	m_pSprite = new CSprite(m_pHatch, CPoint(), CSize(), 0);
}

CHatchFill::~CHatchFill()
{
	delete m_pHatch;
	delete m_pSprite;
}

void CHatchFill::SetStyle(int nStyle)
{
	switch (nStyle)
	{
	case HFS_HORIZONTAL:
		m_nHatchBitmap = 0x00000000000000FF;
		break;
	case HFS_VERTICAL:
		m_nHatchBitmap = 0x0101010101010101;
		break;
	case HFS_FDIAGONAL:
		m_nHatchBitmap = 0x8040201008040201;
		break;
	case HFS_BDIAGONAL:
		m_nHatchBitmap = 0x0102040810204080;
		break;
	case HFS_CROSS:
		m_nHatchBitmap = 0x01010101010101FF;
		break;
	case HFS_DIAGCROSS:
		m_nHatchBitmap = 0x8142241818244281;
		break;
	default:
		m_nHatchBitmap = 0;
	}
}

void CHatchFill::InitFill()
{
	//构造画刷图象
	unsigned __int64 nBitMask = 0x8000000000000000;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			m_pHatch->SetPixel(j, i, m_nHatchBitmap & nBitMask ? m_color : m_bkcolor);
			nBitMask >>= 1;
		}
	}

	//以下是将贴图复制7份，以提高绘制效率
	int nBytesPerGroup = m_pHatch->BytesPerPixel() * 8;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 1; j < 8; j++)
		{
			//将贴图复制7份
			memcpy(m_pHatch->GetBits(j << 3, i), m_pHatch->GetBits(0, i), nBytesPerGroup);
		}
	}
}

void CHatchFill::FillLine(int x1, int x2, int y)
{
	//第一段的源位置
	CPoint xSrcPos;
	xSrcPos.x = (x1 - m_xOrigin.x) & 0x07;
	xSrcPos.y = (y - m_xOrigin.y) & 0x07;

	//总长度，已绘制长度，逐段长度
	int nLength = x2 - x1;
	int nDrawLen = 0;
	int nTempLen = min(64 - xSrcPos.x, nLength);
	do
	{
		m_pSprite->SetSrcPos(xSrcPos);
		m_pSprite->SetDrawPos(x1 + nDrawLen, y);
		m_pSprite->SetSize(nTempLen, 1);
		m_pSprite->Draw(m_pImage);

		//更新长度和源位置 
		nDrawLen += nTempLen;
		nTempLen = min(nLength - nDrawLen, 64);

		xSrcPos.x = 0;
	} while (nDrawLen < nLength);
}
