#ifndef __ScrollWindow_h__
#define __ScrollWindow_h__

#include <boost/function.hpp>

class CBrowseData;
class CBrowseView : public CPanel {
	typedef boost::function<void(CBrowseView *)>			FN_UpdateView;
  public:
	CBrowseView(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize, int nDepth);
	virtual ~CBrowseView();

  public:
	void			 BindBrowseData(CBrowseData *pBrowseData);
	CBrowseData		*GetBrowseData() const;

  public:
	void			 SetAutoScroll(bool bAutoScroll);
	bool			 IsAutoScroll() const;

  public:
	virtual void	 ShowBrowseView() = 0;
	virtual void	 UpdateView() = 0;

  public:
	virtual int		 GetFrontWidth() const = 0;
	virtual int		 GetFrontHeight() const = 0;
	virtual int		 GetFieldWidth() const = 0;
	virtual int		 GetFieldHeight() const = 0;
	virtual int		 GetTotalWidth() const = 0;
	virtual int		 GetTotalHeight() const = 0;

  public:
	virtual void	 OnVScrollWindow(int nDelta) = 0;
	virtual void	 OnHScrollWindow(int nDelta) = 0;
	virtual void	 OnVSlideWindow(int nFront, int nTotal) = 0;
	virtual void	 OnHSlideWindow(int nFront, int nTotal) = 0;

  public:
	virtual void	 OnAdjustWidth(int nItem, int nWidth) = 0;

  public:
	void			 SetUpdateView(FN_UpdateView UpdateView);

  protected:
	CBrowseData		*m_pBrowseData;
	bool			 m_bAutoScroll; //ÊÇ·ñ×Ô¶¯¹öÆÁ

  public:
	FN_UpdateView	 OnUpdateView;
};

inline void CBrowseView::SetAutoScroll(bool bAutoScroll)
{
	m_bAutoScroll = bAutoScroll;
}

inline bool CBrowseView::IsAutoScroll() const
{
	return m_bAutoScroll;
}

inline CBrowseData *CBrowseView::GetBrowseData() const
{
	return m_pBrowseData;
}

inline void CBrowseView::SetUpdateView(FN_UpdateView UpdateView)
{
	OnUpdateView = UpdateView;
}

#endif
