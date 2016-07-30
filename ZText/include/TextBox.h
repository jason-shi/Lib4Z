#ifndef __TextBox_h__
#define __TextBox_h__

#include <LineTag.h>

class CTextBar;
class CTextBox : public CPanel {
	typedef std::vector<CLineTag *>		VECLINETAG;
  public:
	CTextBox(CPoint xDrawPos, CSize xSize, int nDepth);
	virtual ~CTextBox();

  public:
	bool			 Initial();
	bool			 Release();

  public:
	CLineTag		*GetLineTag(int nLine);

  public:
	void			 ExtOutText(const TCHAR *lpText, int nLength);
	void			 EnhOutText(const TCHAR *lpText, int nLength);

  public:
	void			 EnhScanText(const TCHAR *lpText, int nLength);

  protected:
	int				 m_nCapacity;
	CTextBar	   **m_pTextBar;

  protected:
	int				 m_nTopLine;
	int				 m_nTopMargin;

  protected:
	VECLINETAG		 m_vecLineTag;
};

#endif
