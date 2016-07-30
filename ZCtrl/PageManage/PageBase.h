#ifndef __PageBase_h__
#define __PageBase_h__

class CPageBase : public CPanel {
  public:
	CPageBase(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize, int nDepth);
	~CPageBase();

  public:
	void			 SetActive(bool bActive = true);
	bool			 IsActive() const;

  public:
	virtual bool	 Initial();
	virtual bool	 Rebuild();
	virtual bool	 Release();
	virtual void	 Destory();

  protected:
	bool			 m_bActive;

  public:
	CPageBase		*m_pNextPage;
};

inline bool CPageBase::IsActive() const
{
	return m_bActive;
}

#endif
