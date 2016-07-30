#ifndef __ConnManage_h__
#define __ConnManage_h__

#include <hash_map>
#include <winsock2.h>

class CConnection;
class CConnManage {
	class st_connection {
	  public:
		int							nTime; //����ʱ��
		CConnection				   *pConnection; //����������
	};
	typedef stdext::hash_map<SOCKET, st_connection> CONNECTIONMAP;
  public:
	CConnManage();
	~CConnManage();

  public:
	bool			 InsertConnection(CConnection *pConnection);
	bool			 RemoveConnection(SOCKET socket);
	CConnection		*FindConnection(SOCKET socket);
	void			 OnLoop(int nElapse);

  private:
	CONNECTIONMAP	 m_mapConnection;
};

extern CConnManage			*g_pConnManage;

#endif
