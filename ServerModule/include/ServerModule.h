#ifndef __ServerModule_h__
#define __ServerModule_h__

#include "IServerModule.h"

struct TServerConfig {
	int nPort;
	int nTotalRoom;
	int nTotalTable;
	int nPlayerNum;
	int nGameMode;
	int nBattleMode;
	int nStartMode;
	bool bRunRandom;
	bool bRandomShuffle;
};

class CPlayerManage;
class CCoreTable;
class CServerModule : public IServerModule {
  public:
	CServerModule();
	virtual ~CServerModule() {}
	virtual void InitServerModule(TServerConfig *pServerConfig);
	virtual void Release();
	ITable* CreateTable(int nPlayerNum) { return m_pTableCreator->CreateTable(nPlayerNum); }
	virtual void SetTableCreator(ITableCreator* pTableCreator);

  private:
	ITableCreator				*m_pTableCreator;

  public:
	int							 m_nPort;
	int							 m_nTotalRoom;
	int							 m_nTotalTable;
	int							 m_nPlayerNum;
	int							 m_nGameMode;
	int							 m_nBattleMode;
	int							 m_nStartMode;
	bool						 m_bRunRandom;
	bool						 m_bRandomShuffle;
};

extern CServerModule	*g_pServerModule;
extern unsigned int		 g_uiCurTime;

#endif
