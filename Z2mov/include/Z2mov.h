#ifndef __Z2mov_h__
#define __Z2mov_h__

#include <Anime.h>
#include <FrameAnime.h>
#include <MoveAnime.h>
#include <ZoomAnime.h>
#include <RotateAnime.h>
#include <FlickAnime.h>

#define WM_ANIME_UPDATE		(WM_USER + 2)
#define WM_ANIME_END		(WM_USER + 3)

class CZ2mov {
  public:
	SINGLETON(CZ2mov);
	~CZ2mov();

  protected:
	CZ2mov();

  public:
	bool			 Initial();
	void			 Release();
	void			 SetWindow(CWindow *_window);
	CWindow			*GetWindow() const;
	void			 AddAnime(CAnime *pAnime);
	void			 DelAnime(CAnime *pAnime);
	void			 Update(int nDelta);

  private:
	CWindow			*window;
	CRect			 ClientRect;

  private:
	CAnime			*m_pHead;
};

inline void CZ2mov::SetWindow(CWindow *_window)
{
	window = _window;
	window->GetClientRect(&ClientRect);
}

inline CWindow *CZ2mov::GetWindow() const
{
	return window;
}

#define g_pZ2mov	 CZ2mov::GetInstancePtr()

#ifdef	_MSC_VER
	#ifdef	_DEBUG
		#pragma comment(lib, "Z2movD.lib")
	#else
		#pragma comment(lib, "Z2mov.lib")
	#endif
#endif

#endif
