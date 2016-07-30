#ifndef	__Dib_h__
#define	__Dib_h__

#include "Misc.h"

class CFile;
class CDib {
  public:
	CDib() : W(0), H(0), D(0), hGlobal(0), Bits(0), Info(0) {}
	CDib(int width, int height, int depth);
	virtual ~CDib();

	BOOL			 Create(int width, int height, int depth);
	void			 Release();

	const BITMAPINFO	*GetInfo() const { return Info; }

	const void		*GetBits() const { return Bits; }
	void			*GetBits() { return Bits; }

	const void		*GetBits(int x, int y) const;
	void			*GetBits(int x, int y);

	BOOL			 IsOK() const { return Bits != 0; }
	int				 Width() const { return W; }
	int				 Height() const { return H; }
	int				 Depth() const { return D; }
	unsigned		 BytesPerLine() const { return bytes_per_line; }
	unsigned		 BytesPerPixel() const { return bytes_per_pixel; }
	CSize			 Size() const { return CSize(W, H); }

	void			 Clear();

	static unsigned	 ScanBytes(int pixWidth, int pixDepth);
	static unsigned	 PixelBytes(int pixDepth);

  protected:
	BOOL			 LoadBMP(const TCHAR* filename);
	BOOL			 LoadPNG(const TCHAR* filename);

  protected:
	BOOL			 ReadBMP(FILE *fp);
	BOOL			 ReadPNG(FILE *fp);

  protected:
	HGLOBAL			 hGlobal;
	void			*Bits;
	BITMAPINFO		*Info;
	unsigned		 bytes_per_line;
	unsigned		 bytes_per_pixel;
	int				 W;
	int				 H;
	int				 D;
};

inline unsigned CDib::ScanBytes(int pixWidth, int pixDepth)
{
	return (unsigned)(((long)pixWidth * pixDepth + 31) >> 5) << 2;
}

inline unsigned CDib::PixelBytes(int pixDepth)
{
	return (unsigned)((pixDepth + 7) >> 3);
}

inline const void *CDib::GetBits(int x, int y) const
{
	return (const void *)((const char *)GetBits() + (H - y - 1) * bytes_per_line + x * bytes_per_pixel);
}

inline void *CDib::GetBits(int x, int y)
{
	return (void *)((char *)GetBits() + (H - y - 1) * bytes_per_line + x * bytes_per_pixel);
}

#endif
