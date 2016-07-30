#include "StdAfx.h"
#include "RoomManage.h"
#include "TableManage.h"
#include "PlayerManage.h"
#include "corepacketdefine.h"
#include "coreopcode.h"
#include "globaldefine.h"

CRoomManage::CRoomManage()
{
	m_nTotalRoom = 0;
}

CRoomManage::~CRoomManage()
{
}

void CRoomManage::Release()
{
	//±ÿ–Îœ» Õ∑≈PlayerManage
	for (size_t i = 0; i < m_vecPlayerManage.size(); i++)
		SAFE_RELEASE(m_vecPlayerManage[i]);
	for (size_t i = 0; i < m_vecTableManage.size(); i++)
		SAFE_RELEASE(m_vecTableManage[i]);
}

bool CRoomManage::Init(int nTotalRoom)
{
	m_nTotalRoom = nTotalRoom;
	m_vecPlayerManage.resize(m_nTotalRoom);
	m_vecTableManage.resize(m_nTotalRoom);
	for (int i = 0; i < m_nTotalRoom; i++)
	{
		m_vecPlayerManage[i] = new CPlayerManage(i);
		m_vecTableManage[i] = new CTableManage(i);
	}
	return true;
}

CTableManage* CRoomManage::FindTableManage(int nRoomID)
{
	if (nRoomID >= 0 && nRoomID < m_nTotalRoom)
	{
		return m_vecTableManage[nRoomID];
	}
	return NULL;
}

CPlayerManage* CRoomManage::FindPlayerManage(int nRoomID)
{
	if (nRoomID >= 0 && nRoomID < m_nTotalRoom)
	{
		return m_vecPlayerManage[nRoomID];
	}
	return NULL;
}

void CRoomManage::HeartBit()
{
	CEchoNot not;
	not.opcode = OP_EchoNot;
	not.nNow = GetTickCount();
	COutputStream os;
	os << not;

	for (int i = 0; i < m_nTotalRoom; i++)
	{
		m_vecPlayerManage[i]->OnHeartBit(not.nNow, os.GetData(), os.GetLength());
	}
}
