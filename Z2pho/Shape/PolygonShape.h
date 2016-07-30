#ifndef __PolygonShape_h__
#define __PolygonShape_h__

#include "Shape.h"

class CPolygonShape : public CShape {
	typedef struct Edge {
		int				 ymax; //�ߵ����y����
		double			 x; //�뵱ǰɨ���ߵĽ���x����
		double			 dx; //������ֱ��б�ʵĵ���,����
		Edge			*pNext; //ָ����һ����
	} Edge, *LPEdge;

  public:
	CPolygonShape(int nCount, LPPOINT lpPoints);
	~CPolygonShape();

  public:
	void			 Set(int nCount, LPPOINT lpPoints);

  private:
	void			 _Draw(CCanvas *pCanvas);
	void			 _Fill(CCanvas *pCanvas);

  private:
	void			 _BuildET(LPEdge *pET, int y0, int y1); //�����߱�
	void			 _SortAET(LPEdge pAET); //����x����������Ա߱�

  protected:
	int				 m_nCount;
	LPPOINT			 lpPoints;
};

inline void CPolygonShape::Set(int nCount, LPPOINT lpPoints)
{
	this->m_nCount = nCount;
	this->lpPoints = lpPoints;
}

#endif
