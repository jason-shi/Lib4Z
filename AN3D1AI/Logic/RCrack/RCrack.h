#ifndef __RCrack_h__
#define __RCrack_h__

#include "AN3D1AI.h"
#include "..\RPlayer\RPlayer.h"

#define MAX_ACTION_DEPTH		55

struct TZAction {
	byte			 byType;    //Type of the first card in the current round;
	                            //TYPE_GVP should be set at the beginning of a new round
	byte			 byRspType; //Type of the last card;
	                            //TYPE_GVP should be set at the beginning of a new round
	byte			 byGrade;   //0 means card 3, 1 means card 4, and so on, until 11 means card ace;
	                            //12 means minor card 2, 13 means trump card 2, 14 means little joker, 15 means big joker
	byte			 byChair;   //Chair ID of the current putter
	byte			 byChair1;  //Chair ID of the first putter in the current round

	byte			 byChairM;  //Chair ID of the biggest card in the current round
	byte			 byTypeM;   //Type of the biggest card in the current round
	byte			 byGradeM;  //Grade of the biggest card in the current round

	byte			 byCurr5TK; //Score of 5,10,K in the current round
	byte			 byGain5TK; //From banker's perspective
	byte			 byLost5TK; //From banker's perspective
};

class CRCrack {
  public:
	SINGLETON(CRCrack);
	~CRCrack();
  private:
	CRCrack();

  public:
	bool			 Initial();
	void			 CallCrack(TCrackParam* pCrackParam, TCrackResult *pCrackResult);
	void			 InitOriCardCount();
	void			 InitOriTotal5TK();
	byte			 CrackSche();

  protected:
	byte			 GetNextPlayer(byte byChair);
	void			 SaveAction();
	void			 BackAction();
	void			 GetResult(TCrackResult *pCrackResult);
	void			 SetResult(TCrackResult *pCrackResult);

  protected:
	byte			 TranslateWin();
	byte			 UpdateAction();
	void			 RestoreWith(const TCrackParam& tCrackParam);
	byte			 Crack();

  protected:
	byte			 GetMiddleGrade(word _bitmap);
	void			 InitTrumpCount();
	void			 InitEscapeCard();
	void			 SimSweepCard(word bitmap[4], byte byFuTwo[4]);
	void			 InitBankerSweep();
	void			 InitMaxGrade();
	void			 InitMaxScore();
	void			 InitOverBanker();
	void			 InitScoreBitmap();

  protected:
	byte			 BankerFirstCard(byte &byGrade);
	byte			 BankerFollowCard(byte &byGrade);
	byte			 PlayerFirstCard(byte &byGrade);
	byte			 PlayerFollowCard(byte &byGrade);
	void			 Think();

  protected:
	TCrackParam		 m_tCrackParam;
	TCrackResult	 m_tCrackResult;

  protected:
	CRPlayer		*m_pRPlayer[4];
	byte			 m_byBanker;
	byte			 m_byFriend; //¶ÓÓÑÎ»Í¼

  protected:
	TZAction		 m_tAction[MAX_ACTION_DEPTH];
	int				 m_nActionDepth;

  protected:
	TZAction		 Z;
};

#define g_pRCrack		CRCrack::GetInstancePtr()


inline byte CRCrack::GetNextPlayer(byte byChair)
{
	return ++byChair & 0x03;
}

inline void CRCrack::SaveAction()
{
	memcpy(&m_tAction[m_nActionDepth++], &Z, sizeof(TZAction));
	if (m_nActionDepth != 1)
		m_pRPlayer[Z.byChair]->OutCards(Z.byRspType, Z.byGrade);
}

inline void CRCrack::BackAction()
{
	memcpy(&Z, &m_tAction[--m_nActionDepth], sizeof(TZAction));
	if (m_nActionDepth != 0)
		m_pRPlayer[Z.byChair]->InCards(Z.byRspType, Z.byGrade);
}

inline void CRCrack::GetResult(TCrackResult *pCrackResult)
{
	pCrackResult->byRspType = m_tAction[1].byRspType;
	pCrackResult->byGrade = m_tAction[1].byGrade;
}

inline void CRCrack::SetResult(TCrackResult *pCrackResult)
{
	m_tAction[1].byRspType = pCrackResult->byRspType;
	m_tAction[1].byGrade = pCrackResult->byGrade;
}

#endif
