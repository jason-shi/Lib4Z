#ifndef __RPlayerXJ_h__
#define __RPlayerXJ_h__

#include "..\RPlayer.h"

class CRPlayerXJ : public CRPlayer {
  public:
	SINGLETON(CRPlayerXJ);
	~CRPlayerXJ();

  private:
	CRPlayerXJ();

  public:
	byte			 _MakeGvp(byte &byGrade, byte &byCount, word &wAttach);
	byte			 _MakeSingle(byte &byGrade, byte &byCount);
	byte			 _MakePair(byte &byGrade, byte &byCount);
	byte			 _MakeAnySingle(byte &byGrade, byte &byCount);
	byte			 _MakeAnyPair(byte &byGrade, byte &byCount);

  private:
	byte			 _MakeGvpRound1(byte &byGrade, byte &byCount, word &wAttach);
	byte			 _MakeGvpRound2(byte &byGrade, byte &byCount, word &wAttach);
	byte			 _MakeGvpRound3(byte &byGrade, byte &byCount, word &wAttach);
};

#define g_pRPlayerXJ			CRPlayerXJ::GetInstancePtr()

#endif
