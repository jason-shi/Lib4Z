#include "StdAfx.h"

CPolygonShape::CPolygonShape(int nCount, LPPOINT lpPoints)
					: m_nCount(nCount), lpPoints(lpPoints)
{
}

CPolygonShape::~CPolygonShape()
{
}

void CPolygonShape::_Draw(CCanvas *pCanvas)
{
	for (int i = 0; i < m_nCount; i++)
	{
		int j = (i + 1) % m_nCount;
		CLineShape tLineShape(lpPoints[i].x, lpPoints[i].y, lpPoints[j].x, lpPoints[j].y);
		tLineShape.SetColor(m_color); //继承多边形的颜色
		tLineShape.Draw(pCanvas);
	}
	g_pZ2coo->Redraw(pCanvas);
}

void CPolygonShape::_Fill(CCanvas *pCanvas)
{
	int y0, y1; //扫描线的最大和最小y坐标
	LPEdge pAET; //活化边表头指针
	LPEdge *pET; //边表头指针
	pAET = new Edge; //初始化表头指针
	pAET->pNext = NULL; //获取y方向扫描线边界
	y0 = y1 = lpPoints[0].y;
	for (int i = 1; i < m_nCount; i++)
	{
		if (lpPoints[i].y < y0)
			y0 = lpPoints[i].y;
		else if (lpPoints[i].y > y1)
			y1 = lpPoints[i].y;
	}
	if (y0 >= y1)
		return;

	//初始化边表，第一个结点不用，这样可以保证pET[i]非空
	pET = new LPEdge[y1 - y0 + 1];
	for (int i = 0; i <= y1 - y0; i++)
	{
		pET[i] = new Edge;
		pET[i]->pNext = NULL;
	}
	_BuildET(pET, y0, y1);
	//扫描
	for (int i = y0; i <= y1; i++)
	{
		LPEdge peg0 = pET[i-y0]->pNext;
		LPEdge peg1 = pET[i-y0];
		if (peg0) //有新边加入
		{
			while (peg1->pNext)
				peg1 = peg1->pNext;
			peg1->pNext = pAET->pNext;
			pAET->pNext = peg0;
		}
		_SortAET(pAET);
		//遍历活边表，画线
		peg0 = pAET;
		while (peg0->pNext && peg0->pNext->pNext)
		{
			FillLine(ROUND(peg0->pNext->x), ROUND(peg0->pNext->pNext->x), i);
			peg0 = peg0->pNext->pNext;
		}
		//把ymax = i的节点从活边表删除并把每个节点的x值递增dx
		peg0 = pAET;
		while (peg0->pNext)
		{
			if (peg0->pNext->ymax < i + 2)
			{
				peg1 = peg0->pNext;
				peg0->pNext = peg0->pNext->pNext;
				//删除
				delete peg1;
				continue;
			}
			peg0->pNext->x += peg0->pNext->dx; //把每个节点的x值递增dx
			peg0 = peg0->pNext;
		}
	}
	//删除边表
	for (int i = 0; i < y1 - y0 + 1; i++)
		if (pET[i])
			delete pET[i];
	if (pAET)
		delete pAET;
	if (pET)
		delete[] pET;

	//镶边以防止边界损失
	Draw(pCanvas);

	g_pZ2coo->Redraw(pCanvas);
}

void CPolygonShape::_BuildET(LPEdge *pET, int y0, int y1)
{
	for(int i = 0;i < m_nCount;i++)
	{
		int j = (i + 1) % m_nCount;
		if (lpPoints[i].y != lpPoints[j].y) //加入非水平边
		{
			LPEdge peg; //指向该边的指针
			LPEdge ppeg; //指向边指针的指针
			//构造边
			peg = new Edge;
			int k = (lpPoints[i].y > lpPoints[j].y) ? i : j;
			peg->ymax = lpPoints[k].y; //该边最大y坐标
			k = (k == j) ? i : j;
			peg->x = (double)lpPoints[k].x; //该边与扫描线焦点x坐标
			peg->dx = (double)(lpPoints[i].x - lpPoints[j].x) / (lpPoints[i].y - lpPoints[j].y); //该边斜率的倒数
			peg->pNext = NULL; //插入边
			ppeg = pET[lpPoints[k].y-y0];
			while (ppeg->pNext)
				ppeg = ppeg->pNext;
			ppeg->pNext = peg;
		}
	}
}

void CPolygonShape::_SortAET(LPEdge pAET)
{
	LPEdge peg0 = pAET;
	while (peg0->pNext)
	{
		LPEdge pegmax = peg0;
		LPEdge peg1 = peg0;
		LPEdge pegi = NULL;
		while (peg1->pNext)
		{
			if (peg1->pNext->x > pegmax->pNext->x)
				pegmax = peg1;
			peg1 = peg1->pNext;
		}
		pegi = pegmax->pNext;
		pegmax->pNext = pegi->pNext;
		pegi->pNext = pAET->pNext;
		pAET->pNext = pegi;
		if (peg0 == pAET)
			peg0 = pegi;
	}
}
