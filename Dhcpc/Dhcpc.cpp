#include "StdAfx.h"
#include "Dhcpc.h"
#include "DhcpClient.h"

using namespace std;

CDhcpc::CDhcpc()
{
}

CDhcpc::~CDhcpc()
{
}

IDhcpClient *CDhcpc::CreateClient()
{
	return new CDhcpClient();
}

void CDhcpc::DeleteClient(IDhcpClient *pDhcpClient)
{
	SAFE_DELETE(pDhcpClient);
}
