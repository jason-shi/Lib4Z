#ifndef __PageManage_h__
#define __PageManage_h__

class CPageBase;
class CPageManage : public CPanel {
  public:
	CPageManage(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize, int nDepth);
	virtual ~CPageManage();

  public:
	void			 AddPage(CPageBase *pPage);
	void			 DelPage(CPageBase *pPage);
	void			 ActivePage(CPageBase *pPage);

  private:
	void			 _AddPage(CPageBase *pPage);
	void			 _DelPage(CPageBase *pPage);

  protected:
	CPageBase		*m_pHeadPage;
	CPageBase		*m_pActived;
};

#endif
