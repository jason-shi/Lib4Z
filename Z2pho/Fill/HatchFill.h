#ifndef __HatchFill_h__
#define __HatchFill_h__

#include "Fill.h"

#define HFS_HORIZONTAL			0    /* ----- */
#define HFS_VERTICAL			1    /* ||||| */
#define HFS_FDIAGONAL			2    /* \\\\\ */
#define HFS_BDIAGONAL			3    /* ///// */
#define HFS_CROSS				4    /* +++++ */
#define HFS_DIAGCROSS			5    /* xxxxx */

class CHatchFill : public CFill {
  public:
	CHatchFill();
	~CHatchFill();

  public:
	void			 SetBitmap(__int64 nHatchBitmap);
	void			 SetStyle(int nHatchStyle);
	void			 SetOrigin(int xOrigin, int yOrigin);

  public:
	void			 InitFill();
	void			 FillLine(int x1, int x2, int y);

  protected:
	__int64			 m_nHatchBitmap;
	CPoint			 m_xOrigin;
	CImage			*m_pHatch;
	CSprite			*m_pSprite;
};

inline void CHatchFill::SetBitmap(__int64 nHatchBitmap)
{
	m_nHatchBitmap = nHatchBitmap;
}

inline void CHatchFill::SetOrigin(int xOrigin, int yOrigin)
{
	m_xOrigin.x = xOrigin;
	m_xOrigin.y = yOrigin;
}

#endif
