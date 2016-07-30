#include "StdAfx.h"
#include "DhcpClient.h"
#include "DLinkProc.h"
#include "DhcpProc.h"

using namespace std;

CDhcpClient::CDhcpClient()
{
	m_dwJDLink = 0;
	m_dwJDhcp  = 0;
}

CDhcpClient::~CDhcpClient()
{
	PostMessageWait(m_dwJDLink, EV_DESTROY);
	WaitForJProcess(m_dwJDLink);

	PostMessageWait(m_dwJDhcp,  EV_DESTROY);
	WaitForJProcess(m_dwJDhcp);
}

bool CDhcpClient::Initial(dword dwClientID, dword dwProcBase)
{
	m_dwClientID = dwClientID;
	m_dwProcBase = dwProcBase;

	m_dwJDLink = m_dwProcBase + m_dwClientID * 2 + PID_DLINK;
	m_dwJDhcp  = m_dwProcBase + m_dwClientID * 2 + PID_DHCP;

	if (!CreateJProcess(&CDLinkProcCreator(), m_dwJDLink, 10, 2048, 32, 2048, 4))
		return false;
	if (!CreateJProcess(&CDhcpProcCreator(),  m_dwJDhcp,  10, 2048, 32, 2048, 4))
		return false;

	PostMessageWait(m_dwJDLink, EV_SET_JDHCP);
	PostMessageWait(m_dwJDhcp,  EV_SET_JDLINK);
	return true;
}

void CDhcpClient::Run()
{
	PostMessageWait(m_dwJDhcp, EV_APPLY_RENT);
}

dword CDhcpClient::GetJDLink() const
{
	return m_dwJDLink;
}

void CDhcpClient::AddJDest(dword dwJDest)
{
	PostMessageWait(m_dwJDhcp, EV_ADD_JDEST);
}

void CDhcpClient::DelJDest(dword dwJDest)
{
	PostMessageWait(m_dwJDhcp, EV_DEL_JDEST);
}
