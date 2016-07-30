#include "StdAfx.h"
#include <boost\bind.hpp>
#include "DhcpInst.h"
#include "DhcpCode.h"
#include "DhcpProc.h"
#include "DhcpConfig.h"

CDhcpInst::CDhcpInst(CDhcpProc *pDhcpProc)
{
	m_pJSelf = pDhcpProc;
}

CDhcpInst::~CDhcpInst()
{
	m_pJSelf = NULL;
}

void CDhcpInst::OnRecvOffer(TDhcpMsg *pDhcpMsg)
{
	if (!NextStateOnRecv(ID_DHCP_OFFER))
		return;

	strcpy(m_szHostName, pDhcpMsg->szHostName);
	strcpy(m_szBootFile, pDhcpMsg->szBootFile);

	SendRequest();
}

void CDhcpInst::OnRecvAck(TDhcpMsg *pDhcpMsg)
{
	if (!NextStateOnRecv(ID_DHCP_ACK))
		return;

	m_wSeconds = pDhcpMsg->wSeconds;
	m_dwClientIP = pDhcpMsg->dwClientIP;
	m_dwUserIP = pDhcpMsg->dwUserIP;
	m_dwServerIP = pDhcpMsg->dwServerIP;
	m_dwAgentIP = pDhcpMsg->dwAgentIP;
	strcpy(m_szHostName, pDhcpMsg->szHostName);
	strcpy(m_szBootFile, pDhcpMsg->szBootFile);

	TOption &tOption = pDhcpMsg->tOption;
	m_dwSubnetMask = tOption.dwSubnetMask;
	m_vecRouter = tOption.vecRouter;
	m_vecDNSServer = tOption.vecDNSServer;
	m_vecLogServer = tOption.vecLogServer;
	m_wInterfMTU = tOption.wInterfMTU;
	m_vecStaticRoute = tOption.vecStaticRoute;
	m_dwARPCacheTime = tOption.dwARPCacheTime;
	m_vecNTPServer = tOption.vecNTPServer;
	m_dwIPLeaseTime = tOption.dwIPLeaseTime;
	m_byMsgType = tOption.byMsgType;
	m_dwServerID = tOption.dwServerID;

	g_pZTimer->AddTimer(TM_RENT_UPDATE, m_dwIPLeaseTime * 1000, boost::bind(&CDhcpInst::OnTimerOut, this, _1), (void *)TM_RENT_UPDATE);
}

void CDhcpInst::OnRecvNack(TDhcpMsg *pDhcpMsg)
{
	if (!NextStateOnRecv(ID_DHCP_NACK))
		return;
}

void CDhcpInst::OnRecvRelease(TDhcpMsg *pDhcpMsg)
{
	if (!NextStateOnRecv(ID_DHCP_RELEASE))
		return;
}

void CDhcpInst::SendDiscover()
{
	TDhcpMsg tDhcpMsg;
	tDhcpMsg.byOpCode = DHCP_OPCODE_UP;
	tDhcpMsg.byHWType = DHCP_HWTYPE_ETH;
	tDhcpMsg.byHWLen = 6;
	tDhcpMsg.byHops = DLINK_DEFAULT_HOPS;
	tDhcpMsg.wSeconds = 0;
	tDhcpMsg.wFlags = DLINK_BROADCAST;
	tDhcpMsg.dwClientIP = 0;
	tDhcpMsg.dwUserIP = 0;
	tDhcpMsg.dwServerIP = 0;
	tDhcpMsg.dwAgentIP = 0;
	tDhcpMsg.tOption.byMsgType = ID_DHCP_DISCOVER;
	SendDLinkData(&tDhcpMsg);

	NextStateOnSend(ID_DHCP_DISCOVER);
}

void CDhcpInst::SendRequest()
{
	TDhcpMsg tDhcpMsg;
	tDhcpMsg.byOpCode = DHCP_OPCODE_UP;
	tDhcpMsg.byHWType = DHCP_HWTYPE_ETH;
	tDhcpMsg.byHWLen = 6;
	tDhcpMsg.byHops = DLINK_DEFAULT_HOPS;
	tDhcpMsg.wSeconds = 0;
	tDhcpMsg.wFlags = DLINK_BROADCAST;
	tDhcpMsg.dwClientIP = m_dwClientIP;
	tDhcpMsg.dwUserIP = m_dwUserIP;
	tDhcpMsg.dwServerIP = m_dwServerIP;
	tDhcpMsg.dwAgentIP = m_dwAgentIP;
	tDhcpMsg.tOption.byMsgType = ID_DHCP_REQUEST;
	SendDLinkData(&tDhcpMsg);

	NextStateOnSend(ID_DHCP_REQUEST);
}

