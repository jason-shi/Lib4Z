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
		tLineShape.SetColor(m_color); //�̳ж���ε���ɫ
		tLineShape.Draw(pCanvas);
	}
	g_pZ2coo->Redraw(pCanvas);
}

void CPolygonShape::_Fill(CCanvas *pCanvas)
{
	int y0, y1; //ɨ���ߵ�������Сy����
	LPEdge pAET; //��߱�ͷָ��
	LPEdge *pET; //�߱�ͷָ��
	pAET = new Edge; //��ʼ����ͷָ��
	pAET->pNext = NULL; //��ȡy����ɨ���߽߱�
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

	//��ʼ���߱���һ����㲻�ã��������Ա�֤pET[i]�ǿ�
	pET = new LPEdge[y1 - y0 + 1];
	for (int i = 0; i <= y1 - y0; i++)
	{
		pET[i] = new Edge;
		pET[i]->pNext = NULL;
	}
	_BuildET(pET, y0, y1);
	//ɨ��
	for (int i = y0; i <= y1; i++)
	{
		LPEdge peg0 = pET[i-y0]->pNext;
		LPEdge peg1 = pET[i-y0];
		if (peg0) //���±߼���
		{
			while (peg1->pNext)
				peg1 = peg1->pNext;
			peg1->pNext = pAET->pNext;
			pAET->pNext = peg0;
		}
		_SortAET(pAET);
		//������߱�����
		peg0 = pAET;
		while (peg0->pNext && peg0->pNext->pNext)
		{
			FillLine(ROUND(peg0->pNext->x), ROUND(peg0->pNext->pNext->x), i);
			peg0 = peg0->pNext->pNext;
		}
		//��ymax = i�Ľڵ�ӻ�߱�ɾ������ÿ���ڵ��xֵ����dx
		peg0 = pAET;
		while (peg0->pNext)
		{
			if (peg0->pNext->ymax < i + 2)
			{
				peg1 = peg0->pNext;
				peg0->pNext = peg0->pNext->pNext;
				//ɾ��
				delete peg1;
				continue;
			}
			peg0->pNext->x += peg0->pNext->dx; //��ÿ���ڵ��xֵ����dx
			peg0 = peg0->pNext;
		}
	}
	//ɾ���߱�
	for (int i = 0; i < y1 - y0 + 1; i++)
		if (pET[i])
			delete pET[i];
	if (pAET)
		delete pAET;
	if (pET)
		delete[] pET;

	//����Է�ֹ�߽���ʧ
	Draw(pCanvas);

	g_pZ2coo->Redraw(pCanvas);
}

void CPolygonShape::_BuildET(LPEdge *pET, int y0, int y1)
{
	for(int i = 0;i < m_nCount;i++)
	{
		int j = (i + 1) % m_nCount;
		if (lpPoints[i].y != lpPoints[j].y) //�����ˮƽ��
		{
			LPEdge peg; //ָ��ñߵ�ָ��
			LPEdge ppeg; //ָ���ָ���ָ��
			//�����
			peg = new Edge;
			int k = (lpPoints[i].y > lpPoints[j].y) ? i : j;
			peg->ymax = lpPoints[k].y; //�ñ����y����
			k = (k == j) ? i : j;
			peg->x = (double)lpPoints[k].x; //�ñ���ɨ���߽���x����
			peg->dx = (double)(lpPoints[i].x - lpPoints[j].x) / (lpPoints[i].y - lpPoints[j].y); //�ñ�б�ʵĵ���
			peg->pNext = NULL; //�����
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
