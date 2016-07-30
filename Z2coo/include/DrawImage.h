#ifndef	__DrawImage_h__
#define	__DrawImage_h__

#include "Image.h"
#include "Misc.h"
#pragma		comment(lib,"Msimg32.lib")

class CDrawImage : public CImage	{
  public:
	CDrawImage();
	~CDrawImage();

	BOOL			 Create(HDC	dc,	int	width, int height);

	void			 Draw(HDC dc, int x, int y,	int	w, int h, int ox=0,	int	oy=0);
	void			 Draw(HDC dc, const	CRect &rect, CPoint	point);
	void			 Draw(HDC dc, const	CRect &rect);

	HBITMAP			 GetBitmapHandle() const	{ return hBitmap; }

	void			 DrawText(HFONT hFont, int x, int y, const char *str, COLORREF color = BGR(255, 255, 255));
	void			 WipeIn(CImage *image, const CRect &rect, int count);
	void			 WipeOut(const CRect &rect, int count);
	void			 FadeFromBlack(CImage *image, const CRect &rect, int count);
	void			 FadeToBlack(CImage *image, const CRect &rect, int count);
	void			 FadeFromWhite(CImage *image, const CRect &rect, int count);
	void			 FadeToWhite(CImage *image, const CRect &rect, int count);

  protected:
	void			 FadeCvt(CImage *image, const CRect &rect, const byte *cvt);

  protected:
	BITMAPINFOHEADER InfoHeader;
	HBITMAP			 hBitmap;
};

inline void	CDrawImage::Draw(HDC dc, int x,	int	y, int w, int h, int ox, int oy)
{
	HDC	memdc =	CreateCompatibleDC(dc);
	HGDIOBJ	oldbmp = SelectObject(memdc, hBitmap);
	BLENDFUNCTION ftn;
	ftn.BlendOp = AC_SRC_OVER;
	ftn.BlendFlags = 0;
	ftn.AlphaFormat = 0;
	ftn.SourceConstantAlpha = 255;
	AlphaBlend(dc, x, y, w,	h, memdc, ox, oy, w, h, ftn);
	GdiFlush();
	SelectObject(memdc,	oldbmp);
	DeleteDC(memdc);
}

inline void	CDrawImage::Draw(HDC dc, const CRect &rect,	CPoint point)
{
	Draw(dc, rect.left,	rect.top, rect.Width(),	rect.Height(), point.x,	point.y);
}

inline void	CDrawImage::Draw(HDC dc, const CRect &rect)
{
	Draw(dc, rect.left,	rect.top, rect.Width(),	rect.Height(), rect.left, rect.top);
}

#endif
