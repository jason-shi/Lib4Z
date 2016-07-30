#ifndef __IServerModule_h__
#define __IServerModule_h__

#include <netstream.h>
#include <coreopcode.h>
#include <corepacketdefine.h>

class ICorePlayer;
class ICoreTable;
class IPlayer { //上层玩家
  public:
	virtual ~IPlayer() {}
	virtual void	 BindCorePlayer2Player(ICorePlayer* pCorePlayer) = 0;
	virtual void	 ProcessPacket(const char* pData, int nLength) = 0;
};

class ITable;
class ICorePlayer { //下层玩家
  public:
	virtual ~ICorePlayer() {}
	virtual ITable	*GetTable() = 0;
	virtual void	 Send(const char* pData, int nLength) = 0;
};

class ITable { //上层桌子
  public:
	virtual ~ITable() {}
	virtual void	 BindCoreTable2Table(ICoreTable* pCoreTable) = 0;
	virtual void	 Release() = 0;
	virtual void	 OnStartGame() = 0;
	virtual IPlayer	*GetPlayer(int nChairID) = 0;
};

class ICoreTable { //下层桌子
  public:
	virtual ~ICoreTable() {}
	virtual void	 Release() = 0;
	virtual void	 EndGame() = 0;
	virtual void	 AddTimer(int type, int timeout, FN_TimerProc TimerProc, void *lpParam = NULL) = 0;
	virtual void	 DelTimer(int type) = 0;
	virtual void	 SetPause(bool bPause) = 0;
	virtual void	 NotifyTable(const char* pData, int nLength, ICorePlayer* pExceptPlayer) = 0;
};

class ITableCreator {
  public:
	virtual ~ITableCreator() {}
	virtual ITable	*CreateTable(int nPlayerNum) = 0;
};

struct TServerConfig;
class IServerModule {
  public:
	virtual ~IServerModule() {}
	virtual void	 InitServerModule(TServerConfig *pServerConfig) = 0;
	virtual void	 SetTableCreator(ITableCreator *pTableCreator) = 0;
	virtual void	 Release() = 0;
};

IServerModule		*CreateServerModule();

#ifdef	_MSC_VER
	#ifdef	_DEBUG
		#pragma comment(lib, "ServerModuleD.lib")
	#else
		#pragma comment(lib, "ServerModule.lib")
	#endif
#endif

#endif
