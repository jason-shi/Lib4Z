#include "StdAfx.h"
#include <boost\bind.hpp>
#include "DhcpInst.h"
#include "DhcpCode.h"
#include "DhcpProc.h"

#include "IPPool\IP4Pool.h"
#include "IPSet\IP4Set.h"

CDhcpInst::CDhcpInst(CDhcpProc *pDhcpProc)
{
	m_pJSelf = pDhcpProc;
}

CDhcpInst::~CDhcpInst()
{
	m_pJSelf = NULL;
}

void CDhcpInst::OnRecvDiscover(TDhcpMsg *pDhcpMsg)
{
	if (m_byState != ST_DHCP_INITIAL)
		return;

	CIP4Set *pIP4Set = m_pJSelf->m_pIP4Pool->Alloc(pDhcpMsg->byHWAddr + 10); //Ingore 10 bytes
	if (pIP4Set == NULL)
	{
		SendNack();
		return;
	}

	SendOffer();
}

void CDhcpInst::OnRecvRequest(TDhcpMsg *pDhcpMsg)
{
	if (m_byState != ST_DHCP_REQUEST)
		return;

	CIP4Set *pIP4Set = m_pJSelf->m_pIP4Pool->Alloc(pDhcpMsg->byHWAddr + 10, pDhcpMsg->dwClientIP); //Ingore 10 bytes
	if (pIP4Set == NULL)
	{
		SendNack();
		return;
	}

	SendAck();
}

void CDhcpInst::OnRecvDecline(TDhcpMsg *pDhcpMsg)
{
	if (m_byState != ST_DHCP_REQUEST)
		return;

	m_pJSelf->m_pIP4Pool->Release(pDhcpMsg->byHWAddr + 10); //Ingore 10 bytes
}

void CDhcpInst::OnRecvRelease(TDhcpMsg *pDhcpMsg)
{
	if (m_byState != ST_DHCP_BOUND)
		return;

	m_pJSelf->m_pIP4Pool->Release(pDhcpMsg->byHWAddr + 10); //Ingore 10 bytes
}

void CDhcpInst::OnRecvInform(TDhcpMsg *pDhcpMsg)
{
	SendAck();
}

void CDhcpInst::SendOffer()
{
	TDhcpMsg tDhcpMsg;
	tDhcpMsg.byOpCode = DHCP_OPCODE_DOWN;
	tDhcpMsg.byHWType = DHCP_HWTYPE_ETH;
	tDhcpMsg.byHWLen = 6;
	tDhcpMsg.byHops = DLINK_DEFAULT_HOPS;
	tDhcpMsg.wSeconds = 0;
	tDhcpMsg.wFlags = 0;
	tDhcpMsg.dwClientIP = 0;
	tDhcpMsg.dwUserIP = 0;
	tDhcpMsg.dwServerIP = 0;
	tDhcpMsg.dwAgentIP = 0;
	memcpy(tDhcpMsg.byHWAddr + 10, m_byMAC, 6);
	strcpy(tDhcpMsg.szHostName, m_szHostName);
	strcpy(tDhcpMsg.szBootFile, m_szBootFile);

	//ÌîÐ´Option
	TOption &tOption = tDhcpMsg.tOption;
	tOption.dwSubnetMask = m_dwSubnetMask;
	tOption.vecRouter = m_vecRouter;
	tOption.vecDNSServer = m_vecDNSServer;
	tOption.vecLogServer = m_vecLogServer;
	tOption.wInterfMTU = m_wInterfMTU;
	tOption.vecStaticRoute = m_vecStaticRoute;
	tOption.dwARPCacheTime = m_dwARPCacheTime;
	tOption.vecNTPServer = m_vecNTPServer;
	tOption.dwIPLeaseTime = m_dwIPLeaseTime;
	tOption.byMsgType = ID_DHCP_OFFER;
	tOption.dwServerID = m_dwServerID;
	SendDLinkData(&tDhcpMsg);
}

