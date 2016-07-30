/******************************************************************************
 *                                  JProc˵��
 *
 * �½�̬--0        ����
 * ����̬--1        ����
 * ����̬--2        ����
 * ����̬--3        ����
 * ����-����̬--4   ����
 * ����-����̬--5   ����
 * ��ֹ̬--6        ����
 * ����̬--7        ����
 * ����̬--8        ���� ���ȼ�N
 *
 * ����Ҫ 8 + N �����У���X���ȼ��ľ���̬�Ķ���ָ��Ϊ p[8 + N]
 *
 * ���̶�����ͨ������Ϊ����ɾ��ͷ����Ӷ�β��
 *
 *            pHead          pTail
 *              |              |
 *              |              |
 *              |              |
 *              V              V
 * ���̶���Ϊ   P -> A -> B -> C -> NULL
 *
 * Ϊȷ��ѭ���Ĵ�������ʧ�����������㹻���Request
 *
 ******************************************************************************/
#ifndef __JProcess_h__
#define __JProcess_h__

#define JP_NEWPROC				0
#define JP_RUNNING				1
#define JP_BLOCKED				2
#define JP_STANDBY				3
#define JP_SUS_READY			4
#define JP_SUS_BLKED			5
#define JP_DONE					6
#define JP_ZOMIFIED				7
#define JP_READY				8

struct JMessage;
struct JRequest;

class JProcessCreator;
class JMsgManage;
class JReqManage;
class JProcess {
  public:
	JProcess(dword dwProcID, byte byPriority);
	virtual ~JProcess();

  public:
	bool			 InitMsgManage(int nMsgLength, int nMsgSize);
	bool			 InitReqManage(int nReqLength, int nReqSize);
	virtual void	 ProcEntry(byte byState, dword dwEvent, byte *pBuff, int nLength) {}

  public:
	dword			 GetProcID() const;
	byte			 GetPriority() const;
	byte			 GetState() const;
	void			 NextState(byte byState);

  public:
	int				 GetQIndex() const;

  public:
	bool			 Suspend();
	bool			 Resume();
	int				 WaitMe(dword dwMilliseconds);

  protected:
	virtual void	 OnDestroy();

  public:
	int				 SendMessage(dword dwProcID, dword dwEvent, byte *pBuff = NULL, int nLength = 0);
	bool			 PostMessage(dword dwProcID, dword dwEvent, byte *pBuff = NULL, int nLength = 0);
	bool			 WaitMessage();
	bool			 GetMessage(JMessage *pJMessage, dword dwFilterMin, dword dwFilterMax);
	bool			 PeekMessage(JMessage *pJMessage, dword dwFilterMin, dword dwFilterMax, bool bRemove);

  public:
	int				 SendRequest(dword dwProcID, dword dwEvent, byte *pBuff = NULL, int nLength = 0);
	bool			 PostRequest(dword dwProcID, dword dwEvent, byte *pBuff = NULL, int nLength = 0);
	bool			 WaitRequest();
	bool			 GetRequest(JRequest *pJRequest, dword dwFilterMin, dword dwFilterMax);
	bool			 PeekRequest(JRequest *pJRequest, dword dwFilterMin, dword dwFilterMax, bool bRemove);

  protected:
	dword			 m_dwProcID; //��16λΪģ��ţ���16λΪʵ����
	byte			 m_byPriority;
	byte			 m_byState;

  public:
	byte			 m_byStatus;
	JProcess		*m_pJSender;

  public:
	JProcess		*m_pNext;
	JMsgManage		*m_pJMsgManage;
	JReqManage		*m_pJReqManage;
};

JProcess			*CreateJProcess(JProcessCreator *pCreator,
									dword dwProcID, byte byPriority, int nMsgLength, int nMsgSize, int nReqLength, int nReqSize);
void				 WaitForJProcess(dword dwProcID);
void				 PostMessageWait(dword dwProcID, dword dwEvent, byte *pBuff = NULL, int nLength = 0);

inline dword JProcess::GetProcID() const
{
	return m_dwProcID;
}

inline byte JProcess::GetPriority() const
{
	return m_byPriority;
}

inline byte JProcess::GetState() const
{
	return m_byState;
}

inline void JProcess::NextState(byte byState)
{
	m_byState = byState;
}

inline int JProcess::GetQIndex() const
{
	return m_byStatus < 8 ? m_byStatus : 8 + m_byPriority;
}

#endif
