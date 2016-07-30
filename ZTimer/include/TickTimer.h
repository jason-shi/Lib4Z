#ifndef __TickTimer_h__
#define __TickTimer_h__

#include <boost/function.hpp>

typedef boost::function<void(int nType, void *lpParam)>		FN_TimerProc;

class CTickTimer {
  public:
	CTickTimer(int nType, int nPeriod, FN_TimerProc TimerProc, void *lpParam, bool bToRemove);
	~CTickTimer();

  public:
	bool			 IsToRemove() { return m_bToRemove; }

  public:
	FN_TimerProc	 OnTimerProc;
	int				 m_nType;
	int				 m_nElapse;
	int				 m_nPeriod;
	void			*m_lpParam;
	bool			 m_bPause;
	bool			 m_bToRemove;
};

#endif
