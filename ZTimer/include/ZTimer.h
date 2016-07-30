#ifndef __ZTimer_h__
#define __ZTimer_h__

#include "TickTimer.h"

#ifndef  SINGLETON
#define  SINGLETON(type) \
	static inline type* GetInstancePtr() \
	{ \
		static type _instance; \
		return &_instance; \
	}
#endif

#define WM_TIMER_UPDATE		(WM_USER + 4)

class CZTimer {
	typedef std::list<CTickTimer>	 TimerList;
  public:
	SINGLETON(CZTimer);
	~CZTimer();

  private:
	CZTimer();

  public:
	bool			 Initial();
	void			 Release();
	void			 SetWindow(CWindow *_window);
	CWindow			*GetWindow() const;
	void			 SendMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0);

  public:
	void			 AddTimer(int type, int timeout, FN_TimerProc TimerProc, void *lpParam = NULL);
	void			 DelTimer(int type);
	void			 ClearTimer();
	void			 SetPause(bool bPause);
	void			 Update(int nDelta);

  protected:
	CIntLock		 mutex;
	TimerList		 m_lstTickTimer;
	TimerList		 m_lstHandle;

  protected:
	CWindow			*window;
	CRect			 ClientRect;
};

inline void CZTimer::SetWindow(CWindow *_window)
{
	window = _window;
	window->GetClientRect(&ClientRect);
}

inline CWindow *CZTimer::GetWindow() const
{
	return window;
}

inline void CZTimer::SendMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	window->SendMessage(uMsg, wParam, lParam);
}

#define g_pZTimer			CZTimer::GetInstancePtr()

#endif
