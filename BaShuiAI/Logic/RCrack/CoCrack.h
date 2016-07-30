#ifndef __CoCrack_h__
#define __CoCrack_h__

#include "IRCrack.h"

class CCoCrack : public CRCrack {
  public:
	SINGLETON(CCoCrack);
	~CCoCrack();
  private:
	CCoCrack();

  public:
	void			 CallCrack(const TCoCrackParam* pCoCrackParam, TCoCrackResult *pCoCrackResult);
  protected:
	void			 InitCrack(const TCoCrackParam *pCoCrackParam);
	int				 GetSplitIndex(int nPlayer);

  protected:
	void			 SetResultType(int i, TCoCrackResult *pCoCrackResult, const dword v);
	void			 SetResultCard(int i, TCoCrackResult *pCoCrackResult, const dword v);
	void			 THShunAdjust(int i, TCoCrackResult *pCoCrackResult);

  protected:
	byte			 CheckKillAll(TCoCrackResult *pCoCrackResult);
	byte			 GetChairByScore(byte byKillAll, TCoCrackResult *pCoCrackResult);
	bool			 ComapreWith(byte byPileNum, byte byCard1[8], byte byCard2[8]);
	int				 GetWinScore(TCoCrackResult *pCoCrackResult);

  protected:
	byte			 V[TotalPlayer - 1][8 + 1]; //牌值
	word			 S[TotalPlayer - 1][4 + 1]; //花色
	byte			 TH1[TotalPlayer - 1]; //同花1
	byte			 TH2[TotalPlayer - 1]; //同花2
	byte			 m_byOriCard[TotalPlayer - 1][8];
};

#define g_pCoCrack		CCoCrack::GetInstancePtr()

#endif
