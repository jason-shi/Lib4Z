#ifndef __TextFmt_h__
#define __TextFmt_h__

class CTextFmt {
  public:
	CTextFmt(int nCapacity);
	virtual ~CTextFmt();

  public:
	bool			 Initial();
	bool			 Release();
	TCHAR			*GetBuff() const;
	int				 GetLength() const;

  public:
	void			 WriteText(TCHAR *pText);
	void			 WriteText(TCHAR *pText, int L);
	void			 WriteFamily(TCHAR *pFamily, int L);
	void			 WriteWidth(short shWidth);
	void			 WriteHeight(short shHeight);
	void			 WriteTransparent(bool bTransparent);
	void			 WriteColor(COLORREF color);
	void			 WriteBkColor(COLORREF bkcolor);
	void			 WriteStyle(char chStyle);
	void			 WriteXSpace(short shXSpace);
	void			 WriteYSpace(short shYSpace);
	void			 WriteReturn();

  private:
	void			 _SetBool(const bool value);
	void			 _SetChar(const char value);
	void			 _SetShort(const short value);
	void			 _SetColor(const COLORREF color);
	void			 _SetString(const TCHAR *pString, int L);

  protected:
	TCHAR			*m_wcsBuff;
	int				 m_nLength;
	int				 m_nCapacity;
};

inline TCHAR *CTextFmt::GetBuff() const
{
	return m_wcsBuff;
}

inline int CTextFmt::GetLength() const
{
	return m_nLength;
}

inline void CTextFmt::WriteText(TCHAR *pText)
{
	WriteText(pText, wcslen(pText));
}

#endif
