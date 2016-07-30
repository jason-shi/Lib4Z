#ifndef __ZLexer_h__
#define __ZLexer_h__

#ifndef  SINGLETON
#define  SINGLETON(type) \
	static inline type* GetInstancePtr() \
	{ \
		static type _instance; \
		return &_instance; \
	}
#endif

class CZLexer {
	typedef std::wstring		WSTRING;
  public:
	SINGLETON(CZLexer);
	~CZLexer();
  private:
	CZLexer();

  public:
	static int		 ParseLine(WSTRING strLine, std::vector<WSTRING>& vecReturn, const TCHAR cTrim);
	static int		 GetNumber(TCHAR *wcsBuff, int &pos, const TCHAR cTrim);
	static bool		 GetNumber(dword *pResult, const TCHAR* wcsBuff, int &pos);
	static bool		 GetIP4Addr(dword *pResult, const TCHAR* wcsBuff, int &pos);
	static bool		 GetIP6Addr(dword *pResult, const TCHAR* wcsBuff, int &pos);
	static bool		 GetMACAddr(byte *pResult, const TCHAR* wcsBuff, int &pos);
	static bool		 GetDecimal(double *pResult, const TCHAR* wcsBuff, int &pos);
	static bool		 GetMoment(dword *pResult, const TCHAR* wcsBuff, int &pos);

  public:
	static WSTRING	 ReplaceLString(WSTRING szInput, WSTRING szOld, WSTRING szNew);
	static WSTRING	 ReplaceRString(WSTRING szInput, WSTRING szOld, WSTRING szNew);

  public:
	static TCHAR	*TrimLString(const TCHAR* wcsBuff);
	static TCHAR	*TrimRString(const TCHAR* wcsBuff);
	static TCHAR	*TrimString(const TCHAR* wcsBuff);

  public:
	static bool		 GetAttribute(TCHAR *wcsBuff, WSTRING attribute, int& nReturn);
	static bool		 GetAttributeStr(TCHAR *wcsBuff, WSTRING attribute, WSTRING& wcsReturn);
};

#define g_pZLexer			 CZLexer::GetInstancePtr()

#ifdef	_MSC_VER
	#ifdef	_DEBUG
		#pragma comment(lib, "ZLexerD.lib")
	#else
		#pragma comment(lib, "ZLexer.lib")
	#endif
#endif

#endif
