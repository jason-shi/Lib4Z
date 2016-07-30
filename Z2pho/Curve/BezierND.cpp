#include "StdAfx.h"
#include "BezierND.h"

CBezierND::CBezierND(int nCount, LPPOINT lpPoints)
				: CBezierBase(nCount, lpPoints)
{
}

CBezierND::~CBezierND()
{
}

void CBezierND::DeCasteljau(int ptx[], int pty[])
{
	int pointx[MaxBezierDegree][MaxBezierDegree];
	int pointy[MaxBezierDegree][MaxBezierDegree];

	for (int i = 0; i < m_nCount; i++)
	{
		pointx[0][i] = ptx[i];
		pointy[0][i] = pty[i];
	}
	//逐层计算分割点
	for (int i = 1; i < m_nCount; i++)
	{
		for (int j = 0; j < m_nCount - i; j++)
		{
			pointx[i][j] = (pointx[i - 1][j] + pointx[i - 1][j + 1]) >> 1;
			pointy[i][j] = (pointy[i - 1][j] + pointy[i - 1][j + 1]) >> 1;
		}
	}
	//只压入右半部分
	for (int i = 0; i < m_nCount; i++)
	{
		m_span.ptx[i] = pointx[m_nCount - i - 1][i];
		m_span.pty[i] = pointy[m_nCount - i - 1][i];
	}
	m_stack.push_back(m_span);

	//暂存左半部分
	for (int i = 0; i < m_nCount; i++)
	{
		m_span.ptx[i] = pointx[i][0];
		m_span.pty[i] = pointy[i][0];
	}
}

bool CBezierND::CheckTermination(int ptx[], int pty[])
{
	int dx = abs(ptx[m_nCount - 1] - ptx[0]);
	int dy = abs(pty[m_nCount - 1] - pty[0]);

	int epsilon = 65536;
	if (dy > dx && dy < dx + dx || dy <= dx && dy + dy > dx)
		epsilon <<= 2;

	//max(|Xi - Xi-1|, |Yi - Yi-1|) <= epsilon
	for (int i = 1; i < m_nCount; i++)
	{
		if (abs(ptx[i] - ptx[i - 1]) > epsilon || abs(pty[i] - pty[i - 1]) > epsilon)
			return false;
	}

	return true;
}
