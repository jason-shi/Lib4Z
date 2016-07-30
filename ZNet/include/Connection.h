#include "INetServer.h"

class CConnection : public IConnection {
  public:
	CConnection(SOCKET socket, IPassive *pPassive);
	~CConnection();

  public:
	void			 SetPassive(IPassive *pPassive) { m_pPassive = pPassive; }
	void			 Send(const char* pData, int nLength);
	bool			 CloseConnection();
	void			 Release();
	void			 GetPeerAddr(struct sockaddr* peer);
	void			 ShiftOnLogin();
	SOCKET			 GetSocket() { return m_socket; }
	IPassive		*GetPassive() { return m_pPassive; }

  private:
	SOCKET			 m_socket;
	IPassive		*m_pPassive;
};
