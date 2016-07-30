#ifndef __RCrack3_h__
#define __RCrack3_h__

#include "IRCrack.h"

class CRPlayer;
class CRCrack : public IRCrack {
  public:
	SINGLETON(CRCrack);
	~CRCrack();
  private:
	CRCrack();

  protected:
	byte			 TranslateWin(byte &byChair, byte &byChair1, byte &byType, byte &byRspType, byte &byGrade, byte &byCount, word &wAttach, word &wChoice);

  protected:
	byte			 Crack_1(byte byChair1, byte byType, byte byRspType, byte byGrade, byte byCount, word wAttach);
	byte			 Crack_2(byte byChair1, byte byType, byte byRspType, byte byGrade, byte byCount, word wAttach);

  protected:
	byte			 Crack(byte byChair1, byte byType, byte byRspType, byte byGrade, byte byCount, word wAttach);
	void			 Think(byte byChair1, byte byType, byte byRspType, byte byGrade, byte byCount, word wAttach);
	byte			 Optimize(byte byChair1, byte byType, byte byRspType, byte byGrade, byte byCount, word wAttach);
	byte			 CrackSche();
};

#define g_pRCrack		CRCrack::GetInstancePtr()

#endif
