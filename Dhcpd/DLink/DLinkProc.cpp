#include "StdAfx.h"
#include "DLinkCode.h"
#include "DLinkInst.h"
#include "DLinkProc.h"

CDLinkProc::CDLinkProc(dword dwProcID, byte byPriority)
				: JProcess(dwProcID, byPriority)
{
}

CDLinkProc::~CDLinkProc()
{
}

void CDLinkProc::ProcEntry(byte byState, dword dwEvent, byte *pBuff, int nLength)
{
	switch (byState)
	{
	case DLINK_INIT_STATE:
		switch (dwEvent)
		{
		case EV_CREATE:
			OnCreate();
			break;
		}
		break;
	case DLINK_WORK_STATE:
		switch (dwEvent)
		{
		case EV_DESTROY:
			OnDestroy();
			break;
		case EV_SET_JDHCP:
			OnSetJDhcp(pBuff, nLength);
			break;
		case EV_ADD_JDEST:
			OnAddJDest(pBuff, nLength);
			break;
		case EV_DEL_JDEST:
			OnDelJDest(pBuff, nLength);
			break;
		case EV_NET_PACKET:
			OnRecvNetPacket(pBuff, nLength);
			break;
		case EV_DLINK_DATA:
			OnRecvDLinkData(pBuff, nLength);
			break;
		}
	}
}

void CDLinkProc::OnCreate()
{
	m_pDLinkInst = new CDLinkInst(this);
	if (!m_pDLinkInst)
		return;

	NextState(DLINK_WORK_STATE);
}

void CDLinkProc::OnDestroy()
{
	SAFE_DELETE(m_pDLinkInst);
	return JProcess::OnDestroy();
}

void CDLinkProc::OnSetJDhcp(byte *pPacket, int nLength)
{
	if (nLength != 4)
		return;

	m_dwJDhcp = (int)pPacket;
}

void CDLinkProc::OnAddJDest(byte *pPacket, int nLength)
{
	if (nLength != 4)
		return;

	m_vecJDest.push_back((int)pPacket);
}

void CDLinkProc::OnDelJDest(byte *pPacket, int nLength)
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

void CDLinkProc::OnRecvNetPacket(byte *pPacket, int nLength)
{
	TDLinkMsg tDLinkMsg;

	if (DLinkDecode(&tDLinkMsg, pPacket, nLength) != nLength)
		return;

	m_pDLinkInst->OnRecvNetPacket(&tDLinkMsg);
}

void CDLinkProc::OnRecvDLinkData(byte *pPacket, int nLength)
{
	if (nLength != sizeof(TDLinkData))
		return;

	m_pDLinkInst->OnRecvDLinkData((TDLinkData *)pPacket);
}
