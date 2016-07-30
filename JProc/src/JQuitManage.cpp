#include "StdAfx.h"
#include "JQuitManage.h"

JQuitManage::JQuitManage()
{
}

JQuitManage::~JQuitManage()
{
}

void JQuitManage::AddJQuit(dword dwProcID)
{
	m_mutex.Lock();

	m_vecJQuit.push_back(dwProcID);
	m_mutex.Unlock();
}

void JQuitManage::DelJQuit(dword dwProcID)
{
	m_mutex.Lock();

	VECJQUIT::iterator iter;
	for (iter = m_vecJQuit.begin(); iter != m_vecJQuit.end(); iter++)
	{
		if (*iter == dwProcID)
		{
			m_vecJQuit.erase(iter);
			break;
		}
	}
	m_mutex.Unlock();
}

bool JQuitManage::CheckJQuit(dword dwProcID)
{
	m_mutex.Lock();

	VECJQUIT::iterator iter;
	for (iter = m_vecJQuit.begin(); iter != m_vecJQuit.end(); iter++)
	{
		if (*iter == dwProcID)
		{
			m_mutex.Unlock();
			return false;
		}
	}
	m_mutex.Unlock();
	return true;
}
