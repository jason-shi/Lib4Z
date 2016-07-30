#ifndef __JReqManage_h__
#define __JReqManage_h__

struct JRequest {
	dword			 dwEvent;
	int				 nLength;
	byte			*pBuffer;

	JRequest		*m_pNext;
};

class JReqManage {
  public:
	JReqManage();
	virtual ~JReqManage();

  public:
	bool			 InitReqManage(int nReqLength, int nReqSize);
	void			 ClearRequest();
	int				 GetReqLength();

  private:
	void			 _PushFront(JRequest *pNode);
	void			 _PushBack(JRequest *pNode);
	JRequest		*_TakeFront();
	void			 _Delete(JRequest *pParent, JRequest *pNode);

  private:
	JRequest		*_GetNode();
	void			 _RtnNode(JRequest *pNode);

  public:
	bool			 SendRequest(dword dwEvent, byte *pBuff, int nLength);
	bool			 PostRequest(dword dwEvent, byte *pBuff, int nLength);

  public:
	bool			 GetRequest(JRequest *pJRequest, dword dwFilterMin, dword dwFilterMax);
	bool			 PeekRequest(JRequest *pJRequest, bool bRemove);
	bool			 PeekRequest(JRequest *pJRequest, dword dwFilterMin, dword dwFilterMax, bool bRemove);

  protected:
	int				 m_nReqLength;
	int				 m_nReqSize;
	JRequest	   **m_pJRequest;

  protected:
	CIntLock		 m_mutex;
	JRequest		*m_pFree;
	JRequest		*m_pReqHead; //始终指向保护结点
	JRequest		*m_pReqTail; //指向最后一个结点
	int				 m_nReqCount;
};

inline int JReqManage::GetReqLength()
{
	return m_nReqLength;
}

inline JRequest *JReqManage::_GetNode()
{
	JRequest *pNode = m_pFree;
	m_pFree = m_pFree->m_pNext;

	return pNode;
}

inline void JReqManage::_RtnNode(JRequest *pNode)
{
	pNode->m_pNext = m_pFree;
	m_pFree = pNode;
}

#endif
