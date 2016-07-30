#include "StdAfx.h"
#include "DLink.h"

using namespace std;

CDLink::CDLink(dword dwProcID, byte byPriority)
				: JProcess(dwProcID, byPriority)
{
}

CDLink::~CDLink()
{
}

void CDLink::DefProcEntry(byte byState, dword dwEvent, byte *pBuff, int nLength)
{
	switch (dwEvent)
	{
	case EV_ADD_JDEST:
		OnAddJDest(pBuff, nLength);
		break;
	case EV_DEL_JDEST:
		OnDelJDest(pBuff, nLength);
		break;
	}
}

void CDLink::OnAddJDest(byte *pPacket, int nLength)
{
	if (nLength != 4)
		return;

	m_vecJDest.push_back((int)pPacket);
}

void CDLink::OnDelJDest(byte *pPacket, int nLength)
{
	if (nLength != 4)
		return;

	VECDWORD::iterator iter;
	for (iter = m_vecJDest.begin(); iter != m_vecJDest.end(); iter++)
	{
		if (*iter == (int)pPacket)
		{
			m_vecJDest.erase(iter);
			break;
		}
	}
}
