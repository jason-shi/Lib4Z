#include "StdAfx.h"
#include "NewtonCDS.h"

CNewtonCDS::CNewtonCDS(int N, const CComplex& C0) : N(N), C0(C0)
{
}

CNewtonCDS::~CNewtonCDS()
{
}

int CNewtonCDS::IterBrief(CComplex cinit, CComplex& ret, double dL[4])
{
	CComplex c1(cinit);
	CComplex c2;

	int i, t = 0;
	for (i = 1; i <= m_nMaxIter; i++)
	{
		//调用牛顿迭代公式
		IterFormula(c1, c2);

		double product = (c2 - c1).GetProduct();
		if (product < 0.00001)
		{
			if (++t > 2) dL[3] = product / 0.00001; //找到近似解
			if (i > m_nMinIter) break;
		}
		if (i == 1)
		{
			ret = c2 - c1;
			dL[0] = ret.GetModule();
		}
		if (i == m_nMinIter)
		{
			dL[1] = (c2 - c1).GetModule();
			dL[2] = c2.GetModule();
		}
		c1 = c2;
	}
	return i - m_nMaxIter / N;
}

void CNewtonCDS::Draw(CCanvas *pCanvas)
{
	CImage *pImage = pCanvas->GetImage();
	CComplex cinit; //迭代的初值

	//从中间开始向两边绘制时使用的变量
	for (int n = 1; n < pImage->Height(); n++)
	{
		int B = pImage->Height() / 2 - (1 - (n % 2) * 2) * n / 2;
		for (int A = 0; A < pImage->Width(); A++)
		{
			cinit.a = (m_end.a - m_start.a) * A / pImage->Width() + m_start.a;
			cinit.b = (m_end.b - m_start.b) * B / pImage->Height() + m_start.b;
			//防止出现(0, 0)点
			if (cinit.a == 0) cinit.a = 1.0E-150;
			if (cinit.b == 0) cinit.b = 1.0E-150;

			COLORREF color = GetColor(cinit);
			pImage->SetPixel(A, B, color);
		}
	}
}

COLORREF CNewtonCDS::GetColor(CComplex cinit)
{
	byte b = GetBValue(m_color);
	byte g = GetGValue(m_color);
	byte r = GetRValue(m_color);

	CComplex ret;
	double dL[4] = { 0 };

	//计算迭代后的点
	int i = IterBrief(cinit, ret, dL);

	int newb = 256 - abs((int)abs(b - (2 * r - 256) * (log(abs(ret.a))) * m_fFactor) % 512 - 256);
	int newg = 256 - abs((int)abs(g - (2 * b - 256) * (log(abs(dL[0]))) * m_fFactor) % 512 - 256);
	int newr = 256 - abs((int)abs(r - (2 * g - 256) * (log(abs(ret.b))) * m_fFactor) % 512 - 256);

	return BGRA(max(0, min(255, newb)), max(0, min(255, newg)), max(0, min(255, newr)), 255);
}
