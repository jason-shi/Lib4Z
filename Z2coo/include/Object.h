#ifndef __Object_h__
#define __Object_h__

#include <boost/function.hpp>

//鼠标部分行为回调
#define MP_MOUSEENTER			0
#define MP_MOUSELEAVE			1
#define MP_LCLICK				2
#define MP_RCLICK				3
#define MP_MCLICK				4
#define MP_MOUSEPICK			5
#define MP_MOUSEDRAG			6
#define MP_MOUSEDROP			7
#define MP_MAX					8

//鼠标支持的扩展行为
#define MA_LDBLCLK				0x00000001
#define MA_RDBLCLK				0x00000002
#define MA_KEEPLDOWN			0x00000004
#define MA_KEEPRDOWN			0x00000008
#define MA_MOUSEWHEEL			0x00000010

//在父窗口的居中对齐方式
#define ADC_X_DIRECTION			0x01
#define ADC_Y_DIRECTION			0x02

class CObject;
typedef boost::function<void(CObject *, UINT nFlags, CPoint point)>		 FN_MouseProc;

class CImage;
class CObject : public CWater {
  public:
	CObject();
	CObject(CPoint xDrawPos, CSize xSize, int nDepth);
	virtual ~CObject();

  public:
	void			 SetObjId(int nObjId);
	int				 GetObjId() const;
	CPoint			 GetOffset() const;
	CPoint			 GetDrawPos() const;
	void			 SetSize(CSize xSize);
	void			 SetSize(int cx, int cy);
	CSize			 GetSize() const;
	void			 GetRect(CRect *rect) const;
	BOOL			 PtIn(CPoint point);
	void			 SetDepth(int nDepth);
	int				 GetDepth() const;
	void			 SetCease(bool bCease = true);
	void			 Show(bool bShow = true);
	bool			 IsShow() const;
	void			 SetHome(CObject *pHome);
	CObject			*GetHome() const;
	void			 SetInteract(bool bInteract);
	bool			 IsInteract() const;
	void			 RevDraw(CImage *pImage);
	void			 RevDraw(CImage *pImage, const CRect& rect);
	void			 EnableAction(int nMouseAction);
	bool			 IsEnableAction(int nMouseAction) const;
	void			 TrackMouseEvent(int nEventTrack);
	void			 SetMouseProc(int nType, FN_MouseProc MouseProc);
	FN_MouseProc	 GetMouseProc(int nType) const;

  public:
	virtual void	 SetDrawPos(CPoint xDrawPos);
	virtual void	 SetDrawPos(int x, int y);
	virtual void	 Offset(CPoint point);
	virtual void	 Offset(int x, int y);
	virtual void	 _UpdateOffset(CPoint point);
	virtual void	 AdjustAnchor(CPoint point, const CRect& rect);
	virtual void	 AdjustCenter(const CRect& rect, byte byFlag);
	virtual void	 SetImage(CImage *pImage);
	virtual CImage	*GetImage() const;
	virtual void	 SetSrcPos(CPoint xSrcPos);
	virtual void	 SetSrcPos(int x, int y);
	virtual CPoint	 GetSrcPos() const;
	virtual void	 Draw(CImage *pImage);
	virtual void	 Draw(CImage *pImage, const CRect& rect);

  public:
	virtual bool	 IsExist() const;
	virtual CObject	*FindObject(const CPoint& point);
	virtual CObject	*FindWheel(const CPoint& point);
	virtual CObject	*FindExcept(const CPoint& point, CObject *pExcept);

  public:
	virtual void	 OnLoseFocus();
	virtual void	 OnMouseMove(UINT nFlags, CPoint point);
	virtual void	 OnMouseWheel(UINT nFlags, short zDelta, CPoint point);
	virtual bool	 OnMouseEnter(UINT nFlags, CPoint point);
	virtual bool	 OnMouseLeave(UINT nFlags, CPoint point);
	virtual void	 OnLButtonDown(UINT nFlags, CPoint point);
	virtual void	 OnLButtonUp(UINT nFlags, CPoint point);
	virtual void	 OnRButtonDown(UINT nFlags, CPoint point);
	virtual void	 OnRButtonUp(UINT nFlags, CPoint point);
	virtual void	 OnMButtonDown(UINT nFlags, CPoint point);
	virtual void	 OnMButtonUp(UINT nFlags, CPoint point);
	virtual void	 OnLClick(UINT nFlags, CPoint point);
	virtual void	 OnRClick(UINT nFlags, CPoint point);
	virtual void	 OnMClick(UINT nFlags, CPoint point);
	virtual void	 OnLDblClk(UINT nFlags, CPoint point);
	virtual void	 OnRDblClk(UINT nFlags, CPoint point);
	virtual void	 OnMDblClk(UINT nFlags, CPoint point);
	virtual void	 OnTrackLeave(UINT nFlags, CPoint point);
	virtual void	 OnTrackHover(UINT nFlags, CPoint point);

  public:
	virtual void	 OnDragInteract(CObject *pObject);
	virtual void	 OnDropInteract(CObject *pObject);

