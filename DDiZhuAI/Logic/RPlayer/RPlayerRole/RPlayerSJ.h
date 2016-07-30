#ifndef __RPlayerSJ_h__
#define __RPlayerSJ_h__

#include "..\RPlayer.h"

class CRPlayerSJ : public CRPlayer {
  public:
	SINGLETON(CRPlayerSJ);
	~CRPlayerSJ();

  private:
	CRPlayerSJ();

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

#define g_pRPlayerSJ			CRPlayerSJ::GetInstancePtr()

#endif
