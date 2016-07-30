#ifndef __PolygonShape_h__
#define __PolygonShape_h__

#include "Shape.h"

class CPolygonShape : public CShape {
	typedef struct Edge {
		int				 ymax; //边的最大y坐标
		double			 x; //与当前扫描线的交点x坐标
		double			 dx; //边所在直线斜率的倒数,增量
		Edge			*pNext; //指向下一条边
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
	void			 _BuildET(LPEdge *pET, int y0, int y1); //构建边表
	void			 _SortAET(LPEdge pAET); //按照x递增排序活性边表

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
