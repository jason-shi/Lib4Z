#ifndef	__RspbManage_h__
#define	__RspbManage_h__

#include <SingleTon.h>

class IRspblty {
  public:
	IRspblty();
	virtual ~IRspblty();
};

class IRspbZler {
  public:
	IRspbZler();
	virtual ~IRspbZler();

  public:
	virtual bool	 ProcessRspb(int nType);
	virtual bool	 ProcessRspb(IRspblty *pRspblty);

  public:
	IRspbZler		*m_pNext;
};

class CRspbManage {
  public:
	SINGLETON(CRspbManage);
	~CRspbManage();
  private:
	CRspbManage();

  public:
	void			 AddRspbZler(IRspbZler *pRspbZler);
	void			 DelRspbZler(IRspbZler *pRspbZler);
	bool			 ProcessRspb(int nType);
	bool			 ProcessRspb(IRspblty *pRspblty);

  protected:
	IRspbZler		*m_pHead;
};

#define g_pRspbManage			 CRspbManage::GetInstancePtr()

#endif
