#include "StdAfx.h"
#include "CorePlayer.h"
#include "CoreTable.h"
#include "RoomManage.h"
#include "TableManage.h"
#include "PlayerManage.h"
#include "BrokenRoom.h"
#include "coreopcode.h"
#include "ServerModule.h"

static int			 g_nPID = 1;

CCorePlayer::CCorePlayer()
{
	m_pCoreTable = NULL;
	m_pPlayerManage = NULL;
	m_pTableManage = NULL;
	m_pConnection = NULL;
	m_pPlayer = NULL;
}

CCorePlayer::~CCorePlayer()
{
}

void CCorePlayer::Release()
{
	//玩家进入房间而未进入桌子时，m_pCoreTable和m_pPlayerManage均为NULL
	if (m_pCoreTable)
		m_pCoreTable->RemovePlayer(GetChairID());

	m_pCoreTable = NULL;
	m_pPlayerManage = NULL;
	m_pTableManage = NULL;

	//释放连接
	if (m_pConnection)
	{
		m_pConnection->SetPassive(NULL); //注：釜底抽薪
		m_pConnection->Release();
		m_pConnection = NULL;
	}
	m_pPlayer = NULL;

	//此时CorePlayer实体处于游离状态
	delete this;
}

void CCorePlayer::OnConnection(IConnection* pConnection)
{
	m_pConnection = pConnection;
	m_nLastPacketTime = 0;
	m_nNetDelay = 200;
	m_bForceLeave = false;
	m_bBroken = false;
	m_strAddr.clear();
	m_nPort = 0;
	m_nPID = g_nPID++;
	GetPlayerIP();
	GetPlayerPort();
	SetChairID(-1);
	SetStatus(US_CONNECTED);
}

void CCorePlayer::OnDisconnect()
{
	if (GetStatus() >= US_CONNECTED)
	{
		if (GetStatus() >= US_TABLE)
		{
			if (GetStatus() == US_RACING)
			{
				SetBrokenStatus(true);
				CBrokenRoom::GetInstancePtr()->InsertPlayer(this);
				m_pCoreTable->ForceLeave(this);
			}
			else
			{
				m_pCoreTable->LeaveTable(this);
			}
		}
		if (!m_bBroken)
		{
			m_pPlayerManage->RemovePlayer(m_nPID);
			SetStatus(US_UNCONNECTED);
		}
	}
	m_pConnection = NULL;
}

const std::string& CCorePlayer::GetPlayerIP()
{
	if(m_pConnection && m_strAddr.empty())
	{
		struct sockaddr peer;
		m_pConnection->GetPeerAddr(&peer);
		const char* p = inet_ntoa(((sockaddr_in*)&peer)->sin_addr);
		m_strAddr = p;
	}
	return m_strAddr;
}

const unsigned short CCorePlayer::GetPlayerPort()
{
	if(m_pConnection && m_nPort == 0)
	{
		struct sockaddr peer;
		m_pConnection->GetPeerAddr(&peer);
		m_nPort = ((sockaddr_in*)&peer)->sin_port;
	}
	return m_nPort;
}

void CCorePlayer::HeartBit(uint nNow, const char* pData, int nLength)
{
	uint nElapse = nNow - m_nLastPacketTime;
	if (nElapse > 1000 * 60000 && !m_bBroken)
	{
		if(m_pConnection)
			m_pConnection->CloseConnection();
	}
	else if (nElapse >= 1000 * 5)
	{
		if (m_pConnection)
			Send(pData, nLength);
	}
}

ITable* CCorePlayer::GetTable()
{
	return m_pCoreTable->m_pTable;
}

void CCorePlayer::Send(const char* pData, int nLength)
{
	if(m_pConnection)
		m_pConnection->Send(pData, nLength);
}

void CCorePlayer::SendLoginReq()
{
	CLoginReq req;
	req.opcode = OP_LoginReq;
	req.nPID = m_nPID;
	SendPacket(req);
}

void CCorePlayer::SendPlayerNumber()
{
	if (g_pServerModule->m_nGameMode == GM_LOCALNET || g_pServerModule->m_nGameMode == GM_BATTLENET)
	{
		CPlayerNumberNot not;
		not.opcode = OP_PlayerNumberNot;
		not.nPlayerNum = g_pServerModule->m_nPlayerNum;
		SendPacket(not);
	}
}

