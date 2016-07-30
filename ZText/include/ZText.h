#ifndef __ZText_h__
#define __ZText_h__

#include <freetype\ftcache.h>
#include <LineTag.h>
#include <TextFmt.h>
#include <TextBlk.h>
#include <TextBar.h>
#include <TextBox.h>

#define TF_TEXT				0x0001 //文本
#define TF_FAMILY			0x0002 //字体
#define TF_WIDTH			0x0003 //宽度
#define TF_HEIGHT			0x0004 //高度
#define TF_TRANSPARENT		0x0005 //透明
#define TF_COLOR			0x0006 //颜色
#define TF_BKCOLOR			0x0007 //背景色
#define TF_STYLE			0x0008 //风格
#define TF_X_SPACE			0x0009 //字间距
#define TF_Y_SPACE			0x000A //行间距
#define TF_RETURN			0x000B //软回车

#define TS_NORMAL			0
#define TS_ITALIC			FT_STYLE_FLAG_ITALIC
#define TS_BOLD				FT_STYLE_FLAG_BOLD

typedef struct MyFaceRec_
{
	const char		*file_path;
	byte			 style;
	int				 face_index;

	~MyFaceRec_()
	{
		delete file_path;
		file_path = NULL;
	}
} MyFaceRec, *MyFace;

struct TFontStyle {
	char			 szFaceName[LF_FACESIZE * 2];
	byte			 byStyle;
	FTC_ScalerRec	 tScalerRec;
};

class CZText {
	typedef stdext::hash_map<std::string, MyFace>	 FACEMAP;
  private:
	CZText();

  public:
	SINGLETON(CZText);
	~CZText();

  public:
	FTC_CMapCache	 GetCMapCache();
	FTC_SBitCache	 GetSBitCache();

  public:
	FTC_FaceID		 GetFaceID(char *pFamily, byte byStyle);

  public:
	void			 SetDefaultFont(HFONT& hFont);
	HFONT&			 GetDefaultFont();
	void			 SetSpaceWidth(int nWidth);
	int				 GetSpaceWidth() const;
	char			*GetFontsDir();

  protected:
	FT_Library		 m_pLibrary;
	FTC_Manager		 m_pCacheManager;
	FTC_CMapCache	 m_pCMapCache;
	FTC_SBitCache	 m_pSBitCache;

  protected:
	FACEMAP			 m_mapFaceID;

  protected:
	HFONT			 m_hDefaultFont;
	int				 m_nSpaceWidth;
	char			 m_szFontsDir[_MAX_PATH];
};

inline FTC_CMapCache CZText::GetCMapCache()
{
	return m_pCMapCache;
}

inline FTC_SBitCache CZText::GetSBitCache()
{
	return m_pSBitCache;
}

inline void CZText::SetDefaultFont(HFONT& hFont)
{
	m_hDefaultFont = hFont;
}

inline HFONT& CZText::GetDefaultFont()
{
	return m_hDefaultFont;
}

inline void CZText::SetSpaceWidth(int nWidth)
{
	m_nSpaceWidth = nWidth;
}

inline int CZText::GetSpaceWidth() const
{
	return m_nSpaceWidth;
}

inline char *CZText::GetFontsDir()
{
	return m_szFontsDir;
}

#define g_pZText			 CZText::GetInstancePtr()

#ifdef	_MSC_VER
	#ifdef	_DEBUG
		#pragma comment(lib, "ZTextD.lib")
	#else
		#pragma comment(lib, "ZText.lib")
	#endif
#endif

#endif
