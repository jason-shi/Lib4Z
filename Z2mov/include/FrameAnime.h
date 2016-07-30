#ifndef __FrameAnime_h__
#define __FrameAnime_h__

#include "Anime.h"

class CImage;
class CFrameAnime : public CAnime {
  public:
	CFrameAnime();
	CFrameAnime(CImage *pImage, CRect xSrcRect, int nFrame, int nPeriod, FN_AnimeEnd AnimeEnd = NULL, int *pParam = NULL);
	virtual ~CFrameAnime();

  public:
	void			 SetImage(CImage *pImage);
	void			 SetSrcRect(CRect xSrcRect);

  protected:
	void			 SetUp();
	void			 Update(int nDelta);

  private:
	CRect			 m_xSrcRect; //”≥…‰‘¥
	CPoint			 m_xCurPos;
};

inline void CFrameAnime::SetSrcRect(CRect xSrcRect)
{
	if (IsActive())
		return;
	m_xSrcRect = xSrcRect;
}

#endif
