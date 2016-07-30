#ifndef __FlickAnime_h__
#define __FlickAnime_h__

#include "Anime.h"

class CFlickAnime : public CAnime {
  public:
	CFlickAnime();
	CFlickAnime(int nFrame, int nShowPrd, int nHidePrd, bool bPriHide = true);
	virtual ~CFlickAnime();

  public:
	void			 SetShowPrd(int nShowPrd);
	void			 SetHidePrd(int nHidePrd);
	void			 SetPriHide(bool bPriHide = true);

  protected:
	void			 SetUp();
	void			 Update(int nDelta);

  private:
	int				 m_nShowPrd;
	int				 m_nHidePrd;
	bool			 m_bPriHide;
};

inline void CFlickAnime::SetShowPrd(int nShowPrd)
{
	if (IsActive())
		return;
	m_nShowPrd = nShowPrd;
}

inline void CFlickAnime::SetHidePrd(int nHidePrd)
{
	if (IsActive())
		return;
	m_nHidePrd = nHidePrd;
}

inline void CFlickAnime::SetPriHide(bool bPriHide)
{
	if (IsActive())
		return;
	m_bPriHide = bPriHide;
}

#endif
