#include "LoginManage.h"
#include "Connection.h"

CLoginManage			*g_pLoginManage;

CLoginManage::CLoginManage()
{
}

CLoginManage::~CLoginManage()
{
}

bool CLoginManage::InsertConnection(CConnection* pConnection)
{
	LOGINMAP::iterator iter = m_mapLogin.find(pConnection->GetSocket());
	if (iter != m_mapLogin.end())
	{
		return false;
	}
	m_mapLogin[pConnection->GetSocket()].pConnection = pConnection;
	return true;
}

bool CLoginManage::RemoveConnection(SOCKET socket)
{
	LOGINMAP::iterator iter = m_mapLogin.find(socket);
	if (iter != m_mapLogin.end())
	{
		m_mapLogin.erase(socket);
		return true;
	}
	return false;
}

CConnection* CLoginManage::FindConnection(SOCKET socket)
{
	LOGINMAP::iterator iter = m_mapLogin.find(socket);
	if (iter != m_mapLogin.end())
	{
		return iter->second.pConnection;
	}
	return NULL;
}
