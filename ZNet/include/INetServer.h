#ifndef __INetServer_h__
#define __INetServer_h__

#include <WinSock2.h>

class IPassive;
class IConnection { //主动事件接口
  public:
	virtual ~IConnection() {}
	virtual void	 SetPassive(IPassive *pPassive) = 0;
	virtual void	 Send(const char* pData, int nLength) = 0;
	virtual bool	 CloseConnection() = 0;
	virtual void	 Release() = 0;
	virtual void	 GetPeerAddr(struct sockaddr* peer) = 0;
	virtual void	 ShiftOnLogin() = 0;
};

class IPassive {
  public:
	virtual ~IPassive() {}
	virtual void	 OnConnection(IConnection *pConnection) = 0;
	virtual void	 OnRecv(const char* pData, int nLen) = 0;
	virtual void	 OnDisconnect() = 0;
	virtual void	 SendLoginReq() = 0;
	virtual void	 Release() = 0;
};

class ICorePlayerCreator {
  public:
	virtual ~ICorePlayerCreator() {}
	virtual IPassive *CreateCorePlayer() = 0;
};

class INetServer {
  public:
	virtual ~INetServer() {}
	virtual void	 InitNetServer(int nPort) = 0;
	virtual void	 OnLoop(int nElapse) = 0;
	virtual void	 Release() = 0;
	virtual void	 SetCorePlayerCreator(ICorePlayerCreator* pCorePlayerCreator) = 0;
};

INetServer				*CreateNetServer();

#ifdef	_MSC_VER
	#ifdef	_DEBUG
		#pragma comment(lib, "NetServerD.lib")
	#else
		#pragma comment(lib, "NetServer.lib")
	#endif
#endif

#endif
