#ifndef __ZPool_h__
#define __ZPool_h__

#include "Water.h"

#define MAX_POOL_FLAG			32

class CZPool {
  public:
	CZPool();
	~CZPool();

  public:
	bool			 Alloc(int nSize, int nCount);
	void			 MemFill(void *pBuffer);
	char			*GetNode();
	void			 RtnNode(char *pNode);
	void			 Reclaim();
	bool			 ForEach(FN_WaterProc WaterProc, void *lpParam);

  protected:
	char			*m_pHead;
	int				 m_nSize;
	int				 m_nCount;
	int				 m_nFlag[MAX_POOL_FLAG];
	char			*m_pSpare;
};

CZPool				*CreateZPool(int nSize, int nCount, void *pBuffer);

#endif
