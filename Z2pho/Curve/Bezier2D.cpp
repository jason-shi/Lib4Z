#include "StdAfx.h"
#include "Bezier2D.h"

CBezier2D::CBezier2D(int nCount, LPPOINT lpPoints)
				: CBezierBase(nCount, lpPoints)
{
}

CBezier2D::~CBezier2D()
{
}

void CBezier2D::DeCasteljau(int ptx[], int pty[])
{
	int ax = (ptx[0] + ptx[1]) >> 1;
	int ay = (pty[0] + pty[1]) >> 1;

	//只压入右半部分
	Span tNewSpan;
	tNewSpan.ptx[2] = ptx[2];
	tNewSpan.pty[2] = pty[2];
	tNewSpan.ptx[1] = (ptx[1] + ptx[2]) >> 1;
	tNewSpan.pty[1] = (pty[1] + pty[2]) >> 1;
	tNewSpan.ptx[0] = (ax + tNewSpan.ptx[1]) >> 1;
	tNewSpan.pty[0] = (ay + tNewSpan.pty[1]) >> 1;
	m_stack.push_back(tNewSpan);

	//暂存左半部分
	m_span.ptx[1] = ax;
	m_span.pty[1] = ay;
	m_span.ptx[2] = tNewSpan.ptx[0];
	m_span.pty[2] = tNewSpan.pty[0];
}

bool CBezier2D::CheckTermination(int ptx[], int pty[])
{
	int dx = abs(ptx[2] - ptx[0]);
	int dy = abs(pty[2] - pty[0]);

	int epsilon = 65536;
	if (dy > dx && dy < dx + dx || dy <= dx && dy + dy > dx)
		epsilon <<= 2;

	//max(|Xi - Xi-1|, |Yi - Yi-1|) <= epsilon
	return abs(ptx[1] - ptx[0]) <= epsilon && abs(pty[1] - pty[0]) <= epsilon
		&& abs(ptx[2] - ptx[1]) <= epsilon && abs(pty[2] - pty[1]) <= epsilon;
}
