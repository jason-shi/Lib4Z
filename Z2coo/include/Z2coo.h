#ifndef __Z2coo_h__
#define __Z2coo_h__

#include <Image.h>
#include <DrawImage.h>
#include <Sprite.h>
#include <Panel.h>
#include <Dialog.h>
#include <Button.h>
#include <WarCraft.h>
#include <WarPanel.h>
#include <LianKan.h>
#include <LianPanel.h>

#ifndef  SINGLETON
#define  SINGLETON(type) \
	static inline type* GetInstancePtr() \
	{ \
		static type _instance; \
		return &_instance; \
	}
#endif

class CZ2coo {
  public:
	SINGLETON(CZ2coo);
	~CZ2coo();

  protected:
	CZ2coo();

  public:
	bool			 Initial();
	void			 Release();
	void			 SetWindow(CWindow *_window);
	CWindow			*GetWindow() const;
	bool			 IsActived(CObject *pObject) const;
	void			 SetActived(CObject *pObject);
	CObject			*GetActived() const;
	CPoint			 GetLDown() const;
	CPoint			 GetRDown() const;
	void			 Draw(CImage *pImage);
	void			 Draw(CImage *pImage, const CRect& rect);
	void			 Redraw(CObject *pObject);
	void			 Redraw(const CRect& rect);
	void			 Redraw(CObject *pObject, const CRect& add); //附加区域
	void			 TrackMouseEvent(int nEventTrack);
	void			 SendMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0);
	void			 PostMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0);

  public:
	void			 AddObject(CObject *pObject);
	void			 DelObject(CObject *pObject);
	CObject			*FindObject(const CPoint& point);
	CObject			*FindExcept(const CPoint& point, CObject *pExcept);

  public:
	virtual void	 OnLButtonDown(UINT nFlags, CPoint point);
	virtual void	 OnLButtonUp(UINT nFlags, CPoint point);
	virtual void	 OnMouseMove(UINT nFlags, const CPoint& point);
	virtual void	 OnMouseWheel(UINT nFlags, short zDelta, CPoint point);
	virtual void	 OnRButtonDown(UINT nFlags, CPoint point);
	virtual void	 OnRButtonUp(UINT nFlags, CPoint point);
	virtual void	 OnMButtonDown(UINT nFlags, CPoint point);
	virtual void	 OnMButtonUp(UINT nFlags, CPoint point);

  private:
	void			 _OnLButtonUp(UINT nFlags, CPoint point);
	void			 _OnMouseMove(UINT nFlags, const CPoint& point);
	bool			 _PtInPressed(UINT nFlags, CPoint point);
	void			 _ResetFocus(UINT nFlags, CPoint point);

  protected:
	virtual bool	 CheckDrawThrs(CPoint point);
	virtual void	 OnDrawRect(UINT nFlags, const CPoint& point);
	virtual void	 OnDrawOver(UINT nFlags, const CPoint& point);

  protected:
	virtual bool	 CheckDragThrs(CPoint point);
	virtual void	 OnMousePick(UINT nFlags, const CPoint& point);
	virtual void	 OnMouseDrag(UINT nFlags, const CPoint& point);
	virtual void	 OnMouseDrop(UINT nFlags, const CPoint& point);

  protected:
	void			 TerminateAction(UINT nFlags, const CPoint& point);

  private:
	CWindow			*window;
	CRect			 ClientRect;

  private:
	CContainer		*m_pContainer;

  private:
	int				 m_nLStatus;
	CObject			*m_pActived;
	CPoint			 m_xLDown; //左键按下位置
	CPoint			 m_xRDown; //右键按下位置
};

inline void CZ2coo::SetWindow(CWindow *_window)
{
	window = _window;
	window->GetClientRect(&ClientRect);
}

inline CWindow *CZ2coo::GetWindow() const
{
	return window;
}

inline bool CZ2coo::IsActived(CObject *pObject) const
{
	return m_pActived == pObject;
}

inline void CZ2coo::SetActived(CObject *pObject)
{
	m_pActived = pObject;
}

inline CObject *CZ2coo::GetActived() const
{
	return m_pActived;
}

inline CPoint CZ2coo::GetLDown() const
{
	return m_xLDown;
}

inline CPoint CZ2coo::GetRDown() const
{
	return m_xRDown;
}

inline void CZ2coo::Draw(CImage *pImage)
{
	m_pContainer->Draw(pImage);
}

inline void CZ2coo::Draw(CImage *pImage, const CRect& rect)
{
	m_pContainer->Draw(pImage, rect);
}

inline void CZ2coo::Redraw(CObject *pObject)
{
	CRect rect;
	pObject->GetRect(&rect);
	window->InvalidateRect(&rect);
}

inline void CZ2coo::Redraw(const CRect& rect)
{
	window->InvalidateRect(&rect);
}

inline void CZ2coo::Redraw(CObject *pObject, const CRect& add)
{
	CRect rect;
	pObject->GetRect(&rect);
	rect |= add;
	window->InvalidateRect(&rect);
}

inline void CZ2coo::TrackMouseEvent(int nEventTrack)
{
	m_pContainer->TrackMouseEvent(nEventTrack);
}

inline void CZ2coo::SendMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	window->SendMessage(uMsg, wParam, lParam);
}

inline void CZ2coo::PostMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	window->PostMessage(uMsg, wParam, lParam);
}

inline void CZ2coo::AddObject(CObject *pObject)
{
	m_pContainer->AddObject(pObject);
}

inline void CZ2coo::DelObject(CObject *pObject)
{
	m_pContainer->DelObject(pObject);
}

inline CObject *CZ2coo::FindObject(const CPoint& point)
{
	return m_pContainer->FindObject(point);
}

inline CObject *CZ2coo::FindExcept(const CPoint& point, CObject *pExcept)
{
	return m_pContainer->FindExcept(point, pExcept);
}

#define g_pZ2coo			 CZ2coo::GetInstancePtr()

#ifdef	_MSC_VER
	#ifdef	_DEBUG
		#pragma comment(lib, "Z2cooD.lib")
	#else
		#pragma comment(lib, "Z2coo.lib")
	#endif
#endif

#endif
