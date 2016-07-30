#ifndef __IDhcpClient_h__
#define __IDhcpClient_h__

class IDhcpClient {
  public:
	virtual ~IDhcpClient() {}

  public:
	virtual bool	 Initial(dword dwClientID, dword dwProcBase) = 0;
	virtual void	 Run() = 0;

  public:
	virtual dword	 GetJDLink() const = 0;
	virtual void	 AddJDest(dword dwJDest) = 0;
	virtual void	 DelJDest(dword dwJDest) = 0;
};

#endif
