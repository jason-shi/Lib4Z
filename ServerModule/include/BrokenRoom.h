#ifndef __BrokenRoom_h__
#define __BrokenRoom_h__

class CCorePlayer;
class CBrokenRoom {
	class st_broken {
	  public:
		int			nTime; //¶ÏÏßÊ±¼ä
		CCorePlayer	*pCorePlayer;
	};
	typedef stdext::hash_map<int, st_broken>	PLAYERMAP;

  public:
	SINGLETON(CBrokenRoom);
	~CBrokenRoom();
  private:
	CBrokenRoom();

  public:
	bool			 InsertPlayer(CCorePlayer *player);
	bool			 RemovePlayer(int nPID);
	void			 OnLoop(int nElapse);

  private:
	PLAYERMAP		m_mapPlayers;
};

#endif
