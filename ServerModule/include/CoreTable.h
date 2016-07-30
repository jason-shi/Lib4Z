#ifndef __CoreTable_h__
#define __CoreTable_h__

#include <IServerModule.h>

enum CTableStatus {
	TS_ZERO,
	TS_EMPTY,
	TS_WATING,
	TS_RACING,
	OTHERTABLESTATUS,
};

class CCorePlayer;
class CPlayerManage;
class CCoreTable : public ICoreTable {
	typedef std::vector<CCorePlayer *>		 VECPLAYER;
  public:
	CCoreTable(int nRoomID, int nTableID);
	~CCoreTable();

  public:
	void			 Release();
	void			 EndGame();
	void			 AddTimer(int type, int timeout, FN_TimerProc TimerProc, void *lpParam = NULL);
	void			 DelTimer(int type);
	void			 SetPause(bool bPause);

  public:
	void			 SetStatus(int nStatus) { m_nStatus = (CTableStatus)nStatus; }
	int				 GetStatus() { return m_nStatus; }
	bool			 CanStartGame(int nChairID);
	void			 OnStartGame();
	void			 InsertPlayer(CCorePlayer *pCorePlayer, int nChairID);
	void			 RemovePlayer(int nChairID);
	bool			 CheckEnterTable(CCorePlayer *pCorePlayer, int nChairID);
	bool			 CheckLeaveTable(CCorePlayer *pCorePlayer);
	void			 EnterTable(CCorePlayer* pCorePlayer, int nChairID);
	void			 LeaveTable(CCorePlayer* pCorePlayer);
	void			 ForceLeave(CCorePlayer* pCorePlayer);
	template<class T>
		void		 NotifyTable(const T& packet, ICorePlayer* pExceptPlayer = NULL)
	{
		COutputStream os;
		os << packet;
		NotifyTable(os.GetData(), os.GetLength(), pExceptPlayer);
	}
	void			 NotifyTable(const char* pData, int nLength, ICorePlayer* pExceptPlayer);

  private:
	VECPLAYER		 m_vecPlayer;
	CTableStatus	 m_nStatus;
	int				 m_nSitDown;
	int				 m_nRoomID;
	int				 m_nTableID;
	int				 m_nPlayerNum;

  public:
	ITable			*m_pTable;
};

#endif
