#include "StdAfx.h"
#include "DhcpCode.h"
#include "DhcpInst.h"
#include "DhcpProc.h"

CDhcpProc::CDhcpProc(dword dwProcID, byte byPriority)
				: JProcess(dwProcID, byPriority)
{
	m_pDhcpInst = NULL;
}

CDhcpProc::~CDhcpProc()
{
	m_pDhcpInst = NULL;
}

void CDhcpProc::ProcEntry(byte byState, dword dwEvent, byte *pBuff, int nLength)
{
	switch (byState)
	{
	case DHCP_INIT_STATE:
		switch (dwEvent)
		{
		case EV_CREATE:
			OnCreate();
			break;
		}
		break;
	case DHCP_WORK_STATE:
		switch (dwEvent)
		{
		case EV_DESTROY:
			OnDestroy();
			break;
		case EV_SET_JDLINK:
			OnSetJDLink(pBuff, nLength);
			break;
		case EV_DLINK_DATA:
			OnRecvDLinkData(pBuff, nLength);
			break;
		case EV_APPLY_RENT:
			m_pDhcpInst->ApplyRent(L"DhcpcConfig.txt");
			break;
		}
		break;
	}
}

void CDhcpProc::OnCreate()
{
	m_pDhcpInst = new CDhcpInst(this);
	if (!m_pDhcpInst)
		return;

	NextState(DHCP_WORK_STATE);
}

void CDhcpProc::OnDestroy()
{
	SAFE_DELETE(m_pDhcpInst);
	return JProcess::OnDestroy();
}

void CDhcpProc::OnSetJDLink(byte *pPacket, int nLength)
{
	if (nLength != 4)
		return;

	m_dwJDLink = (int)pPacket;
}

void CDhcpProc::OnRecvDLinkData(byte *pPacket, int nLength)
{
	TDhcpMsg tDhcpMsg;

	//消息解码
	if (DhcpDecode(&tDhcpMsg, pPacket, nLength) == -1)
		return;

	//消息校验
	if (tDhcpMsg.byOpCode != DHCP_OPCODE_UP)
		return;

	//消息处理
	switch (tDhcpMsg.tOption.byMsgType)
	{
	case ID_DHCP_OFFER:
		m_pDhcpInst->OnRecvOffer(&tDhcpMsg);
		break;
	case ID_DHCP_ACK:
		m_pDhcpInst->OnRecvAck(&tDhcpMsg);
		break;
	case ID_DHCP_NACK:
		m_pDhcpInst->OnRecvNack(&tDhcpMsg);
		break;
	case ID_DHCP_RELEASE:
		m_pDhcpInst->OnRecvRelease(&tDhcpMsg);
		break;
	}
}
