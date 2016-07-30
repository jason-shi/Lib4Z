#ifndef __Table_h__
#define __Table_h__

static int TblBitIndex[37] = { 0, 0, 1, 26, 2, 23, 27, 0, 3, 16, 24, 30, 28, 11, 0, 13, 4, 7, 17,
								0, 25, 22, 31, 15, 29, 10, 12, 6, 0, 21, 14, 9, 5, 20, 8, 19, 18, };

extern byte			*TblBitCount;
extern word			*TblShunZi2;
extern word			*TblShunZi3;
extern word			*TblShunZi5;
extern word			*TblSanShun;
extern word			*TblFiveShun;
extern word			*TblShun5PrhL;
extern word			*TblShun5PrhH;
extern word			*TblShun3PrhH;
extern word			*TblShun5PrhS;
extern word			*TblShun3PrhS;
extern word			*TblShun5PrhE;
extern word			*TblShun3PrhE;
extern word			*TblMiddlePrh;
extern word			*TblContraryMask;

void				 CreateAllTables();
void				 DeleteAllTables();

#endif
