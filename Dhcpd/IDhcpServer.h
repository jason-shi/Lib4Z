#ifndef __IDhcpServer_h__
#define __IDhcpServer_h__

class IDhcpServer {
  public:
	  virtual ~IDhcpServer() {}

  public:
	virtual bool	 Initial(dword dwServerID, dword dwProcBase) = 0;
	virtual void	 Run() = 0;

  public:
	virtual dword	 GetJDLink() const = 0;
	virtual void	 AddJDest(dword dwJDest) = 0;
};

#endif
