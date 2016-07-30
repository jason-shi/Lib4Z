#ifndef __RCrack_h__
#define __RCrack_h__

#include "IRCrack.h"

class CRCrack : public IRCrack {
  public:
	SINGLETON(CRCrack);
	~CRCrack();
  protected:
	CRCrack();

  public:
	void			 CallCrack(const TCrackParam* pCrackParam, TCrackResult *pCrackResult);
  protected:
	void			 Crack_1(const TCrackParam* pCrackParam, TCrackResult *pCrackResult);
	void			 Crack_2(const TCrackParam* pCrackParam, TCrackResult *pCrackResult);

  protected:
	void			 InitCrack(const TCrackParam *pCrackParam);
	int				 GetSplitIndex();

  protected:
	void			 SetResultType(TCrackResult *pCrackResult, const dword v);
	void			 SetResultCard(TCrackResult *pCrackResult, const dword v);
	void			 THShunAdjust(TCrackResult *pCrackResult);

  public:
	int				 ComparePile(int nIndex1, int nIndex2);

  protected:
	byte			 V[8 + 1]; //牌值
	word			 S[4 + 1]; //花色
	byte			 TH1; //同花1
	byte			 TH2; //同花2
	byte			 m_byOriCard[8];
};

inline int CRCrack::ComparePile(int nIndex1, int nIndex2)
{
	return TblFastCompare[nIndex1] > TblFastCompare[nIndex2] ? 1
		 : TblFastCompare[nIndex1] < TblFastCompare[nIndex2] ? -1 : 0;
}

#define g_pRCrack		CRCrack::GetInstancePtr()

#endif
