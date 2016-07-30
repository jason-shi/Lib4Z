#ifndef __TableManage_h__
#define __TableManage_h__

#include <hash_map>
#include <string>

class CCoreTable;
class CCorePlayer;
class CPlayerManage;
class CTableManage {
	typedef std::vector<CCoreTable *>		 VECTABLE;
  public:
	CTableManage(int nRoomID);
	~CTableManage();

  public:
	void			 Release();
	bool			 InsertTable(CCoreTable* table);
	bool			 RemoveTable(int nPID);
	CCoreTable		*FindTable(int nRoomID);
	void			 SendUserList(CCorePlayer* pReceiver);
	void			 OnHeartBit(unsigned int nNow);

  private:
	int				 m_nRoomID;
	int				 m_nTotalTable;
	VECTABLE		 m_vecCoreTable;
};

#endif
