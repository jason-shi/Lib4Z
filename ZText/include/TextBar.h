#ifndef __TextBar_h__
#define __TextBar_h__

struct TFontStyle;
class CTextBar : public CPanel {
  public:
	CTextBar(bool bMultiLine, int nHeight, CPoint xDrawPos, CSize xSize, int nDepth);
	virtual ~CTextBar();

  public:
	bool			 Initial();
	bool			 Release();
	void			 Set(bool bMultiLine, int nHeight, CPoint xDrawPos, CSize xSize, int nDepth);

  public:
	void			 ClearText();
	int				 ExtOutText(const TCHAR *lpText, int nLength, CLineTag *pLineTag);
	int				 EnhOutText(const TCHAR *lpText, int nLength, CLineTag *pLineTag);
	int				 ExtScanText(const TCHAR *lpText, int nLength, CLineTag *pLineTag);
	int				 EnhScanText(const TCHAR *lpText, int nLength, CLineTag *pLineTag);

  protected:
	void			 SetRotate(FT_Face &face);

  protected:
	FTC_SBit		 GetBitmap(TCHAR tch, FTC_ScalerRec &tScalerRec);
	void			 AdjustBitmap(FTC_SBit bitmap);
	bool			 PenAdvance(CRect &rect, int nXSpace, int nYSpace, FTC_SBit bitmap);
	void			 FillGlyph(CRect &rect, int nWidth, int nHeight, int nYBearing, COLORREF bkcolor);
	void			 DrawGlyph(FTC_SBit bitmap, bool transparent, COLORREF color);

  private:
	void			 _DrawMonoGlyph(FTC_SBit bitmap, bool transparent, COLORREF color);
	void			 _DrawGrayGlyph(FTC_SBit bitmap, bool transparent, COLORREF color);

  protected:
	bool			 m_bMultiLine;
	int				 m_nHeight;
	int				 m_nBaseLine;
	CPoint			 m_xPencil;
};

#endif
