#ifndef __RCrack2_h__
#define __RCrack2_h__

#include "IRCrack.h"

class CRPlayer;
class CRCrack2 : public IRCrack {
  public:
	SINGLETON(CRCrack2);
	~CRCrack2();
  private:
	CRCrack2();

  protected:
	bool			 TranslatePut(byte &byChair, byte &byChair1, byte &byType, byte &byRspType, byte &byGrade, byte &byCount);
	bool			 TranslateGvp(byte &byChair, byte &byChair1, byte &byType, byte &byRspType, byte &byGrade, byte &byCount);
	byte			 TranslateWin(byte byScore, byte &byWinBitmap, byte &byChair, byte &byChair1, byte &byType, byte &byRspType, byte &byGrade, byte &byCount);

  protected:
	byte			 Crack(byte byWinBitmap, byte byChair1, byte byType, byte byRspType, byte byGrade, byte byCount, byte byScore);
	void			 Think(byte byChair1, byte byType, byte byRspType, byte byGrade, byte byCount);
	byte			 Optimize(byte byWinBitmap, byte byChair1, byte byType, byte byRspType, byte byGrade, byte byCount, byte byScore);
	byte			 CrackSche();
};

#define g_pRCrack2		CRCrack2::GetInstancePtr()

#endif
