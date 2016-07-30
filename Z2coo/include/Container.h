#ifndef __Container_h__
#define __Container_h__

#include "Object.h"

class CContainer : public CObject {
  public:
	CContainer();
	CContainer(CPoint xDrawPos, CSize xSize, int nDepth);
	virtual ~CContainer();

  public:
	bool			 Initial();
	bool			 Release();

  public:
	void			 SetDrawPos(CPoint xDrawPos);
	void			 SetDrawPos(int x, int y);
	void			 Offset(CPoint point);
	void			 Offset(int x, int y);
	void			 _UpdateOffset(CPoint point);
	void			 Draw(CImage *pImage);
	void			 Draw(CImage *pImage, const CRect &rect);

  public:
	void			 AddObject(CObject *pObject);
	void			 DelObject(CObject *pObject);
	bool			 Including(CObject *pObject);
	virtual CObject	*FindObject(const CPoint& point);
	virtual CObject	*FindWheel(const CPoint& point);
	virtual CObject	*FindExcept(const CPoint& point, CObject *pExcept);

  private:
	void			 _AddObject(CObject *pObject);
	void			 _DelObject(CObject *pObject);

  protected:
	CObject			*m_pHead;
};

inline void CContainer::SetDrawPos(CPoint xDrawPos)
{
	SetDrawPos(xDrawPos.x, xDrawPos.y);
}

inline void CContainer::Offset(CPoint point)
{
	Offset(point.x, point.y);
}

#endif