  public:
	virtual void	 SetDragMode(byte byDragMode);
	virtual byte	 GetDragMode() const;
	virtual bool	 CheckDragMode(UINT nFlags);
	virtual void	 SetDrawMode(byte byDrawMode);
	virtual byte	 GetDrawMode() const;
	virtual bool	 CheckDrawMode(UINT nFlags);

  public:
	virtual int		 GetDrawThrs();
	virtual void	 OnDrawRect(UINT nFlags, const CRect& rect);
	virtual void	 OnDrawOver(UINT nFlags, const CRect& rect);

  public:
	virtual int		 GetDragThrs() const;
	virtual void	 OnMousePick(UINT nFlags, CPoint point);
	virtual void	 OnMouseDrag(UINT nFlags, CPoint point);
	virtual void	 OnMouseDrop(UINT nFlags, CPoint point);

  protected:
	int				 m_nObjId;
	CPoint			 m_xOffset;
	CPoint			 m_xDrawPos;
	CSize			 m_xSize;
	int				 m_nDepth;
	bool			 m_bCease     : 1;
	bool			 m_bShow      : 1;
	bool			 m_bExist     : 1;
	bool			 m_bInteract  : 1;
	byte			 m_byDragMode;
	byte			 m_byDrawMode;
	int				 m_nMouseAction;
	int				 m_nEventTrack;
	FN_MouseProc	 OnMouseProc[MP_MAX];

  protected:
	static CPoint	 DragOffset;

  public:
	CObject			*m_pNext;
	CObject			*m_pHome;
	CObject			*m_pParent;
};

inline void CObject::SetObjId(int nObjId)
{
	m_nObjId = nObjId;
}

inline int CObject::GetObjId() const
{
	return m_nObjId;
}

inline void CObject::SetDrawPos(CPoint xDrawPos)
{
	SetDrawPos(xDrawPos.x, xDrawPos.y);
}

inline void CObject::Offset(CPoint point)
{
	Offset(point.x, point.y);
}

inline CPoint CObject::GetOffset() const
{
	return m_xOffset;
}

inline CPoint CObject::GetDrawPos() const
{
	return m_xDrawPos;
}

inline void CObject::SetSize(CSize xSize)
{
	SetSize(xSize.cx, xSize.cy);
}

inline CSize CObject::GetSize() const
{
	return m_xSize;
}

inline void CObject::SetImage(CImage *pImage)
{
}

inline CImage* CObject::GetImage() const
{
	return NULL;
}

inline void CObject::SetSrcPos(CPoint xSrcPos)
{
}

inline void CObject::SetSrcPos(int x, int y)
{
}

inline CPoint CObject::GetSrcPos() const
{
	return CPoint(0, 0);
}

inline void CObject::GetRect(CRect *rect) const
{
	rect->left = m_xOffset.x + m_xDrawPos.x;
	rect->top = m_xOffset.y + m_xDrawPos.y;
	rect->right = rect->left + m_xSize.cx;
	rect->bottom = rect->top + m_xSize.cy;
}

inline BOOL CObject::PtIn(CPoint point)
{
	CRect rect;
	GetRect(&rect);
	return rect.PtInRect(point);
}

inline void CObject::SetDepth(int nDepth)
{
	m_nDepth = nDepth;
}

inline int CObject::GetDepth() const
{
	return m_nDepth;
}

inline void CObject::SetCease(bool bCease)
{
	m_bCease = bCease;
}

inline bool CObject::IsShow() const
{
	return m_bShow;
}

inline void CObject::SetHome(CObject *pHome)
{
	m_pHome = pHome;
}

inline CObject *CObject::GetHome() const
{
	return m_pHome;
}

inline void CObject::SetInteract(bool bInteract)
{
	m_bInteract = bInteract;
}

inline bool CObject::IsInteract() const
{
	return m_bInteract;
}

inline void CObject::SetMouseProc(int nType, FN_MouseProc MouseProc)
{
	OnMouseProc[nType] = MouseProc;
}

inline FN_MouseProc CObject::GetMouseProc(int nType) const
{
	return OnMouseProc[nType];
}

inline bool CObject::IsExist() const
{
	return m_bShow;
}

inline void CObject::_UpdateOffset(CPoint point)
{
	m_xOffset = point;
}

inline void CObject::SetDrawMode(byte byDrawMode)
{
	m_byDrawMode = byDrawMode;
}

inline byte CObject::GetDrawMode() const
{
	return m_byDrawMode;
}

inline bool CObject::CheckDrawMode(UINT nFlags)
{
	return m_byDrawMode == nFlags;
}

inline void CObject::SetDragMode(byte byDragMode)
{
	m_byDragMode = byDragMode;
}

inline byte CObject::GetDragMode() const
{
	return m_byDragMode;
}

inline bool CObject::CheckDragMode(UINT nFlags)
{
	return m_byDragMode == nFlags;
}

inline void CObject::EnableAction(int nMouseAction)
{
	m_nMouseAction |= nMouseAction;
}

inline bool CObject::IsEnableAction(int nMouseAction) const
{
	return (m_nMouseAction & nMouseAction) != 0;
}

inline void CObject::TrackMouseEvent(int nEventTrack)
{
	m_nEventTrack = nEventTrack;
}

#endif
