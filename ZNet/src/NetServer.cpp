#include "NetServer.h"
#include "Connection.h"
#include "ConnManage.h"
#include "LoginManage.h"
#include "EnDecode.h"
#include "syncobj.h"
#include <time.h>
#include <queue>

CNetServer					*g_pNetServer;

static bool					 g_bFlagDestroy;
static bool					 g_bFlagRecvMsg;
static SOCKET				 g_gcSock;
static SOCKADDR_IN			 g_gcAddr;
static HANDLE				 g_hThreadRecvMsg;
static std::queue<BYTE*>	 g_SendToServerQ;
static std::queue<BYTE*>	 g_HandleServerQ;
static fd_set				 fdSocket; //所有可用套接字集合
static CCriticalSection		 mutex;

typedef struct tag_TSENDBUFF
{
	SOCKET			sock;
	char			szData[MaxMsgLength + 1];
}_TSENDBUFF, *_LPTSENDBUFF;

CConnection* FindConnection(SOCKET socket)
{
	if (CConnection *pConnection = g_pLoginManage->FindConnection(socket))
	{
		return pConnection;
	}
	return g_pConnManage->FindConnection(socket);
}

static UINT WINAPI ThreadRecvMsg(LPVOID lpParameter)
{
	struct timeval tv = {1, 0};

	while (true)
	{
		if (g_bFlagDestroy)
		{
			g_bFlagRecvMsg = false;
			return 0;
		}
		fd_set fdRead = fdSocket; //被修改过的fd_set集合
		int nRet = select(0, &fdRead, NULL, NULL, &tv);
		if (nRet > 0) //处理未决I/O
		{
			for (int i = 0; i < (int)fdSocket.fd_count; i++)
			{
				if (FD_ISSET(fdSocket.fd_array[i], &fdRead))
				{
					if (fdSocket.fd_array[i] == g_gcSock)
					{
						if (fdSocket.fd_count < FD_SETSIZE)
						{
							sockaddr_in addrRemote;
							int nAddrLen = sizeof(addrRemote);
							SOCKET sNew = accept(g_gcSock, (SOCKADDR *)&addrRemote, &nAddrLen);
							if (sNew == INVALID_SOCKET)
								continue;
							g_pConnManage->InsertConnection(new CConnection(sNew, g_pNetServer->CreateCorePlayer()));
							g_pConnManage->FindConnection(sNew)->GetPassive()->SendLoginReq();
							FD_SET(sNew, &fdSocket);
						}
						else
						{
							printf("Too much connections!\n");
							continue;
						}
					}
					else
					{
						char recvBuff[MaxMsgLength + 1];
						int nRecv = recv(fdSocket.fd_array[i], recvBuff, MaxMsgLength, 0);
						if (nRecv > 0) //接收数据
						{
							recvBuff[nRecv] = '\0';
							_LPTSENDBUFF lpSendUserData = new _TSENDBUFF;
							lpSendUserData->sock = fdSocket.fd_array[i];
							memmove(lpSendUserData->szData, recvBuff, MaxMsgLength + 1);
							mutex.Lock();
							g_HandleServerQ.push((BYTE *)lpSendUserData);
							mutex.Unlock();
						}
						else //断开连接
						{
							CConnection *pConnection = FindConnection(fdSocket.fd_array[i]);
							if (pConnection)
								pConnection->Release();
							FD_CLR(fdSocket.fd_array[i], &fdSocket);
						}
					}
				}
			}
		}
		else if (nRet < 0)
		{
			printf("Failed select()\n");
			break;
		}
	}
	g_bFlagRecvMsg = false;
	return 0;
}

static bool InitThreadRecvMsg()
{
	DWORD dwThreadIDRecvMsg = 0;

	if (!g_hThreadRecvMsg)
	{
		g_hThreadRecvMsg = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadRecvMsg, NULL, 0, &dwThreadIDRecvMsg);

		if (g_hThreadRecvMsg)
		{
			g_bFlagRecvMsg = true;
			CloseHandle(g_hThreadRecvMsg);
			g_hThreadRecvMsg = NULL;
			return true;
		}
	}
	return false;
}

bool InitServerSocket(SOCKET &s, SOCKADDR_IN* addr, int nPort, long lEvent)
{
	if (s == INVALID_SOCKET)
	{
		s = socket(AF_INET, SOCK_STREAM, 0);

		addr->sin_family		= AF_INET;
		addr->sin_port			= htons(nPort);
		addr->sin_addr.s_addr	= htonl(INADDR_ANY);

		if ((bind(s, (const struct sockaddr FAR*)addr, sizeof(SOCKADDR_IN))) == SOCKET_ERROR)
			return false;

		if ((listen(s, 5)) == SOCKET_ERROR)
			return false;
	}
	return true;
}

INetServer *CreateNetServer()
{
	if (g_pNetServer == NULL)
	{
		g_pNetServer = new CNetServer();
	}
	return g_pNetServer;
}

CNetServer::CNetServer()
{
	g_pConnManage = new CConnManage();
	g_pLoginManage = new CLoginManage();
}

CNetServer::~CNetServer()
{
}

void CNetServer::InitNetServer(int nPort)
{
	g_gcSock = INVALID_SOCKET;
	InitServerSocket(g_gcSock, &g_gcAddr, nPort, 1);
	FD_ZERO(&fdSocket);
	FD_SET(g_gcSock, &fdSocket);

	g_bFlagDestroy = false;
	g_bFlagRecvMsg = false;
	InitThreadRecvMsg();
}

void CNetServer::Release()
{
	//销毁SOCKET资源
	shutdown(g_gcSock, SD_BOTH);
	closesocket(g_gcSock);
	//循环等待直到线程结束
	if (g_bFlagRecvMsg)
	{
		g_bFlagDestroy = true;
		while (g_bFlagRecvMsg);
	}
	delete g_pConnManage;
	delete g_pLoginManage;
	delete this;
}

void CNetServer::OnLoop(int nElapse)
{
	//连接超时未登陆
	g_pConnManage->OnLoop(nElapse);

	char *pszBegin, *pszEnd;
	char szDecodeMsg[MaxMsgLength + 1];

	mutex.Lock();
	while (!g_HandleServerQ.empty())
	{
		g_SendToServerQ.push(g_HandleServerQ.front());
		g_HandleServerQ.pop();
	}
	mutex.Unlock();
	int nCount = g_SendToServerQ.size();
	if (nCount)
	{
		for (int nLoop = 0; nLoop < nCount; nLoop++)
		{
			_LPTSENDBUFF pSendBuff = (_LPTSENDBUFF)g_SendToServerQ.front();
			g_SendToServerQ.pop();
			if (pSendBuff)
			{
				int nLen = strlen(pSendBuff->szData) + 1;

				if ((pszBegin = (char *)memchr(pSendBuff->szData, '#', nLen)) && (pszEnd = (char *)memchr(pSendBuff->szData, '!', nLen)))
				{
					*pszEnd = '\0';
					int nPos = fnDecode6BitBufA(pszBegin + 1, szDecodeMsg, sizeof(szDecodeMsg));
					FindConnection(pSendBuff->sock)->GetPassive()->OnRecv(szDecodeMsg, nPos);
				}
			}

			delete pSendBuff;
			pSendBuff = NULL;
		}
	}
}

void CNetServer::SetCorePlayerCreator(ICorePlayerCreator* pCorePlayerCreator)
{
	m_pCorePlayerCreator = pCorePlayerCreator;
}
