#ifndef __ToolBtn_h__
#define __ToolBtn_h__

class CToolBtn;
typedef boost::function<void(CToolBtn *, UINT nFlags, CPoint point)>		 FN_SwitchSelect;

class CToolBtn : public CButton {
  public:
	CToolBtn(CImage *pImage, CRect xDrawRect, CRect xSrcRect, int nDepth);
	virtual ~CToolBtn();

  protected:
	void			 OnLButtonDown(UINT nFlags, CPoint point);
	void			 OnLButtonUp(UINT nFlags, CPoint point);
	bool			 OnMouseEnter(UINT nFlags, CPoint point);
	bool			 OnMouseLeave(UINT nFlags, CPoint point);

  public:
	void			 SetFN_SwitchSelect(FN_SwitchSelect SwitchSelect);
	FN_SwitchSelect	 GetFN_SwitchSelect() const;

  public:
	virtual bool	 IsSelected() const;
	virtual void	 OnSelect();
	virtual void	 UnSelect();

  public:
	virtual bool	 ProcessLClick(CObject *pObject, UINT nFlags, CPoint point);
	virtual bool	 ProcessRClick(CObject *pObject, UINT nFlags, CPoint point);
	virtual bool	 ProcessDrawRect(CObject *pObject, UINT nFlags, const CRect& rect);

  protected:
	bool			 m_bSelected;

  protected:
	FN_SwitchSelect	 OnSwitchSelect;
};

inline bool CToolBtn::IsSelected() const
{
	return m_bSelected;
}

inline void CToolBtn::SetFN_SwitchSelect(FN_SwitchSelect SwitchSelect)
{
	OnSwitchSelect = SwitchSelect;
}

inline FN_SwitchSelect CToolBtn::GetFN_SwitchSelect() const
{
	return OnSwitchSelect;
}

#endif
