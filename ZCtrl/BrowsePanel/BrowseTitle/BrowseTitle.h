#ifndef __BrowseTitle_h__
#define __BrowseTitle_h__

class CBrowseTitle;
typedef boost::function<void(CBrowseTitle *, int nItem, int nWidth)>	FN_AdjustWidth;

class CBrowseTitle : public CPanel {
	typedef std::vector<CObject *>		VECITEM;
  public:
	CBrowseTitle(CImage *pImage, CPoint xSrcPos, CSize xSize, int nDepth);
	virtual ~CBrowseTitle();

  public:
	virtual bool	 CreateItems() = 0;

  public:
	void			 AppendItem(CObject *pObject);
	void			 InsertItem(int nItem, CObject *pObject);
	void			 RemoveItem(int nItem);
	void			 AdjustWidth(int nItem, int nWidth);
	void			 SetAdjustWidth(FN_AdjustWidth AdjustWidth);

  private:
	CPoint			 _GetDrawPos(int nItem);
	void			 _AdjustFollow(int nItem, CPoint xDrawPos);

  public:
	int				 GetFrontWidth(int nBrowseItem);
	int				 GetTotalWidth(int nViewWidth);

  public:
	void			 OnHScrollWindow(int nDelta);
	void			 OnHSlideWindow(int nFront, int nTotal);

  public:
	int				 CalcItem(int nBrowseItem, int nDelta);
	int				 CalcItem(int nFrontWidth);
	void			 UpdateBrowseTitle(int nBrowseItem);

  protected:
	VECITEM			 m_vecItem;
	int				 m_nBrowseItem;

  protected:
	FN_AdjustWidth	 OnAdjustWidth;
};

inline void CBrowseTitle::SetAdjustWidth(FN_AdjustWidth AdjustWidth)
{
	OnAdjustWidth = AdjustWidth;
}

#endif
