#include "StdAfx.h"
#include "Dhcpd.h"
#include "DhcpServer.h"

using namespace std;

CDhcpd::CDhcpd()
{
}

CDhcpd::~CDhcpd()
{
}

IDhcpServer *CDhcpd::CreateServer()
{
	return new CDhcpServer();
}

void CDhcpd::DeleteServer(IDhcpServer *pDhcpServer)
{
	SAFE_DELETE(pDhcpServer);
}