void CCorePlayer::OnRecv(const char* pData, int nLen)
{
	m_nLastPacketTime = g_uiCurTime; //记录时间

	CInputStream is(pData, nLen);
	short opcode;
	is >> opcode;
	is.Reset();
	switch (opcode)
	{
	case OP_LoginAct:				DISPATCH_MESSAGE(CLoginAct);				break;
	case OP_UserListReq:			DISPATCH_MESSAGE(CUserListReq);				break;
	case OP_EnterTableAct:			DISPATCH_MESSAGE(CEnterTableAct);			break;
	case OP_LeaveTableAct:			DISPATCH_MESSAGE(CLeaveTableAct);			break;
	case OP_ReadyAct:				DISPATCH_MESSAGE(CReadyAct);				break;
	case OP_EchoNot:				DISPATCH_MESSAGE(CEchoNot);					break;
	case OP_DetailReq:				DISPATCH_MESSAGE(CDetailReq);				break;
	default:
		m_pPlayer->ProcessPacket(pData, nLen);
		break;
	}
}

void CCorePlayer::OnPacket(const CLoginAct& act)
{
	m_pConnection->ShiftOnLogin(); //连接集合转移至登陆集合

	m_pPlayerManage = RoomManage->FindPlayerManage(act.nRoomID);
	m_pTableManage = RoomManage->FindTableManage(act.nRoomID);

	if (!m_pPlayerManage->InsertPlayer(this))
	{
		CCorePlayer* pCorePlayer = m_pPlayerManage->FindPlayer(act.nPID);
		if (pCorePlayer == this) //自己踢自己
			return;
		if (pCorePlayer) //异地登陆
		{
			//首先释放原来的连接
			if (pCorePlayer->GetConnection())
			{
				pCorePlayer->GetConnection()->SetPassive(NULL);
				pCorePlayer->GetConnection()->CloseConnection();
			}
			if (pCorePlayer->IsBroken() || pCorePlayer->GetStatus() == US_RACING) //断线重连
			{
				if (pCorePlayer->IsBroken())
				{
					pCorePlayer->SetBrokenStatus(false);
					pCorePlayer->UpdateHeartBit(GetTickCount());
					CBrokenRoom::GetInstancePtr()->RemovePlayer(act.nPID);
				}
				pCorePlayer->OnConnection(m_pConnection);
				m_pConnection->SetPassive(pCorePlayer);
				if (m_pCoreTable->GetStatus() == TS_RACING)
				{
					pCorePlayer->SetStatus(US_RACING);
				}
				else
				{
					pCorePlayer->SetStatus(US_LOBBY);
				}
			}
			else //重新连接
			{
				pCorePlayer->OnDisconnect();
				pCorePlayer->Release();
				m_pPlayerManage->InsertPlayer(this);
			}
		}
	}
	else
	{
		SetStatus(US_LOBBY);
		CLoginNot not;
		not.opcode = OP_LoginNot;
		SendPacket(not);
	}
	SendPlayerNumber();
}

void CCorePlayer::OnPacket(const CUserListReq& req)
{
	m_pPlayerManage->SendUserList(this);
}

void CCorePlayer::OnPacket(const CEnterTableAct& act)
{
	if (m_pCoreTable)
	{
		//预检查是否可以离开桌子
		if (!m_pCoreTable->CheckLeaveTable(this))
			return;
	}
	CCoreTable *pCoreTable = m_pTableManage->FindTable(act.nTableID);
	if (pCoreTable)
	{
		//预检查是否可以进入桌子
		if (!pCoreTable->CheckEnterTable(this, act.nChairID))
			return;
	}

	if (m_pCoreTable)
		m_pCoreTable->LeaveTable(this);
	if (pCoreTable)
		pCoreTable->EnterTable(this, act.nChairID);
}

void CCorePlayer::OnPacket(const CLeaveTableAct& act)
{
	if (m_pCoreTable)
	{
		//预家查是否可以离开桌子
		if (!m_pCoreTable->CheckLeaveTable(this))
			return;

		m_pCoreTable->LeaveTable(this);
	}
}

void CCorePlayer::OnPacket(const CReadyAct& act)
{
	if (GetStatus() != US_TABLE) return; //防止重复处理此消息

	SetStatus(US_READY);
	//收到后广播
	CReadyNot not;
	not.opcode = OP_ReadyNot;
	not.nChairID = m_UserData.nChairID;
	m_pPlayerManage->NotifyRoom(not);

	//判断是否可以开始游戏
	if (m_pCoreTable->CanStartGame(m_UserData.nChairID))
		m_pCoreTable->OnStartGame();
}

void CCorePlayer::OnPacket(const CEchoNot& noti)
{
	m_nNetDelay = g_uiCurTime - noti.nNow;
}

void CCorePlayer::OnPacket(const CDetailReq& req)
{
	CCorePlayer *pCorePlayer = m_pPlayerManage->FindPlayer(req.nick);
	if (pCorePlayer != NULL)
	{
		CDetailNot not;
		not.opcode = OP_DetailNot;
		not.userInfo = pCorePlayer->m_UserInfo;
		not.userData = pCorePlayer->m_UserData;
		pCorePlayer->SendPacket(not);
	}
}
