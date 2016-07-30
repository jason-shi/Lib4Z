#include "Connection.h"
#include "ConnManage.h"
#include "LoginManage.h"
#include "EnDecode.h"

CConnection::CConnection(SOCKET socket, IPassive *pPassive)
{
	m_socket = socket;
	m_pPassive = pPassive;
	m_pPassive->OnConnection(this);
}

CConnection::~CConnection()
{
	m_pPassive = NULL;
}

void CConnection::Send(const char *pData, int nLength)
{
	char m_szPacket[1024]; //��Ҫ��֤�㹻�󣬼���ÿ��Packet���������ݲ�����1K
	int nPos = fnEncode6BitBufA((unsigned char*)pData, m_szPacket+1, nLength, sizeof(m_szPacket));
	m_szPacket[0] = '#';
	m_szPacket[nPos + 1] = '!';
	send(m_socket, m_szPacket, nPos + 2, 0);
}

bool CConnection::CloseConnection()
{
	if (m_socket != INVALID_SOCKET)
	{
		LINGER lingerStruct;

		lingerStruct.l_onoff	= 1;
		lingerStruct.l_linger	= 0;

		setsockopt(m_socket, SOL_SOCKET, SO_LINGER, (char *)&lingerStruct, sizeof(lingerStruct));

		g_pLoginManage->RemoveConnection(m_socket);

		closesocket(m_socket);
		m_socket = INVALID_SOCKET;
		return true;
	}
	return false;
}

void CConnection::Release()
{
	CloseConnection(); //�ͷ�socket

	if (m_pPassive) //�ͷ���ض���
	{
		m_pPassive->OnDisconnect();
		m_pPassive->Release();
		m_pPassive = NULL;
	}
	delete this; //�ͷ��Լ�
}

void CConnection::GetPeerAddr(struct sockaddr* peer)
{
	memset(peer, 0, sizeof(sockaddr));
	int length = sizeof(sockaddr);
	getpeername(m_socket, peer, &length);
}

void CConnection::ShiftOnLogin()
{
	g_pLoginManage->InsertConnection(this);
	g_pConnManage->RemoveConnection(m_socket);
}
