#ifndef __Common_h__
#define __Common_h__

#define CARD_2					(1 << 0)
#define CARD_3					(1 << 1)
#define CARD_4					(1 << 2)
#define CARD_5					(1 << 3)
#define CARD_6					(1 << 4)
#define CARD_7					(1 << 5)
#define CARD_8					(1 << 6)
#define CARD_9					(1 << 7)
#define CARD_10					(1 << 8)
#define CARD_J					(1 << 9)
#define CARD_Q					(1 << 10)
#define CARD_K					(1 << 11)
#define CARD_A					(1 << 12)

#define MASK_SHUNZI				(0x0FFF) //234567890JQK
#define MASK_A2					(CARD_A | CARD_2)
#define MASK_KA					(CARD_K | CARD_A)
#define MASK_A23				(CARD_A | CARD_2 | CARD_3)
#define MASK_QKA				(CARD_Q | CARD_K | CARD_A)

//特殊牌型（内部使用）
#define TYPE_TRI_SHUNZI_1S		0x7E //一堆同花顺
#define TYPE_TRI_SHUNZI_2S		0x7F //两堆同花顺

//分堆牌型
#define INNER_TYPE_GVP			0
#define INNER_TYPE_SINGLE		1
#define INNER_TYPE_SISTER		2
#define INNER_TYPE_KA			3
#define INNER_TYPE_A2			4
#define INNER_TYPE_PAIR			5
#define INNER_TYPE_SHUNZI		6
#define INNER_TYPE_A23			7
#define INNER_TYPE_QKA			8
#define INNER_TYPE_BOMB			9
#define INNER_TYPE_S_SHUNZI		10
#define INNER_TYPE_S_A23		11
#define INNER_TYPE_S_QKA		12

#define GRADE_INIT				13

#define WORD_BIT_COUNT(n)		(TblBitCount[(n) >> 8] + TblBitCount[(n) & 0xFF])

#endif
