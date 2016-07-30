#ifndef __syncobj_h__
#define __syncobj_h__

#include <windows.h>

class CCriticalSection 
{
  public:
	CRITICAL_SECTION	m_csAccess;

  public:
	CCriticalSection();
	virtual ~CCriticalSection();

	void Lock();
	void Unlock();
};

typedef CCriticalSection	CIntLock;	// InterThread Lock

#endif
