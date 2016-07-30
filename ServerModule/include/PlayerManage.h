#ifndef __PlayerManage_h__
#define __PlayerManage_h__

class CCorePlayer;
class CPlayerManage {
	typedef stdext::hash_map<int, CCorePlayer *>		 PLAYERMAP;
  public:
	CPlayerManage(int nRoomID);
	~CPlayerManage();

  public:
	void			 Release();
	bool			 InsertPlayer(CCorePlayer* player);
	bool			 RemovePlayer(int nPID);
	CCorePlayer		*FindPlayer(int nPID);
	CCorePlayer		*FindPlayer(std::string nick);
	void			 SendUserList(CCorePlayer* pReceiver);
	void			 OnHeartBit(unsigned int nNow, const char* pData, int nLength);
	template<class T>
		void		 NotifyRoom(const T& packet, CCorePlayer* pExceptPlayer = NULL)
	{
		COutputStream os;
		os << packet;
		NotifyRoom(os.GetData(), os.GetLength(), pExceptPlayer);
	}
	void			 NotifyRoom(const char* pData, int nLength, CCorePlayer* pExceptPlayer);

  private:
	int						 m_nRoomID;
	PLAYERMAP				 m_mapPlayers;
};

#endif
