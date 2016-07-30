#ifndef __NetClient_h__
#define __NetClient_h__

#include "INetClient.h"

#define MaxMsgLength			1024

class CNetClient : public INetClient {
  public:
	CNetClient();
	virtual ~CNetClient();

  public:
	virtual void	 Release();
	virtual bool	 ConnectToServer(HWND hWnd, UINT uAsynMsg, char* szIP, int nPort, UINT uMsg);
	virtual void	 OnLoop(int nElapse);
	virtual bool	 IsConnected() { return m_bConnected; }
	virtual void	 Send(const char* pData, int nLength);
	virtual LRESULT	 OnSocketMessage(WPARAM wParam, LPARAM lParam);

  public:
	bool			 DisconnectToServer();

  public:
	HWND			 m_hWnd;
	UINT			 m_uMsg;

  protected:
	bool			 m_bConnected;
	int				 m_nCurRecv;
	CHAR			 m_szBuff[MaxMsgLength + 1]; // For Receive
};

extern CNetClient			*g_pNetClient;

#endif
