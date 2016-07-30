#include "StdAfx.h"
#include "TableManage.h"
#include "PlayerManage.h"
#include "CoreTable.h"
#include "ServerModule.h"
#include "globaldefine.h"

CTableManage::CTableManage(int nRoomID)
{
	m_nRoomID = nRoomID;
	m_nTotalTable = g_pServerModule->m_nTotalTable;
	m_vecCoreTable.resize(m_nTotalTable);
	for (int i = 0; i < m_nTotalTable; i++)
	{
		m_vecCoreTable[i] = new CCoreTable(nRoomID, i);
	}
}

CTableManage::~CTableManage()
{
}

void CTableManage::Release()
{
	for (size_t i = 0; i < m_vecCoreTable.size(); i++)
		SAFE_RELEASE(m_vecCoreTable[i]);

	delete this;
}

CCoreTable* CTableManage::FindTable(int nRoomID)
{
	if (nRoomID >= 0 && nRoomID < m_nTotalTable)
	{
		return m_vecCoreTable[nRoomID];
	}
	return NULL;
}
