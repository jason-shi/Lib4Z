#include "StdAfx.h"
#include "PlayerManage.h"
#include "CorePlayer.h"
#include "coreopcode.h"

CPlayerManage::CPlayerManage(int nRoomID)
{
	m_nRoomID = nRoomID;
}

CPlayerManage::~CPlayerManage()
{
}

void CPlayerManage::Release()
{
	PLAYERMAP::iterator iter = m_mapPlayers.begin();
	for (; iter != m_mapPlayers.end(); iter++)
	{
		SAFE_RELEASE(iter->second);
	}

	delete this;
}

bool CPlayerManage::InsertPlayer(CCorePlayer* pPlayer)
{
	PLAYERMAP::iterator iter = m_mapPlayers.find(pPlayer->GetPID());
	if (iter != m_mapPlayers.end())
	{
		return false;
	}
	m_mapPlayers[pPlayer->GetPID()] = pPlayer;
	return true;
}

bool CPlayerManage::RemovePlayer(int nPID)
{
	PLAYERMAP::iterator iter = m_mapPlayers.find(nPID);
	if (iter != m_mapPlayers.end())
	{
		m_mapPlayers.erase(nPID);
		return true;
	}
	return false;
}

CCorePlayer* CPlayerManage::FindPlayer(int nPID)
{
	PLAYERMAP::iterator iter = m_mapPlayers.find(nPID);
	if (iter != m_mapPlayers.end())
	{
		return iter->second;
	}
	return NULL;
}

CCorePlayer* CPlayerManage::FindPlayer(std::string nick)
{
	PLAYERMAP::iterator iter = m_mapPlayers.begin();
	for (; iter != m_mapPlayers.end(); iter++)
	{
		if (iter->second)
		{
			if (nick.compare(iter->second->GetNick()) == 0)
			{
				return iter->second;
			}
		}
	}
	return NULL;
}

void CPlayerManage::SendUserList(CCorePlayer* pReceiver)
{
	int nCount = 1;
	CUserListNot noti;
	noti.opcode = OP_UserListNot;
	noti.vecBreviary.push_back(pReceiver->GetBreviary()); //压入pReceiver的信息

	PLAYERMAP::iterator iter = m_mapPlayers.begin();
	for (; iter != m_mapPlayers.end(); iter++)
	{
		if (iter->second)
		{
			CCorePlayer *pPlayer = iter->second;
			if (pPlayer->GetStatus() >= US_TABLE)
			{
				if (pPlayer == pReceiver)
					continue;
				noti.vecBreviary.push_back(iter->second->GetBreviary());
				if (pPlayer->IsBroken())
					noti.vecBreviary.back().nStatus = US_BROKEN;
				nCount++;
				if (nCount == 20)
				{
					pReceiver->SendPacket(noti);
					nCount = 0;
					noti.vecBreviary.clear();
				}
			}
		}
	}
	pReceiver->SendPacket(noti);
}

void CPlayerManage::OnHeartBit(uint nNow, const char* pData, int nLength)
{
	PLAYERMAP::const_iterator iter = m_mapPlayers.begin();
	for (; iter != m_mapPlayers.end(); iter++)
	{
		if (iter->second)
		{
			iter->second->HeartBit(nNow, pData, nLength);
		}
	}
}

void CPlayerManage::NotifyRoom(const char* pData, int nLength, CCorePlayer* pExceptPlayer)
{
	PLAYERMAP::iterator iter = m_mapPlayers.begin();
	for (; iter != m_mapPlayers.end(); iter++)
	{
		if (iter->second)
		{
			if (iter->second != pExceptPlayer && iter->second->GetStatus() >= US_LOBBY)
				iter->second->Send(pData, nLength);
		}
	}
}
