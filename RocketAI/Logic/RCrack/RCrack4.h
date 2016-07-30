#ifndef __RCrack4_h__
#define __RCrack4_h__

#include "IRCrack.h"

class CRPlayer;
class CRCrack4 : public IRCrack {
  public:
	SINGLETON(CRCrack4);
	~CRCrack4();
  private:
	CRCrack4();

  protected:
	byte			 GetContract(byte byScore, byte byWinBitmap);
	byte			 GetMaxScore(byte byWinBitmap);

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

#define g_pRCrack4		CRCrack4::GetInstancePtr()

#endif
