#ifndef __ToolBar_h__
#define __ToolBar_h__

class CToolBtn;
class CToolBar : public CPanel {
	typedef std::vector<CToolBtn *>		VECTOOLBTN;
  public:
	CToolBar(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize, int nDepth);
	virtual ~CToolBar();

  public:
	void			 AddToolBtn(CToolBtn *pToolBtn);
	void			 DelToolBtn(CToolBtn *pToolBtn);

  public:
	void			 OnSwitchSelect(CToolBtn *pToolBtn);

  public:
	virtual bool	 ProcessLClick(CObject *pObject, UINT nFlags, CPoint point);
	virtual bool	 ProcessRClick(CObject *pObject, UINT nFlags, CPoint point);
	virtual bool	 ProcessDrawRect(CObject *pObject, UINT nFlags, const CRect& rect);

  protected:
	VECTOOLBTN		 m_vecToolBtn;
	CToolBtn		*m_pSelected;
};

#endif
