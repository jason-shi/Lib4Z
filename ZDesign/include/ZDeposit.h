#ifndef __ZDeposit_h__
#define __ZDeposit_h__

#include <SingleTon.h>

class CZDeposit {
  public:
	SINGLETON(CZDeposit);
	~CZDeposit();

  private:
	CZDeposit();

  public:
	bool			 Alloc(int nSize, int nCount, int nNext, int nPrev); //32个元素以内
	void			 MemFill(void *pBuffer);
	void			*GetNode();
	void			 RtnNode(char *pNode);
	void			 Impawn(char *pNode);
	void			 Redeem(char *pNode);
	void			 Reclaim();

  private:
	char			*m_pHead;
	int				 m_nSize;
	int				 m_nCount;
	int				 m_nNext;
	int				 m_nPrev;
	_int64			 m_nFlag;
};

#define g_pZDeposit	 CZDeposit::GetInstancePtr()

#endif
