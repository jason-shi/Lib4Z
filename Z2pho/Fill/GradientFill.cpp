/******************************************************************************
 *
 * ��ֱ������㷨��б�������ȫ��ͬ�����뵥������
 * ����ͼ�ķ�ʽ��佥������ܸ�Ч��
 * ���ǵ����������ȷǳ���ʱ��ֻ�ܲ��������ػ��ơ�
 * Ȩ�����ߣ�ȡ��ͼ����󳤶�Ϊ4096�������Ǻ��ʵġ�
 *
 ******************************************************************************/
#include "StdAfx.h"
#include "GradientFill.h"

#define MaxSpriteLength			4096

CGradientFill::CGradientFill() : CFill()
{
	m_pGradient = new CImage(MaxSpriteLength, 1);
	m_pSprite = new CSprite(m_pGradient, CPoint(), CSize(), 0);
}

CGradientFill::~CGradientFill()
{
	delete m_pGradient;
	delete m_pSprite;
}

void CGradientFill::InitFill()
{
	int dx = m_xOrigin2.x - m_xOrigin1.x;
	int dy = m_xOrigin2.y - m_xOrigin1.y;

	//������
	m_nLength = dx == 0 ? dy : dx + dy * dy / dx;
	m_nLength = abs(m_nLength);

	int b1 = GetBValue(m_color);
	int g1 = GetGValue(m_color);
	int r1 = GetRValue(m_color);

	//������ɫ�������
	m_db = (float)(GetBValue(m_bkcolor) - b1) / m_nLength;
	m_dg = (float)(GetGValue(m_bkcolor) - g1) / m_nLength;
	m_dr = (float)(GetRValue(m_bkcolor) - r1) / m_nLength;

	//��ֱ�ͽ�����Сʱ���콥����ͼ
	if (m_nLength <= MaxSpriteLength)
	{
		//б����ʱ����Ϊ1 ��ֱ����ʱ�ϵ���Ϊ1
		int uxuy = dx > 0 || dx == 0 && dy > 0 ? 1 : -1;
		int nOffset = uxuy == 1 ? 0 : m_nLength - 1;

		//��������ͳһ����
		for (int i = 0; i < m_nLength; i++, nOffset += uxuy)
		{
			int b = b1 + m_db * nOffset;
			int g = g1 + m_dg * nOffset;
			int r = r1 + m_dr * nOffset;

			m_pGradient->SetPixel(i, 0, BGRA(b, g, r, 255));
		}
	}
}

void CGradientFill::FillLine(int x1, int x2, int y)
{
	//��ֱ������뵥������
	m_xOrigin1.x == m_xOrigin2.x ? _LineVert(x1, x2, y) : _LineLean(x1, x2, y);
}

void CGradientFill::_LineVert(int x1, int x2, int y)
{
	//�����������½���
	int minY = min(m_xOrigin1.y, m_xOrigin2.y);
	int maxY = max(m_xOrigin1.y, m_xOrigin2.y);

	//��ò�ͬ�������ɫ
	COLORREF color;
	if (y < minY)
		color = m_xOrigin2.y > m_xOrigin1.y ? m_color : m_bkcolor;
	else if (y >= maxY)
		color = m_xOrigin2.y > m_xOrigin1.y ? m_bkcolor : m_color;
	else
		color = m_pGradient->GetPixel(y - minY, 0);

	m_pImage->FillLine(x1, x2, y, color);
}

void CGradientFill::_LineLean(int x1, int x2, int y)
{
	int dx = m_xOrigin2.x - m_xOrigin1.x;
	int dy = m_xOrigin2.y - m_xOrigin1.y;

	//��б�����ߴ�ֱ����ֱ����ˮƽɨ���ߵ����ҽ���
	int minX = (float)dy / dx * (m_xOrigin1.y - y) + m_xOrigin1.x;
	if (dx < 0)
		minX -= m_nLength;
	int maxX = minX + m_nLength;

	//���������ɫ
	if (x1 < minX)
		m_pImage->FillLine(x1, min(minX, x2), y, dx > 0 ? m_color : m_bkcolor);

	//�����ұ���ɫ
	if (x2 >= maxX)
		m_pImage->FillLine(max(maxX, x1), x2, y, dx > 0 ? m_bkcolor : m_color);

	//���ƽ�������
	Span span;
	span.nStart = max(x1, minX);
	span.nEnd = min(x2, maxX);
	span.nLength = span.nEnd - span.nStart;
	span.y = y;
	if (span.nLength > 0)
	{
		//����ͼ�ķ�ʽ��Ч��б�ʺܴ�ʱֻ�ܲ��������ػ���
		int nSrcPos = max(-minX, 0);
		m_nLength <= MaxSpriteLength ? _LineBySprite(span, nSrcPos) : _LineByPixel(span, nSrcPos);
	}
}

void CGradientFill::_LineBySprite(const Span& span, int nSrcPos)
{
	m_pSprite->SetSrcPos(nSrcPos, 0);
	m_pSprite->SetDrawPos(span.nStart, span.y);
	m_pSprite->SetSize(span.nLength, 1);

	m_pSprite->Draw(m_pImage);
}

void CGradientFill::_LineByPixel(const Span& span, int nSrcPos)
{
	int b1 = GetBValue(m_color);
	int g1 = GetGValue(m_color);
	int r1 = GetRValue(m_color);

	//б���䣺����Ϊ1���ҵ���Ϊ-1
	int ux = m_xOrigin1.x < m_xOrigin2.x ? 1 : -1;
	int nOffset = m_xOrigin1.x < m_xOrigin2.x ? nSrcPos : m_nLength - nSrcPos - 1;

	//��������ͳһ����
	for (int i = 0; i < span.nLength; i++, nOffset += ux)
	{
		int b = b1 + m_db * nOffset;
		int g = g1 + m_dg * nOffset;
		int r = r1 + m_dr * nOffset;

		m_pImage->SetPixel(span.nStart + i, span.y, BGRA(b, g, r, 255));
	}
}
