#include "ConnManage.h"
#include "Connection.h"

CConnManage			*g_pConnManage;

CConnManage::CConnManage()
{
}

CConnManage::~CConnManage()
{
}

bool CConnManage::InsertConnection(CConnection* pConnection)
{
	CONNECTIONMAP::iterator iter = m_mapConnection.find(pConnection->GetSocket());
	if (iter != m_mapConnection.end())
	{
		return false;
	}
	m_mapConnection[pConnection->GetSocket()].pConnection = pConnection;
	return true;
}

bool CConnManage::RemoveConnection(SOCKET socket)
{
	CONNECTIONMAP::iterator iter = m_mapConnection.find(socket);
	if (iter != m_mapConnection.end())
	{
		m_mapConnection.erase(socket);
		return true;
	}
	return false;
}

CConnection* CConnManage::FindConnection(SOCKET socket)
{
	CONNECTIONMAP::iterator iter = m_mapConnection.find(socket);
	if (iter != m_mapConnection.end())
	{
		return iter->second.pConnection;
	}
	return NULL;
}

void CConnManage::OnLoop(int nElapse)
{
	std::vector<SOCKET> vecKeys;
	CONNECTIONMAP::iterator iter = m_mapConnection.begin();
	for (; iter != m_mapConnection.end(); iter++)
	{
		st_connection& connection = iter->second;
		connection.nTime += nElapse;

		if(connection.nTime >= 10 * 1000)
		{
			connection.pConnection->Release();
			vecKeys.push_back(iter->first);
		}
	}

	for (size_t i = 0; i < vecKeys.size(); i++)
	{
		m_mapConnection.erase(vecKeys[i]);
	}
}
