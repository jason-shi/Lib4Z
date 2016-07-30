#ifndef __SyncObj_h__
#define __SyncObj_h__

#include <windows.h>

class CCriticalSection {
  public:
	CRITICAL_SECTION	m_csAccess;

  public:
	CCriticalSection();
	virtual ~CCriticalSection();

  public:
	void			 Lock();
	void			 Unlock();
};

typedef CCriticalSection	CIntLock;

#endif
