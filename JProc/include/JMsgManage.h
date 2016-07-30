#ifndef __JMsgManage_h__
#define __JMsgManage_h__

struct JMessage {
	dword			 dwEvent;
	int				 nLength;
	byte			*pBuffer;

	JMessage		*m_pNext;
};

class JMsgManage {
  public:
	JMsgManage();
	virtual ~JMsgManage();

  public:
	bool			 InitMsgManage(int nMsgLength, int nMsgSize);
	void			 ClearMessage();
	int				 GetMsgLength();

  private:
	void			 _PushFront(JMessage *pNode);
	void			 _PushBack(JMessage *pNode);
	JMessage		*_TakeFront();
	void			 _Delete(JMessage *pParent, JMessage *pNode);

  private:
	JMessage		*_GetNode();
	void			 _RtnNode(JMessage *pNode);

  public:
	bool			 SendMessage(dword dwEvent, byte *pBuff, int nLength);
	bool			 PostMessage(dword dwEvent, byte *pBuff, int nLength);

  public:
	bool			 GetMessage(JMessage *pJMessage, dword dwFilterMin, dword dwFilterMax);
	bool			 PeekMessage(JMessage *pJMessage, bool bRemove);
	bool			 PeekMessage(JMessage *pJMessage, dword dwFilterMin, dword dwFilterMax, bool bRemove);

  protected:
	int				 m_nMsgLength;
	int				 m_nMsgSize;
	JMessage	   **m_pJMessage;

  protected:
	CIntLock		 m_mutex;
	JMessage		*m_pFree;
	JMessage		*m_pMsgHead; //始终指向保护结点
	JMessage		*m_pMsgTail; //指向最后一个结点
	int				 m_nMsgCount;
};

inline int JMsgManage::GetMsgLength()
{
	return m_nMsgLength;
}

inline JMessage *JMsgManage::_GetNode()
{
	JMessage *pNode = m_pFree;
	m_pFree = m_pFree->m_pNext;

	return pNode;
}

inline void JMsgManage::_RtnNode(JMessage *pNode)
{
	pNode->m_pNext = m_pFree;
	m_pFree = pNode;
}

#endif
