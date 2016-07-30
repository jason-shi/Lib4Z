#ifndef __DigitPanel_h__
#define __DigitPanel_h__

class CDigitPanel : public CPanel {
  public:
	CDigitPanel(CImage *pDigitImage, CRect xSrcRect, CSize xDigitSize, int nMaxDigit, char chFiller, bool bWithSign, int nSignGap = 0);
	virtual ~CDigitPanel();

  public:
	bool			 Initial();
	void			 Set(CImage *pDigitImage, CRect xSrcRect, CSize xDigitSize, int nMaxDigit, char chFiller, bool bWithSign, int nSignGap = 0);
	void			 SetDigitImage(CImage *pDigitImage);
	void			 SetSrcRect(CRect xSrcRect);
	void			 SetDigitSize(CSize xDigitSize);
	void			 SetMaxDigit(int nMaxDigit);
	void			 SetFiller(char chFiller);
	void			 SetWithSign(bool bWithSign, int nSignGap = 0);

  protected:
	int				 GetTotalDigit() const;
	void			 ShowWithSign();
	void			 ShowWithoutSign();

  public:
	void			 ClearDigit();
	void			 ShowDigit(int nNumber);

  protected:
	CImage			*m_pDigitImage;
	CRect			 m_xSrcRect;
	CSize			 m_xDigitSize;
	int				 m_nMaxDigit;
	char			 m_chFiller;
	bool			 m_bWithSign;
	int				 m_nSignGap;

  protected:
	int				 m_nNumber;

  protected:
	CSprite			 SpriteSign;
	CSprite			 SpriteDigit[10]; //(int)(32 * lg2) + 1
};

inline void CDigitPanel::SetDigitImage(CImage *pDigitImage)
{
	m_pDigitImage = pDigitImage;
}

inline void CDigitPanel::SetSrcRect(CRect xSrcRect)
{
	m_xSrcRect = xSrcRect;
}

inline void CDigitPanel::SetDigitSize(CSize xDigitSize)
{
	m_xDigitSize = xDigitSize;
}

inline void CDigitPanel::SetMaxDigit(int nMaxDigit)
{
	m_nMaxDigit = nMaxDigit;
}

inline void CDigitPanel::SetFiller(char chFiller)
{
	m_chFiller = chFiller;
}

inline void CDigitPanel::SetWithSign(bool bWithSign, int nSignGap)
{
	m_bWithSign = bWithSign;
	m_nSignGap = nSignGap;
}

#endif
