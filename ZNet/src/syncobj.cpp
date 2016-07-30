#include "syncobj.h"

CCriticalSection::CCriticalSection()
{
	InitializeCriticalSection( &m_csAccess );
}

CCriticalSection::~CCriticalSection()
{
	DeleteCriticalSection( &m_csAccess );
}

void CCriticalSection::Lock()
{
	EnterCriticalSection( &m_csAccess );
}

void CCriticalSection::Unlock()
{
	LeaveCriticalSection( &m_csAccess );
}
