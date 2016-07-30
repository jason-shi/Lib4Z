#ifndef __RCrack_h__
#define __RCrack_h__

#include "RocketAI.h"

#define MAX_CRACK_DEPTH			64

struct TRAction {
	byte			 byWinBitmap;
	byte			 byChair;
	byte			 byChair1;
	byte			 byType;
	byte			 byRspType;
	byte			 byGrade;
	byte			 byCount;
};

class CRPlayer;
class IRCrack {
  public:
	IRCrack();
	virtual ~IRCrack();

  public:
	virtual bool	 Initial();
	virtual void	 SetBitmap(byte byChair, word bitmap[4]);
	void			 GetResult(TCrackResult *pCrackResult);
	void			 SetResult(TCrackResult *pCrackResult);

  protected:
	byte			 GetNextPlayer(byte byChair);
	void			 GetNextRspser(byte &byChair, byte &byChair1);
	bool			 CheckCanCha(byte &byChaChair, byte byGrade);
	bool			 CheckCanGou(byte &byChaChair, byte byGrade);
	bool			 IsStupidPut(byte byType, byte byGrade, byte byCount);
	void			 InitThkParam();

  protected:
	void			 SaveAction(byte byWinBitmap, byte byChair, byte byChair1, byte byType, byte byRspType, byte byGrade, byte byCount);
	void			 BackAction(byte &byWinBitmap, byte &byChair, byte &byChair1, byte &byType, byte &byRspType, byte &byGrade, byte &byCount);

  protected:
	virtual byte	 Crack(byte byWinBitmap, byte byChair1, byte byType, byte byRspType, byte byGrade, byte byCount, byte byScore) = 0;
	virtual void	 Think(byte byChair1, byte byType, byte byRspType, byte byGrade, byte byCount) = 0;
	virtual byte	 Optimize(byte byWinBitmap, byte byChair1, byte byType, byte byRspType, byte byGrade, byte byCount, byte byScore) = 0;
	virtual byte	 CrackSche() = 0;

  public:
	void			 CallCrack(TCrackParam *pCrackParam, TCrackResult *pCrackResult);

  protected:
	int				 m_nDepth; //思考深度
	bool			 m_bGvpTag; //放弃标记

  protected:
	CRPlayer		*m_pRPlayer[4];
	TRAction		*m_pRAction[MAX_CRACK_DEPTH];

  protected:
	TCrackParam		 m_tCrackParam;
	TCrackResult	 m_tCrackResult;
};

#endif
