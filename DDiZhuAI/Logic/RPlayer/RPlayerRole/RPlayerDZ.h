#ifndef __RPlayerDZ_h__
#define __RPlayerDZ_h__

#include "..\RPlayer.h"

class CRPlayerDZ : public CRPlayer {
  public:
	SINGLETON(CRPlayerDZ);
	~CRPlayerDZ();

  private:
	CRPlayerDZ();

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

#define g_pRPlayerDZ			CRPlayerDZ::GetInstancePtr()

#endif
