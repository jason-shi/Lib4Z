#ifndef __AN3D1AI_h__
#define __AN3D1AI_h__

#define RR_NORMAL				0
#define RR_BANKER				1
#define RR_MIEDI				2
#define RR_TRUMP				3

#define TYPE_GVP				0
#define TYPE_SPADE				1
#define TYPE_HEART				2
#define TYPE_DIAMOND			3
#define TYPE_CLUB				4
#define TYPE_ERROR				5

#define GRADE_INIT				16

struct TScoreParam {
	word			 bitmap[4];
	byte			 byFuTwo;
	byte			 byLastScore;
};

struct TScoreResult {
	byte			 byBidScore;
};

struct TTrumpParam {
	word			 bitmap[4];
	byte			 byFuTwo;
};

struct TTrumpResult {
	byte			 byTrump;
};

struct TMieDiParam {
	word			 bitmap[4];
	byte			 byFuTwo;
};

struct TMieDiResult {
	word			 bitmap[4];
	byte			 byFuTwo;
};

struct TCrackParam {
	byte			 byBanker;
	struct TPlayerParam {
		word		 bitmap[4];
		byte		 byFuTwo;
	}				 tPlayerParam[4];
	struct TBattleParam {
		byte		 byType;
		byte		 byRspType;
		byte		 byGrade;
		byte		 byChair1;
		byte		 byChairM;
		byte		 byTypeM;
		byte		 byGradeM;
		byte		 byCurr5TK;
	}				 tBattleParam;
	dword			 dwCrackDepth;
};

struct TCrackResult {
	byte			 byRspType;
	byte			 byGrade;
	byte			 byCrackScore;
};

void				 AN3D1_Initial();
void				 AN3D1_Release();
void				 AN3D1_CallScore(TScoreParam *pScoreParam, TScoreResult *pScoreResult);
void				 AN3D1_CallTrump(TTrumpParam *pTrumpParam, TTrumpResult *pTrumpResult);
void				 AN3D1_CallMieDi(TMieDiParam *pMieDiParam, TMieDiResult *pMieDiResult);
void				 AN3D1_CallCrack(TCrackParam *pCrackParam, TCrackResult *pCrackResult);

#endif
