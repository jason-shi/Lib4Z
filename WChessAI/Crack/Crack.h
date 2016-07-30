#include "PRIVATE_NAMESPACE_BEGIN"

#ifndef __Crack_h__
#define __Crack_h__

class CCrack {
  public:
	SINGLETON(CCrack);
	virtual ~CCrack();

  private:
	CCrack();

  public:
	bool			 Initial();
	void			 Release();
	void			 SetWindow(CWindow *_window);
	void			 SetMsgId(UINT uMsgId);

  public:
	int				 GetThinkDepth() const;
	int				 GetAstodeSize() const;
	int				 GetSpendTime() const;
	TCrackResult&	 GetCrackResult() const;

  public:
	bool			 InitThreadRFCrack(TCrackParam *pCrackParam);
	bool			 InitThreadBFCrack(TCrackParam *pCrackParam);
	void			 WaitForThreadCrack();
	void			 ExitThreadCrack();

  public:
	void			 PostCrackResult(int nResult);
	void			 OnTimer(int nTimerId);

  private:
	CWindow			*window;
	UINT			 m_uMsgId;
};

inline void CCrack::SetWindow(CWindow *_window)
{
	window = _window;
}

inline void CCrack::SetMsgId(UINT uMsgId)
{
	m_uMsgId = uMsgId;
}

inline void CCrack::PostCrackResult(int nResult)
{
	window->PostMessage(m_uMsgId, nResult);
}

#define g_pZCrack		 CCrack::GetInstancePtr()

#endif

#include "PRIVATE_NAMESPACE_END"
