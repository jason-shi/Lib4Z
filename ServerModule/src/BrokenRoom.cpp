#include "StdAfx.h"
#include "BrokenRoom.h"
#include "CorePlayer.h"
#include "CoreTable.h"
#include "ServerModule.h"

CBrokenRoom::CBrokenRoom()
{
}

CBrokenRoom::~CBrokenRoom()
{
}

bool CBrokenRoom::InsertPlayer(CCorePlayer *player)
{
	PLAYERMAP::iterator iter = m_mapPlayers.find(player->GetPID());
	if(iter != m_mapPlayers.end())
	{
		return false;
	}
	m_mapPlayers[player->GetPID()].pCorePlayer = player;
	return true;
}

bool CBrokenRoom::RemovePlayer(int nPID)
{
	PLAYERMAP::iterator iter = m_mapPlayers.find(nPID);
	if(iter != m_mapPlayers.end())
	{
		m_mapPlayers.erase(nPID);
		return true;
	}
	return false;
}

void CBrokenRoom::OnLoop(int nElapse)
{
	std::vector<int> vecKeys;
	PLAYERMAP::iterator iter = m_mapPlayers.begin();
	for(; iter != m_mapPlayers.end(); iter++)
	{
		st_broken& broken = iter->second;
		broken.nTime += nElapse;

		if(broken.nTime >= 60 * 1000 * 6) //¶ÏÏß³¬Ê±
		{
			broken.pCorePlayer->SetBrokenStatus(false);
			CCoreTable *pCoreTable = broken.pCorePlayer->GetCoreTable();
			if (pCoreTable)
				pCoreTable->ForceLeave(broken.pCorePlayer);
			broken.pCorePlayer->GetConnection()->Release();
			vecKeys.push_back(iter->first);
		}
	}

	for(size_t i = 0; i < vecKeys.size(); i++)
	{
		m_mapPlayers.erase(vecKeys[i]);
	}
}