void CDhcpInst::SendAck()
{
	TDhcpMsg tDhcpMsg;
	tDhcpMsg.byOpCode = DHCP_OPCODE_DOWN;
	tDhcpMsg.byHWType = DHCP_HWTYPE_ETH;
	tDhcpMsg.byHWLen = 6;
	tDhcpMsg.byHops = DLINK_DEFAULT_HOPS;
	tDhcpMsg.wSeconds = 0;
	tDhcpMsg.wFlags = 0;
	tDhcpMsg.dwClientIP = 0;
	tDhcpMsg.dwUserIP = 0;
	tDhcpMsg.dwServerIP = 0;
	tDhcpMsg.dwAgentIP = 0;
	memcpy(tDhcpMsg.byHWAddr + 10, m_byMAC, 6);
	strcpy(tDhcpMsg.szHostName, m_szHostName);
	strcpy(tDhcpMsg.szBootFile, m_szBootFile);

	//ÌîÐ´Option
	TOption &tOption = tDhcpMsg.tOption;
	tOption.dwSubnetMask = m_dwSubnetMask;
	tOption.vecRouter = m_vecRouter;
	tOption.vecDNSServer = m_vecDNSServer;
	tOption.vecLogServer = m_vecLogServer;
	tOption.wInterfMTU = m_wInterfMTU;
	tOption.vecStaticRoute = m_vecStaticRoute;
	tOption.dwARPCacheTime = m_dwARPCacheTime;
	tOption.vecNTPServer = m_vecNTPServer;
	tOption.dwIPLeaseTime = m_dwIPLeaseTime;
	tOption.byMsgType = ID_DHCP_ACK;
	tOption.dwServerID = m_dwServerID;
	SendDLinkData(&tDhcpMsg);

	g_pZTimer->AddTimer(TM_RENT_RELEASE, m_dwIPLeaseTime * 1000, boost::bind(&CDhcpInst::OnTimerOut, this, _1), (void *)TM_RENT_RELEASE);
}

void CDhcpInst::SendNack()
{
	TDhcpMsg tDhcpMsg;
	tDhcpMsg.byOpCode = DHCP_OPCODE_DOWN;
	tDhcpMsg.byHWType = DHCP_HWTYPE_ETH;
	tDhcpMsg.byHWLen = 6;
	tDhcpMsg.byHops = DLINK_DEFAULT_HOPS;
	tDhcpMsg.wSeconds = 0;
	tDhcpMsg.wFlags = 0;
	tDhcpMsg.dwClientIP = 0;
	tDhcpMsg.dwUserIP = 0;
	tDhcpMsg.dwServerIP = 0;
	tDhcpMsg.dwAgentIP = 0;
	memcpy(tDhcpMsg.byHWAddr + 10, m_byMAC, 6);
	tDhcpMsg.szHostName[0] = '\0';
	tDhcpMsg.szBootFile[0] = '\0';
	tDhcpMsg.tOption.byMsgType = ID_DHCP_NACK;
	SendDLinkData(&tDhcpMsg);
}

void CDhcpInst::SendRelease()
{
	TDhcpMsg tDhcpMsg;
	tDhcpMsg.byOpCode = DHCP_OPCODE_DOWN;
	tDhcpMsg.byHWType = DHCP_HWTYPE_ETH;
	tDhcpMsg.byHWLen = 6;
	tDhcpMsg.byHops = DLINK_DEFAULT_HOPS;
	tDhcpMsg.wSeconds = 0;
	tDhcpMsg.wFlags = 0;
	tDhcpMsg.dwClientIP = 0;
	tDhcpMsg.dwUserIP = 0;
	tDhcpMsg.dwServerIP = 0;
	tDhcpMsg.dwAgentIP = 0;
	memcpy(tDhcpMsg.byHWAddr + 10, m_byMAC, 6);
	tDhcpMsg.szHostName[0] = '\0';
	tDhcpMsg.szBootFile[0] = '\0';
	tDhcpMsg.tOption.byMsgType = ID_DHCP_RELEASE;
	SendDLinkData(&tDhcpMsg);
}

void CDhcpInst::SendDLinkData(TDhcpMsg *pDhcpMsg)
{
	byte byDhcpBuff[MAX_DLINK_DATA_LEN];

	int nDataLen = DhcpdEncode(byDhcpBuff, MAX_DLINK_DATA_LEN, pDhcpMsg);
	if (nDataLen == -1)
		return;

	m_pJSelf->PostMessage(m_pJSelf->m_dwJDLink, EV_DLINK_DATA, byDhcpBuff, nDataLen);
}

void CDhcpInst::OnTimerOut(int nTimerID)
{
	switch (nTimerID)
	{
	case TM_RENT_RELEASE:
		SendRelease();
		break;
	}
}
