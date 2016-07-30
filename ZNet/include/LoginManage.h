#ifndef __LoginManage_h__
#define __LoginManage_h__

#include <hash_map>
#include <winsock2.h>

class CConnection;
class CLoginManage {
	class st_connection {
	  public:
		CConnection			   *pConnection; //主动连接类
	};
	typedef stdext::hash_map<SOCKET, st_connection> LOGINMAP;
  public:
	CLoginManage();
	~CLoginManage();

  public:
	bool			 InsertConnection(CConnection* pConnection);
	bool			 RemoveConnection(SOCKET socket);
	CConnection		*FindConnection(SOCKET socket);

  private:
	LOGINMAP		 m_mapLogin;
};

extern CLoginManage			*g_pLoginManage;

#endif
