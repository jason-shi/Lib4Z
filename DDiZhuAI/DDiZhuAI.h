#ifndef __DDiZhuAI_h__
#define __DDiZhuAI_h__

#define TYPE_GVP				0
#define TYPE_SINGLE				1
#define TYPE_PAIR				2
#define TYPE_TRIP				3
#define TYPE_TRIP_S				4
#define TYPE_TRIP_P				5
#define TYPE_PLANE				6
#define TYPE_PLANE_S			7
#define TYPE_PLANE_P			8
#define TYPE_QUAD_S				9
#define TYPE_QUAD_P				10
#define TYPE_SHUNZI				11
#define TYPE_LIANDUI			12
#define TYPE_BOMB				13
#define TYPE_ROCKET				14
#define TYPE_ERROR				15

#define GRADE_INIT				15

struct TScoreParam {
	word			 bitmap[4];
	byte			 byLastScore;
};

struct TScoreResult {
	byte			 byBidScore;
};

struct TCrackParam {
	word			 bitmap[3][4];
	byte			 byDiZhu;
	byte			 byChair1;
	byte			 byType;
	byte			 byRspType;
	byte			 byGrade;
	byte			 byCount;
	word			 wAttach;
	dword			 dwCrackDepth;
};

struct TCrackResult {
	byte			 byRspType;
	byte			 byGrade;
	byte			 byCount;
	word			 wAttach;
	byte			 byCrackScore;
};

void				 DDiZhu_Initial();
void				 DDiZhu_Release();
void				 DDiZhu_CallScore(TScoreParam *pScoreParam, TScoreResult *pScoreResult);
void				 DDiZhu_CallCrack(TCrackParam *pCrackParam, TCrackResult *pCrackResult);

#endif
