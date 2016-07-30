#ifndef __RCrack3_h__
#define __RCrack3_h__

#include "IRCrack.h"

class CRPlayer;
class CRCrack3 : public IRCrack {
  public:
	SINGLETON(CRCrack3);
	~CRCrack3();
  private:
	CRCrack3();

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

#define g_pRCrack3		CRCrack3::GetInstancePtr()

#endif
