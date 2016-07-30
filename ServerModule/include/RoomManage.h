#ifndef __RoomManage_h__
#define __RoomManage_h__

class CTableManage;
class CPlayerManage;
class CCorePlayer;
class CRoomManage {
	typedef std::vector<CTableManage *>		VECTABLE;
	typedef std::vector<CPlayerManage *>	VECPLAYER;
  public:
	SINGLETON(CRoomManage);
	~CRoomManage();
  private:
	CRoomManage();

  public:
	void			 Release();
	bool			 Init(int nTotalRoom);
	CTableManage	*FindTableManage(int nRoomID);
	CPlayerManage	*FindPlayerManage(int nRoomID);
	void			 HeartBit();

  private:
	int				 m_nTotalRoom;
	VECTABLE		 m_vecTableManage;
	VECPLAYER		 m_vecPlayerManage;
};

#define RoomManage			CRoomManage::GetInstancePtr()

#endif
