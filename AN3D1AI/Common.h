#ifndef __Common_h__
#define __Common_h__

#define CARD_3					(1 << 0)
#define CARD_4					(1 << 1)
#define CARD_5					(1 << 2)
#define CARD_6					(1 << 3)
#define CARD_7					(1 << 4)
#define CARD_8					(1 << 5)
#define CARD_9					(1 << 6)
#define CARD_10					(1 << 7)
#define CARD_J					(1 << 8)
#define CARD_Q					(1 << 9)
#define CARD_K					(1 << 10)
#define CARD_A					(1 << 11)
#define CARD_2					(1 << 12)
#define CARD_SK					(1 << 13)
#define CARD_BK					(1 << 14)

#define MASK_5					0x0004
#define MASK_T					0x0080
#define MASK_K					0x0400
#define MASK_TK					(MASK_T | MASK_K)
#define MASK_5TK				(MASK_5 | MASK_T | MASK_K)

#define SCORE_1					1
#define SCORE_2					2
#define SCORE_3					3
#define SCORE_UNK				4
#define INTERRUPT				5

#define CONTRACT_FAIL			0
#define CONTRACT_UNK			1
#define CONTRACT_YES			2
#define CONTRACT_BKT			3

#define WORD_BIT_COUNT(n)		(TblBitCount[(n) >> 8] + TblBitCount[(n) & 0xFF])

extern dword		 g_dwCrackDepth;

#endif
