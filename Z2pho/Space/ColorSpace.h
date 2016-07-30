#ifndef __ColorSpace_h__
#define __ColorSpace_h__

#define COLORSPACE_BEGIN \
	namespace CColorSpace \
	{

#define COLORSPACE_END \
	}


COLORSPACE_BEGIN

class HSB {
  public:
	static HSB		 FromBGRA(COLORREF color);
	COLORREF		 ToBGRA();

  public:
	float			 H;
	float			 S;
	float			 B;
};

COLORSPACE_END

#endif
