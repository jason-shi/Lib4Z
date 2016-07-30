#ifndef	__Image_h__
#define	__Image_h__

#include "Dib.h"
#include "Misc.h"

//取消RGB格式
#undef RGB
#undef GetRValue
#undef GetBValue
#undef GetGValue

//定义BGRA格式
#define BGR(b, g, r)		((COLORREF)(((BYTE)(b)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(r))<<16)))
#define BGRA(b, g, r, a)	((COLORREF)(((BYTE)(b)|((WORD)((BYTE)(g))<<8))|(((DWORD)(BYTE)(r))<<16)|((DWORD)(BYTE)(a)<<24)))

#define GetBValue(bgra)		(LOBYTE(bgra))
#define GetGValue(bgra)		(LOBYTE(((WORD)(bgra)) >> 8))
#define GetRValue(bgra)		(LOBYTE((bgra)>>16))
#define GetAValue(bgra)		(LOBYTE((bgra)>>24))

#define alpha_composite(composite, fg, alpha, bg) { \
	unsigned short temp = ((unsigned short)(fg) * (unsigned short)(alpha) + \
	(unsigned short)(bg) * (unsigned short)(255 - (unsigned short)(alpha)) + (unsigned short)128); \
	(composite) = (unsigned short)((temp + (temp >> 8)) >> 8); \
}


class CDC;
class CImage : public CDib {
  public:
	CImage() : CDib() {}
	CImage(int nWidth, int nHeight);

  public:
	BOOL			 Create(int	nWidth, int nHeight);
	BOOL			 LoadImage(const TCHAR* filename);

	void			 Copy(const CImage* pImage);
	void			 Copy(const CImage* pImage, const CRect& rect);
	void			 Copy(const CImage* pImage, const CRect& rect, CPoint src_pos);
	void			 MixImage(const CImage* pImage);
	void			 MixImage(const CImage* pImage, const CRect& rect);
	void			 MixImage(const CImage* pImage, const CRect& rect, CPoint src_pos);

  public:
	int				 GetPixel(const CPoint& point);
	int				 GetPixel(int x, int y);
	void			 SetPixel(const CPoint& point, COLORREF color);
	void			 SetPixel(int x, int y, COLORREF color);

  public:
	void			 FastLineH(int x1, int x2, int y, COLORREF color);
	void			 FastLineL(int x1, int y1, int x2, int y2, COLORREF color);
	void			 FastLineV(int x, int y1, int y2, COLORREF color);
	void			 FastLineB(int x1, int y1, int x2, int y2, COLORREF color);

  public:
	void			 FillLine(int x1, int x2, int y, COLORREF color);
	void			 FillRect(const CRect& rect, COLORREF color);
	void			 FillRect(int x, int y, int w, int h, COLORREF color);
	void			 GrayedRect(const CRect& rect);

	void			 ZoomRect(const CImage* pImage, const CRect& rect, CPoint src_pos,
							  float fRatioX, float fRatioY); //缩放

	void			 Convolve(const CRect& rect, float *lpKernel); //3*3区域卷积

  private:
	void			 _Convolve_ExtCopy(CImage *pImage, CPoint src_pos);
};

inline void	CImage::Copy(const CImage* pImage)
{
	Copy(pImage, CRect(0, 0, pImage->Width(), pImage->Height()));
}

inline void	CImage::Copy(const CImage* pImage, const CRect& rect)
{
	Copy(pImage, rect, CPoint(0, 0));
}

inline void CImage::MixImage(const CImage* pImage)
{
	MixImage(pImage, CRect(0, 0, pImage->Width(), pImage->Height()));
}

inline void CImage::MixImage(const CImage* pImage, const CRect& rect)
{
	MixImage(pImage, rect, CPoint(0, 0));
}

inline int CImage::GetPixel(const CPoint& point)
{
	return GetPixel(point.x, point.y);
}

inline int CImage::GetPixel(int x, int y)
{
	return *(int *)GetBits(x, y);
}

inline void CImage::SetPixel(const CPoint& point, COLORREF color)
{
	SetPixel(point.x, point.y, color);
}

inline void CImage::SetPixel(int x, int y, COLORREF color)
{
	if (x >= 0 && x < W && y >= 0 && y < H)
		*(int *)GetBits(x, y) = color;
}

inline void CImage::FillRect(int x, int y, int w, int h, COLORREF color)
{
	FillRect(CRect(x, y, x + w, y + h), color);
}

#endif
