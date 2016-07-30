#include "StdAfx.h"
#include "CoreTable.h"
#include "CorePlayer.h"
#include "RoomManage.h"
#include "PlayerManage.h"
#include "coreopcode.h"
#include "ServerModule.h"

CCoreTable::CCoreTable(int nRoomID, int nTableID)
{
	m_nRoomID = nRoomID;
	m_nTableID = nTableID;
	m_nPlayerNum = g_pServerModule->m_nPlayerNum;
	m_vecPlayer.resize(m_nPlayerNum);
	m_nStatus = TS_EMPTY;
	m_nSitDown = 0;
	m_pTable = g_pServerModule->CreateTable(m_nPlayerNum);
	m_pTable->BindCoreTable2Table(this);
}

CCoreTable::~CCoreTable()
{
}

void CCoreTable::Release()
{
	SAFE_RELEASE(m_pTable);
}

void CCoreTable::AddTimer(int type, int timeout, FN_TimerProc TimerProc, void *lpParam)
{
	g_pZTimer->AddTimer(type, timeout, TimerProc, lpParam);
}

void CCoreTable::DelTimer(int type)
{
	g_pZTimer->DelTimer(type);
}

void CCoreTable::SetPause(bool bPause)
{
	g_pZTimer->SetPause(bPause);
}

void CCoreTable::EndGame()
{
	m_nStatus = TS_WATING;
	for (VECPLAYER::iterator iter = m_vecPlayer.begin(); iter != m_vecPlayer.end(); iter++)
	{
		if (*iter)
			(*iter)->SetStatus(US_TABLE);
	}
}

bool CCoreTable::CanStartGame(int nChairID)
{
	//���ݲ�ͬ�������ж��Ƿ���Կ�ʼ��Ϸ
	if (g_pServerModule->m_nGameMode == GM_SINGLEPLAY) //����ģʽ����λ���Ҿ���
	{
		for (VECPLAYER::iterator iter = m_vecPlayer.begin(); iter != m_vecPlayer.end(); iter++)
		{
			if (*iter)
			{
				if ((*iter)->GetChairID() == nChairID && (*iter)->GetStatus() == US_READY)
					return true;
			}
		}
	}
	else if (g_pServerModule->m_nGameMode == GM_LOCALNET || g_pServerModule->m_nGameMode == GM_BATTLENET)
	{
		if (g_pServerModule->m_nStartMode == 1) //ȫ����Ҫ����
		{
			if (m_nSitDown != g_pServerModule->m_nPlayerNum)
				return false;
		}
		for (VECPLAYER::iterator iter = m_vecPlayer.begin(); iter != m_vecPlayer.end(); iter++)
		{
			if (*iter)
			{
				if ((*iter)->GetStatus() != US_READY)
					return false;
			}
		}
		return true;
	}
	else if (g_pServerModule->m_nGameMode == GM_LOADREPLAY) //�ۿ�¼����������
	{
		return true;
	}
	return false;
}

void CCoreTable::OnStartGame()
{
	m_nStatus = TS_RACING;
	for (VECPLAYER::iterator iter = m_vecPlayer.begin(); iter != m_vecPlayer.end(); iter++)
	{
		if (*iter)
		{
			(*iter)->SetStatus(US_RACING);
		}
	}
	m_pTable->OnStartGame();
}

void CCoreTable::InsertPlayer(CCorePlayer *pCorePlayer, int nChairID)
{
	m_vecPlayer[nChairID] = pCorePlayer;
}

void CCoreTable::RemovePlayer(int nChairID)
{
	m_vecPlayer[nChairID] = NULL;
}

bool CCoreTable::CheckEnterTable(CCorePlayer *pCorePlayer, int nChairID)
{
	//�����������û����𣬻��֣������ʣ��Ƿ�����У��Ƿ���������Ƿ������Թۣ��Ƿ�����л�λ�ã��Ƿ����ʱ����λ��
	//Ŀǰ�����ֻ�������Ƿ�����У��Ƿ���������Ƿ����ʱ����λ��
	//Ŀǰֻ�����Ǳ����н��������λ�����λ��
	CEnterTableRej rej;
	rej.opcode = OP_EnterTableRej;
	if (m_nStatus == TS_RACING)
	{
		rej.cCode = 1; //������
		pCorePlayer->SendPacket(rej);
		return false;
	}
	if (m_vecPlayer[nChairID])
	{
		rej.cCode = 2; //�ǿ�����
		pCorePlayer->SendPacket(rej);
		return false;
	}
	return true;
}

