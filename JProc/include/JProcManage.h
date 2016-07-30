#ifndef __JProcManage_h__
#define __JProcManage_h__

#include "JProcess.h"

#undef MAX_PRIORITY
#define MAX_PRIORITY					10

class JProcManage {
	typedef std::vector<JProcess *>		 VECJPROC;
  public:
	SINGLETON(JProcManage);
	~JProcManage();

  private:
	JProcManage();

  private:
	void			 _PushFront(int nIndex, JProcess *pNode);
	void			 _PushBack(int nIndex, JProcess *pNode);
	JProcess		*_TakeFront(int nIndex);
	void			 _Delete(int nIndex, JProcess *pParent, JProcess *pNode);
	void			 _TurnBack(int nIndex);
	void			 _MoveBack(int nIndex1, int nIndex2);

  private:
	JProcess		*_GetNode();
	void			 _RtnNode(JProcess *pNode);

  public:
	bool			 Initial();
	void			 AddProcess(JProcess *pJProcess);
	void			 DelProcess(JProcess *pJProcess);
	JProcess		*FindProcess(dword dwProcID);

  public:
	bool			 Suspend(JProcess *pJProcess);
	bool			 Resume(JProcess *pJProcess);
	int				 WaitMe(JProcess *pJProcess);

  public:
	void			 JProcSchedule(byte byCPUID);

  public:
	VECJPROC		 m_vecJProc;
	VECJPROC		 m_vecJQuit;
	JProcess		*m_pFree;

  protected:
	CIntLock		 m_mutex;
	JProcess		*m_pProcHead[8 + MAX_PRIORITY];
	JProcess		*m_pProcTail[8 + MAX_PRIORITY];
	int				 m_nProcCount[8 + MAX_PRIORITY];
};

inline void JProcManage::_MoveBack(int nIndex1, int nIndex2)
{
	_PushBack(nIndex2, _TakeFront(nIndex1));
}

inline JProcess *JProcManage::_GetNode()
{
	JProcess *pNode = m_pFree;
	m_pFree = m_pFree->m_pNext;

	return pNode;
}

inline void JProcManage::_RtnNode(JProcess *pNode)
{
	pNode->m_pNext = m_pFree;
	m_pFree = pNode;
}

#define g_pJProcManage				JProcManage::GetInstancePtr()

#endif
