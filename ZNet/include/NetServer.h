#ifndef __NetServer_h__
#define __NetServer_h__

#include "INetServer.h"

#define MaxMsgLength			1024

class CConnManage;
class CNetServer : public INetServer {
  public:
	CNetServer();
	virtual ~CNetServer();

  public:
	virtual void	 InitNetServer(int nPort);
	virtual void	 OnLoop(int nElapse);
	virtual void	 Release();
	virtual void	 SetCorePlayerCreator(ICorePlayerCreator *pCorePlayerCreator);
	IPassive		*CreateCorePlayer() { return m_pCorePlayerCreator->CreateCorePlayer(); }

  private:
	ICorePlayerCreator		*m_pCorePlayerCreator;
};

extern CNetServer			*g_pNetServer;

#endif
