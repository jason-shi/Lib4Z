#include "StdAfx.h"

CDrawImage::CDrawImage()
	:CImage(), hBitmap(0)
{
}

CDrawImage::~CDrawImage()
{
	GdiFlush();
	if (hBitmap) {
		::DeleteObject(hBitmap);
	}
}

BOOL CDrawImage::Create(HDC	dc,	int	width, int height)
{
	W =	width;
	H =	height;
	D =	32;

	bytes_per_line = ScanBytes(width, 32);
	bytes_per_pixel	= PixelBytes(32);

	InfoHeader.biSize			= sizeof(BITMAPINFOHEADER);
	InfoHeader.biWidth			= width;
	InfoHeader.biHeight			= height;
	InfoHeader.biBitCount		= 32;
	InfoHeader.biPlanes			= 1;
	InfoHeader.biXPelsPerMeter	= 0;
	InfoHeader.biYPelsPerMeter	= 0;
	InfoHeader.biClrUsed		= 0;
	InfoHeader.biClrImportant	= 0;
	InfoHeader.biCompression	= BI_RGB;
	InfoHeader.biSizeImage		= bytes_per_line * height;

	Info = (BITMAPINFO *)&InfoHeader;
	hBitmap	= CreateDIBSection(dc, Info, DIB_RGB_COLORS, &Bits,	NULL, 0);

	return hBitmap != 0;
}

void CDrawImage::DrawText(HFONT hFont, int x, int y, const char *str, COLORREF color)
{
	CMemoryDC	dc(0);
	HBITMAP	oldBitmap = dc.SelectObject(hBitmap);
	HFONT	oldFont = dc.SelectObject(hFont);

	dc.SetTextColor(color);
	dc.SetBkMode(TRANSPARENT);

	WCHAR szStr[4096];
	MultiByteToWideChar(CP_ACP, 0, str, -1, szStr, sizeof(szStr) / sizeof(WCHAR));
	dc.ExtTextOut(x, y, 0, 0, szStr, wcslen(szStr), NULL);

	dc.SelectObject(oldBitmap);
	dc.SelectObject(oldFont);

	GdiFlush();
}

//
// 擦入处理
//
void CDrawImage::WipeIn(CImage *image, const CRect &rect, int count)
{
	int		len = rect.Width() * 4;
	for (int y=rect.top + count; y<rect.bottom; y+=8) {
		memcpy(GetBits(rect.left, y), image->GetBits(rect.left, y), len);
	}
}

//
// 擦出处理
//
void CDrawImage::WipeOut(const CRect &rect, int count)
{
	int		len = rect.Width() * 4;
	for (int y=rect.top + count; y<rect.bottom; y+=8) {
		memset(GetBits(rect.left, y), 0, len);
	}
}

//
// Fading处理
//
void CDrawImage::FadeCvt(CImage *image, const CRect &rect, const byte *cvt)
{
	for (int y = rect.top; y < rect.bottom; y++) {
		byte *p1 = (byte *)GetBits(rect.left, y);
		byte *p2 = (byte *)image->GetBits(rect.left, y);

		for (int x = rect.left; x < rect.right; x++) {
			*p1++ = cvt[*p2++];		// blue
			*p1++ = cvt[*p2++];		// green
			*p1++ = cvt[*p2++];		// red
			*p1++ = cvt[*p2++];		// alpha
		}
	}
}

//
// “黑->图像”处理
//
void CDrawImage::FadeFromBlack(CImage *image, const CRect &rect, int count)
{
	byte	cvt[256];

	count++;
	for (int i=0; i<256; i++) {
		cvt[i] = (byte)((i * count) / 16);
	}
	FadeCvt(image, rect, cvt);
}

//
//  “图像->黑”处理
//
void CDrawImage::FadeToBlack(CImage *image, const CRect &rect, int count)
{
	byte	cvt[256];

	count = 15 - count;
	for (int i=0; i<256; i++) {
		cvt[i] = (byte)((i * count) / 16);
	}
	FadeCvt(image, rect, cvt);
}

//
// “白->图像”处理
//
void CDrawImage::FadeFromWhite(CImage *image, const CRect &rect, int count)
{
	byte	cvt[256];

	count++;
	int		level = 255 * (16 - count);
	for (int i=0; i<256; i++) {
		cvt[i] = (byte)((i * count + level) / 16);
	}
	FadeCvt(image, rect, cvt);
}

//
// “图像->白”处理
//
void CDrawImage::FadeToWhite(CImage *image, const CRect &rect, int count)
{
	byte	cvt[256];

	count = 15 - count;
	int		level = 255 * (16 - count);
	for (int i=0; i<256; i++) {
		cvt[i] = (byte)((i * count + level) / 16);
	}
	FadeCvt(image, rect, cvt);
}
