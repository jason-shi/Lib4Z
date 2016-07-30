#ifndef __IRCrack_h__
#define __IRCrack_h__

#include "BaShuiAI.h"

class IRCrack {
  public:
	IRCrack();
	virtual ~IRCrack();

  //�ⲿ�ӿ�
  public:
	virtual bool	 Initial();
	virtual void	 CallCrack(const TCrackParam* pCrackParam, TCrackResult *pCrackResult);

  protected:
	TCrackParam		 m_tCrackParam;
	TCrackResult	 m_tCrackResult;
};

#endif
