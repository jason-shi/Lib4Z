#ifndef __Dialog_h__
#define __Dialog_h__

class CDialog : public CPanel {
  public:
	CDialog();
	CDialog(CImage *pImage, CPoint xDrawPos, CSize xSize);
	CDialog(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize);
	virtual ~CDialog();

  public:
	virtual INT_PTR	 DoModal();

  protected:
	virtual BOOL	 PreTranslateMessage(MSG *msg);
	virtual BOOL	 OnIdle(long count);
	virtual int		 Run();
	virtual void	 OnClose();

  protected:
	virtual INT_PTR	 DialogResult();

  protected:
	CObject			*m_pActived;
};

inline BOOL CDialog::OnIdle(long count)
{
	return count >= 100;
}

inline INT_PTR CDialog::DialogResult()
{
	return NULL;
}

#endif
