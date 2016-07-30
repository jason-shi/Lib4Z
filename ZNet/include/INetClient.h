#ifndef __INetClient_h__
#define __INetClient_h__

#include <WinSock2.h>

class INetClient {
  public:
	virtual ~INetClient() {}

  public:
	virtual bool	 ConnectToServer(HWND hWnd, UINT uAsynMsg, char* szIP, int nPort, UINT uMsg) = 0;
	virtual void	 Release() = 0;
	virtual bool	 IsConnected() = 0;
	virtual bool	 DisconnectToServer() = 0;
	virtual void	 OnLoop(int nElapse) = 0;
	virtual void	 Send(const char* pData, int nLength) = 0;
	virtual LRESULT	 OnSocketMessage(WPARAM wParam, LPARAM lParam) = 0;
};

INetClient			*CreateNetClient();

#ifdef	_MSC_VER
	#ifdef	_DEBUG
		#pragma comment(lib, "NetClientD.lib")
	#else
		#pragma comment(lib, "NetClient.lib")
	#endif
#endif

#endif
