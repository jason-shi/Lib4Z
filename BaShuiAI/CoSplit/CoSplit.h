#ifndef __CoSplit_h__
#define __CoSplit_h__

#include "..\Split\Split.h"

class CCoSplit : public CSplit {
  public:
	CCoSplit();
	~CCoSplit();

  public:
	dword			 ForceSplit(dword &dwLimitScore, byte byCard[8], word wFlushBitmap);

  protected:
	void			 TransToOuterType(byte byType, byte &byOuterType);
	bool			 IsTypeMatch(VECBYTE &vecOuterType, byte byPlacedCard[8]);
	void			 SwapCard(byte byCard[8], int nIndex1, int nIndex2);
	bool			 SwapSingleCard(dword &dwLimitScore, VECBYTE &vecOuterType, byte byPlacedCard[8]);
	void			 CollectSwapIndex(VECBYTE &vecSwapIndex, const VECBYTE &vecOuterType);

  protected:
	dword			 SplitCards(dword &dwLimitScore, byte byCard[8], word wFlushBitmap);
};

inline void CCoSplit::SwapCard(byte byCard[8], int nIndex1, int nIndex2)
{
	byte byTemp = byCard[nIndex1];
	byCard[nIndex1] = byCard[nIndex2];
	byCard[nIndex2] = byTemp;
}

#endif
