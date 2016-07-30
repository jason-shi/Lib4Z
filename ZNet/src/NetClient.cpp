#include "NetClient.h"
#include "EnDecode.h"
#include "syncobj.h"
#include "TCHAR.h"
#include <queue>

#pragma	warning(disable: 4996)

CNetClient					*g_pNetClient;

static SOCKET				 g_sockClient;
static std::queue<BYTE*>	 g_SendToClientQ;
static CCriticalSection		 mutex;
static char					 szBuff[MaxMsgLength + 1];
static size_t				 nBuff = 0;

char* OnMessageReceive(CHAR* pszMessage)
{
	char	 szDecodeMsg[MaxMsgLength + 1];
	char	*pszBegin, *pszEnd;
	int		 nCount = 0;

	if (*pszMessage == '*')
	{
		pszBegin = pszMessage + 1;
		send(g_sockClient, "*", 1, 0);
	}
	else
		pszBegin = pszMessage;

	while (pszEnd = strchr(pszBegin, '!'))
	{
		*pszEnd++ = '\0';

		int nPos = fnDecode6BitBufA(pszBegin + 1, szDecodeMsg, sizeof(szDecodeMsg));
		SendMessage(g_pNetClient->m_hWnd, g_pNetClient->m_uMsg, (WPARAM)szDecodeMsg, nPos);

		if (*pszEnd == '*')
		{
			send(g_sockClient, "*", 1, 0);
			pszEnd++;
		}

		if (*pszEnd == '#')
			pszBegin = pszEnd;
		else
			return NULL;
	}
	return pszBegin;
}

INetClient* CreateNetClient()
{
	if (g_pNetClient == NULL)
	{
		g_pNetClient = new CNetClient();
	}
	return g_pNetClient;
}

CNetClient::CNetClient()
{
	g_sockClient = INVALID_SOCKET;
	m_bConnected = false;
}

CNetClient::~CNetClient()
{
}

void CNetClient::Release()
{
	//销毁SOCKET资源
	closesocket(g_sockClient);

	delete this;
}

bool CNetClient::ConnectToServer(HWND hWnd, UINT uAsynMsg, char* szIP, int nPort, UINT uMsg)
{
	SOCKADDR_IN		sockAddr;
	TCHAR			strError[256];

	m_hWnd = hWnd;
	m_uMsg = uMsg;

	if (g_sockClient == INVALID_SOCKET)
	{
		g_sockClient = socket(AF_INET, SOCK_STREAM, 0);
		if (g_sockClient == INVALID_SOCKET)
		{
			_stprintf(strError, _T("socket() generated error %d\n"), WSAGetLastError());
			OutputDebugString(strError);
			return FALSE;
		}

		sockAddr.sin_family		 = AF_INET;
		sockAddr.sin_port		 = htons( nPort );
		sockAddr.sin_addr.s_addr = inet_addr(szIP);

		if (WSAAsyncSelect(g_sockClient, hWnd, uAsynMsg, FD_CONNECT|FD_READ|FD_CLOSE) == SOCKET_ERROR)
		{
			_stprintf(strError, _T("WSAAsyncSelect() generated error %d\n"), WSAGetLastError());
			OutputDebugString(strError);
			return FALSE;
		}

		if (connect(g_sockClient, (const struct sockaddr FAR*)&sockAddr, sizeof(sockAddr)) == SOCKET_ERROR)
		{
			switch (WSAGetLastError())
			{
			case WSAEWOULDBLOCK:
				OutputDebugString( _T("This socket is Non-Blocking Mode" ));
				break;
			}
		}
	}
	return true;
}

bool CNetClient::DisconnectToServer()
{
	if (g_sockClient != INVALID_SOCKET)
	{
		closesocket(g_sockClient);
		g_sockClient = INVALID_SOCKET;
		m_bConnected = false;
		return true;
	}
	return false;
}

void CNetClient::OnLoop(int nElapse)
{
	if (g_SendToClientQ.size())
	{
		mutex.Lock();
		char *pszPacket = (char*)g_SendToClientQ.front();
		g_SendToClientQ.pop();
		mutex.Unlock();
		if (pszPacket)
		{
			strcpy(&szBuff[nBuff], pszPacket);
			char *pszRemain = OnMessageReceive(szBuff);
			if (pszRemain)
			{
				nBuff = strlen(pszRemain);
				memcpy(szBuff, pszRemain, nBuff);
				szBuff[nBuff] = '\0';
			}
			else
			{
				nBuff = 0;
				ZeroMemory(szBuff, sizeof(szBuff));
			}

			delete pszPacket;
		}
	}
}

void CNetClient::Send(const char *pData, int nLength)
{
	char m_szPacket[1024]; //需要保证足够大，假设每个Packet编码后的数据不超过1K
	int nPos = fnEncode6BitBufA((unsigned char*)pData, m_szPacket+1, nLength, sizeof(m_szPacket));
	m_szPacket[0] = '#';
	m_szPacket[nPos + 1] = '!';
	send(g_sockClient, m_szPacket, nPos + 2, 0);
}

LRESULT CNetClient::OnSocketMessage(WPARAM wParam, LPARAM lParam)
{
	switch (WSAGETSELECTEVENT(lParam))
	{
	case FD_CONNECT:
		ZeroMemory(m_szBuff, sizeof(m_szBuff));
		m_nCurRecv = 0;
		if (WSAGETSELECTERROR(lParam) == 0)
		{
			m_bConnected = true;
		}
		break;
	case FD_CLOSE:
		DisconnectToServer();
		break;
	case FD_READ:
		{
			int	nRecv, nCurRecv = 0;
			ioctlsocket((SOCKET)wParam, FIONREAD, (u_long *)&nRecv);
			char* pszPacket = NULL;
			while (!pszPacket) //直到申请成功
				pszPacket = new char[nRecv + 1];
			while (nRecv > nCurRecv)
				nCurRecv += recv((SOCKET)wParam, &pszPacket[nCurRecv], nRecv - nCurRecv, 0);
			pszPacket[nCurRecv] = '\0';
			mutex.Lock();
			g_SendToClientQ.push((BYTE*)pszPacket);
			mutex.Unlock();
			break;
		}
	}

	return 0L;
}