bool CCoreTable::CheckLeaveTable(CCorePlayer *pCorePlayer)
{
	CLeaveTableRej rej;
	rej.opcode = OP_LeaveTableRej;
	if (m_nStatus == TS_RACING)
	{
		rej.cCode = 1; //������
		pCorePlayer->SendPacket(rej);
		return false;
	}
	return true;
}

void CCoreTable::EnterTable(CCorePlayer* pCorePlayer, int nChairID)
{
	InsertPlayer(pCorePlayer, nChairID);
	m_nSitDown++;
	m_nStatus = TS_WATING;

	pCorePlayer->SetStatus(US_TABLE);
	pCorePlayer->SetChairID(nChairID);
	pCorePlayer->SetIPlayer(m_pTable->GetPlayer(nChairID));
	pCorePlayer->GetIPlayer()->BindCorePlayer2Player(pCorePlayer);
	pCorePlayer->SetCoreTable(this);

	//���ͽ�����λ�Ĺ㲥
	CEnterTableNot not;
	not.opcode = OP_EnterTableNot;
	not.nTableID = m_nTableID;
	not.nChairID = nChairID;
	not.cOwner = true;
	pCorePlayer->SendPacket(not);
	not.cOwner = false;
	RoomManage->FindPlayerManage(m_nRoomID)->NotifyRoom(not, pCorePlayer);
}

void CCoreTable::LeaveTable(CCorePlayer* pCorePlayer)
{
	int nChairID = pCorePlayer->GetChairID();
	RemovePlayer(nChairID);
	if (--m_nSitDown == 0)
		m_nStatus = TS_EMPTY;

	pCorePlayer->SetStatus(US_LOBBY);
	pCorePlayer->SetChairID(-1);
	pCorePlayer->GetIPlayer()->BindCorePlayer2Player(NULL);
	pCorePlayer->SetIPlayer(NULL);
	pCorePlayer->SetCoreTable(NULL);

	//�����뿪��λ�Ĺ㲥
	CLeaveTableNot not;
	not.opcode = OP_LeaveTableNot;
	not.nTableID = m_nTableID;
	not.nChairID = nChairID;
	not.cOwner = true;
	pCorePlayer->SendPacket(not);
	not.cOwner = false;
	RoomManage->FindPlayerManage(m_nRoomID)->NotifyRoom(not, pCorePlayer);
}

void CCoreTable::ForceLeave(CCorePlayer* pCorePlayer)
{
	int nChairID = pCorePlayer->GetChairID();
	RemovePlayer(nChairID);
	if (--m_nSitDown == 0)
	{
		m_nStatus = TS_EMPTY;
		EndGame();
	}
	pCorePlayer->GetIPlayer()->BindCorePlayer2Player(NULL);
	pCorePlayer->SetIPlayer(NULL);
	SAFE_DELETE(pCorePlayer);

	//�����뿪��λ�Ĺ㲥
	CLeaveTableNot not;
	not.opcode = OP_LeaveTableNot;
	not.nTableID = m_nTableID;
	not.nChairID = nChairID;
	not.cOwner = true;
	pCorePlayer->SendPacket(not);
	not.cOwner = false;
	RoomManage->FindPlayerManage(m_nRoomID)->NotifyRoom(not, pCorePlayer);
}

void CCoreTable::NotifyTable(const char* pData, int nLength, ICorePlayer* pExceptPlayer)
{
	for (VECPLAYER::iterator iter = m_vecPlayer.begin(); iter != m_vecPlayer.end(); iter++)
	{
		if (*iter)
		{
			if (*iter != pExceptPlayer && (*iter)->GetStatus() >= US_TABLE)
				(*iter)->Send(pData, nLength);
		}
	}
}
