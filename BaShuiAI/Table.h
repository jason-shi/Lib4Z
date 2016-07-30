#ifndef __Table_h__
#define __Table_h__

static int T3[27] = { 0, 3, 6, 9, 12, 15, 18, 21, 24, 27, 30, 33, 36, 39, 42, 45, 48, 51, 54, 57, 60, 63, 66, 69, 72, 75, 78, };

static int TblBitIndex[37] = { 0, 0, 1, 26, 2, 23, 27, 0, 3, 16, 24, 30, 28, 11, 0, 13, 4, 7, 17,
								0, 25, 22, 31, 15, 29, 10, 12, 6, 0, 21, 14, 9, 5, 20, 8, 19, 18, };

extern word			*TblContraryMask;
extern byte			*TblBitCount;
extern byte			*TblMaxGrade;
extern word			*TblSister; //23456789TJQK（不包含A）
extern word			*TblShunZi; //23456789TJQK（不包含A）
extern word			*TblSanShun;
extern byte			*TblSortCard;
extern dword		*TblFastCompare;
extern dword		*TblFastCompareNT;
extern dword		*TblFastSplit; //8次平升跳 0:平 1:升 2:跳 3:同花顺
extern dword		*TblCoFastSplit[8];

void				 CreateAllTables();
void				 DeleteAllTables();

#endif