void CDhcpInst::SendRelease()
{
	TDhcpMsg tDhcpMsg;
	tDhcpMsg.byOpCode = DHCP_OPCODE_UP;
	tDhcpMsg.byHWType = DHCP_HWTYPE_ETH;
	tDhcpMsg.byHWLen = 6;
	tDhcpMsg.byHops = DLINK_DEFAULT_HOPS;
	tDhcpMsg.wSeconds = 0;
	tDhcpMsg.wFlags = DLINK_UNICAST;
	tDhcpMsg.dwClientIP = m_dwClientIP;
	tDhcpMsg.dwUserIP = 0;
	tDhcpMsg.dwServerIP = 0;
	tDhcpMsg.dwAgentIP = 0;
	tDhcpMsg.tOption.byMsgType = ID_DHCP_RELEASE;
	SendDLinkData(&tDhcpMsg);

	NextStateOnSend(ID_DHCP_RELEASE);
}

bool CDhcpInst::NextStateOnRecv(byte byMsgID)
{
	switch (byMsgID)
	{
	case ID_DHCP_OFFER:
		if (m_byState != ST_DHCP_SELECT)
			return false;
		m_byState = ST_DHCP_SELECT;
		break;
	case ID_DHCP_ACK:
		if (m_byState != ST_DHCP_REQUEST
			&& m_byState != ST_DHCP_REBIND
			&& m_byState != ST_DHCP_RENEW)
			return false;
		m_byState = ST_DHCP_BOUND;
		break;
	case ID_DHCP_NACK:
		if (m_byState != ST_DHCP_REBIND
			&& m_byState != ST_DHCP_RENEW)
			return false;
		m_byState = ST_DHCP_INITIAL;
		break;
	case ID_DHCP_RELEASE:
		if (m_byState != ST_DHCP_BOUND)
			return false;
		m_byState = ST_DHCP_INITIAL;
		break;
	}
	return true;
}

void CDhcpInst::NextStateOnSend(byte byMsgID)
{
	switch (byMsgID)
	{
	case ID_DHCP_DISCOVER:
		m_byState = ST_DHCP_SELECT;
		break;
	case ID_DHCP_REQUEST:
		switch (m_byState)
		{
		case ST_DHCP_SELECT: m_byState = ST_DHCP_REQUEST; break;
		case ST_DHCP_BOUND:  m_byState = ST_DHCP_RENEW;   break;
		case ST_DHCP_RENEW:  m_byState = ST_DHCP_REBIND;  break;
		}
		break;
	case ID_DHCP_RELEASE:
		m_byState = ST_DHCP_INITIAL;
		break;
	}
}

void CDhcpInst::SendDLinkData(TDhcpMsg *pDhcpMsg)
{
	byte byDhcpBuff[MAX_DLINK_DATA_LEN];

	int nDataLen = DhcpEncode(byDhcpBuff, MAX_DLINK_DATA_LEN, pDhcpMsg);
	if (nDataLen == -1)
		return;

	m_pJSelf->PostMessage(m_pJSelf->m_dwJDLink, EV_DLINK_DATA, byDhcpBuff, nDataLen);
}

void CDhcpInst::OnTimerOut(int nTimerID)
{
	switch (nTimerID)
	{
	case TM_RENT_UPDATE:
		SendRequest();
		break;
	case TM_RENT_REBIND:
		SendDiscover();
		break;
	case TM_RENT_RELEASE:
		SendRelease();
		break;
	case TM_RENT_APPLY:
		OnApplyTout();
		break;
	}
}

void CDhcpInst::OnApplyTout()
{
}

void CDhcpInst::ApplyRent(wstring szConfigFile)
{
	CDhcpConfig tDhcpConfig;
	if (!tDhcpConfig.LoadConfig(szConfigFile))
	{
		SendDiscover();
		return;
	}

	m_dwRentMoment   = tDhcpConfig.m_dwRentMoment;
	memcpy(m_byMAC, tDhcpConfig.m_byMAC, 6);
	m_dwClientIP     = tDhcpConfig.m_dwClientIP;
	m_dwSubnetMask   = tDhcpConfig.m_dwSubnetMask;
	m_dwAgentIP      = tDhcpConfig.m_dwAgentIP;
	m_vecRouter      = tDhcpConfig.m_vecRouter;
	m_vecDNSServer   = tDhcpConfig.m_vecDNSServer;
	m_vecLogServer   = tDhcpConfig.m_vecLogServer;
	m_wInterfMTU     = tDhcpConfig.m_wInterfMTU;
	m_vecStaticRoute = tDhcpConfig.m_vecStaticRoute;
	m_dwARPCacheTime = tDhcpConfig.m_dwARPCacheTime;
	m_vecNTPServer   = tDhcpConfig.m_vecNTPServer;
	m_dwIPLeaseTime  = tDhcpConfig.m_dwIPLeaseTime;

	time_t dwCurrMoment;

	time(&dwCurrMoment);
	if (dwCurrMoment < m_dwRentMoment) //Ê±¼ä´íÎó
		SendDiscover();
	else
	{
		if (dwCurrMoment - m_dwRentMoment > m_dwIPLeaseTime * 3 / 4)
			SendRequest();
	}

	if (dwCurrMoment > m_dwRentMoment //unsigned int
		&& dwCurrMoment - m_dwRentMoment > m_dwIPLeaseTime)
	{
		SendDiscover();
	}
	else
	{
		SendRequest();
	}
}
