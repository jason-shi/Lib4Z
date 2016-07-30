#ifndef __IRCrack_h__
#define __IRCrack_h__

#include "DDiZhuAI.h"

#define MAX_CRACK_DEPTH			64

struct TRAction {
	byte			 byChair;
	byte			 byChair1;
	byte			 byType;
	byte			 byRspType;
	byte			 byGrade;
	byte			 byCount;
	word			 wAttach;
	word			 wChoice;
};

class CRPlayer;
class IRCrack {
  public:
	IRCrack();
	virtual ~IRCrack();

  //外部接口
  public:
	virtual bool	 Initial();
	virtual void	 CallCrack(TCrackParam *pCrackParam, TCrackResult *pCrackResult);

  protected:
	byte			 GetNextPlayer(byte byChair);
	bool			 IsStupidPut(byte byType, byte byGrade, byte byCount, word wAttach);

  protected:
	void			 GetResult(TCrackResult *pCrackResult);
	void			 SetResult(TCrackResult *pCrackResult);
	void			 SaveAction(byte byChair, byte byChair1, byte byType, byte byRspType, byte byGrade, byte byCount, word wAttach, word wChoice);
	void			 BackAction(byte &byChair, byte &byChair1, byte &byType, byte &byRspType, byte &byGrade, byte &byCount, word &wAttach, word &wChoice);

  //破解、思考、优化的调度过程
  protected:
	virtual void	 InitCrackParam();
	virtual byte	 Crack(byte byChair1, byte byType, byte byRspType, byte byGrade, byte byCount, word wAttach) = 0;
  protected:
	virtual void	 InitThinkParam();
	virtual void	 Think(byte byChair1, byte byType, byte byRspType, byte byGrade, byte byCount, word wAttach) = 0;
  protected:
	virtual void	 InitOptimizeParam();
	virtual byte	 Optimize(byte byChair1, byte byType, byte byRspType, byte byGrade, byte byCount, word wAttach) = 0;
  protected:
	virtual byte	 CrackSche() = 0;

  protected:
	byte			 m_byFriend; //队友位图
	int				 m_nDepth;   //思考深度
	bool			 m_bGvpTag;  //放弃标记

  protected:
	CRPlayer		*m_pRPlayer[3];
	TRAction		*m_pRAction[MAX_CRACK_DEPTH];

  protected:
	TCrackParam		 m_tCrackParam;
	TCrackResult	 m_tCrackResult;
};

inline byte IRCrack::GetNextPlayer(byte byChair)
{
	static byte TChair[3] = { 1, 2, 0, };
	return TChair[byChair];
}

#endif
