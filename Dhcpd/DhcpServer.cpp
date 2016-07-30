#include "StdAfx.h"
#include "DhcpServer.h"
#include "DLinkProc.h"
#include "DhcpProc.h"

using namespace std;

CDhcpServer::CDhcpServer()
{
	m_dwJDLink = 0;
	m_dwJDhcp  = 0;
}

CDhcpServer::~CDhcpServer()
{
	PostMessageWait(m_dwJDLink, EV_DESTROY);
	WaitForJProcess(m_dwJDLink);

	PostMessageWait(m_dwJDhcp,  EV_DESTROY);
	WaitForJProcess(m_dwJDhcp);
}

bool CDhcpServer::Initial(dword dwServerID, dword dwProcBase)
{
	m_dwServerID = dwServerID;
	m_dwProcBase = dwProcBase;

	m_dwJDLink = m_dwProcBase + m_dwServerID * 2 + PID_DLINK;
	m_dwJDhcp  = m_dwProcBase + m_dwServerID * 2 + PID_DHCP;

	if (!CreateJProcess(&CDLinkProcCreator(), m_dwJDLink, 10, 2048, 32, 2048, 4))
		return false;
	if (!CreateJProcess(&CDhcpProcCreator(),  m_dwJDhcp,  10, 2048, 32, 2048, 4))
		return false;

	PostMessageWait(m_dwJDLink, EV_SET_JDHCP);
	PostMessageWait(m_dwJDhcp,  EV_SET_JDLINK);
	return true;
}

void CDhcpServer::Run()
{
}

dword CDhcpServer::GetJDLink() const
{
	return m_dwJDLink;
}

void CDhcpServer::AddJDest(dword dwJDest)
{
	PostMessageWait(m_dwJDhcp, EV_ADD_JDEST);
}

void CDhcpServer::DelJDest(dword dwJDest)
{
	PostMessageWait(m_dwJDhcp, EV_DEL_JDEST);
}
