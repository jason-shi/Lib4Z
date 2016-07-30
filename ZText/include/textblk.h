#ifndef __TextBlk_h__
#define __TextBlk_h__

struct TFontStyle;
class CTextBlk : public CPanel {
  public:
	CTextBlk(bool bMultiLine, int nHeight, CPoint xDrawPos, CSize xSize, int nDepth);
	virtual ~CTextBlk();

  public:
	bool			 Initial();
	bool			 Release();
	void			 Set(bool bMultiLine, int nHeight, CPoint xDrawPos, CSize xSize, int nDepth);

  public:
	void			 ClearText();
	int				 OutText(const TCHAR *lpText, int nLength, HFONT hFont, bool transparent,
								 COLORREF color, COLORREF bkcolor = BGRA(255, 255, 255, 255), int nXSpace = 0, int nYSpace = 0);
	int				 ExtOutText(const TCHAR *lpText, int nLength);
	int				 EnhOutText(const TCHAR *lpText, int nLength);

  protected:
	void			 SetRotate(FT_Face &face);

  protected:
	void			 InitialFontStyle(TFontStyle &tFontStyle, LOGFONT &logFont);
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

  protected:
	CPoint			 m_xPencil;
};

#endif
