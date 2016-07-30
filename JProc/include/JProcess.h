/******************************************************************************
 *                                  JProc说明
 *
 * 新建态--0        队列
 * 运行态--1        队列
 * 阻塞态--2        队列
 * 备用态--3        队列
 * 挂起-就绪态--4   队列
 * 挂起-阻塞态--5   队列
 * 终止态--6        队列
 * 僵化态--7        队列
 * 就绪态--8        队列 优先级N
 *
 * 共需要 8 + N 个队列，第X优先级的就绪态的队列指针为 p[8 + N]
 *
 * 进程队列最通常的行为是添删队头，添加队尾。
 *
 *            pHead          pTail
 *              |              |
 *              |              |
 *              |              |
 *              V              V
 * 进程队列为   P -> A -> B -> C -> NULL
 *
 * 为确保循环的触发不丢失，采用容量足够大的Request
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
	dword			 m_dwProcID; //高16位为模块号，低16位为实例号
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
