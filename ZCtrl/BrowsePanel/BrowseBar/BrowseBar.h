#ifndef __BrowseBar_h__
#define __BrowseBar_h__

class CBrowseData;
class CBrowseBar : public CPanel {
	typedef std::vector<CObject *>		VECITEM;
  public:
	CBrowseBar(CImage *pImage, CPoint xSrcPos, CSize xSize, int nDepth);
	virtual ~CBrowseBar();

  public:
	virtual bool	 CreateItems() = 0;
	virtual void	 Load(CBrowseData *pBrowseData, int nDataIndex) = 0;

  public:
	void			 _Load(CBrowseData *pBrowseData, int nDataIndex);

  public:
	void			 AppendItem(CObject *pObject);
	void			 InsertItem(int nItem, CObject *pObject);
	void			 RemoveItem(int nItem);
	void			 OnAdjustWidth(int nItem, int nWidth);

  private:
	CPoint			 _GetDrawPos(int nItem);
	void			 _AdjustFollow(int nItem, CPoint xDrawPos);

  public:
	int				 GetFrontWidth(int nBrowseItem);
	int				 GetTotalWidth(int nViewWidth);

  public:
	int				 CalcItem(int nBrowseItem, int nDelta);
	int				 CalcItem(int nFrontWidth);
	void			 UpdateBrowseBar(int nBrowseItem);

  protected:
	VECITEM			 m_vecItem;
	int				 m_nDataIndex; //-1ÎªÎÞÐ§
};

#endif
