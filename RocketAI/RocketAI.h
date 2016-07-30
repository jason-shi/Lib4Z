#ifndef __RocketAI_h__
#define __RocketAI_h__

#define RR_NORMAL				0
#define RR_LIGUN				1
#define RR_JUEGUN				2

#define TYPE_GVP				0
#define TYPE_SINGLE				1
#define TYPE_PAIR				2
#define TYPE_BOMB				3
#define TYPE_CANNON				4
#define TYPE_REDFLAG			5
#define TYPE_ROCKET				6
#define TYPE_SHUNZI				7
#define TYPE_LIANDUI			8
#define TYPE_CHA				9
#define TYPE_GOU				10
#define TYPE_ERROR				11

#define GRADE_INIT				15

struct TLiGunParam {
	word			 bitmap[4];
};

struct TLiGunResult {
	bool			 bLiGun;
};

struct TJueGunParam {
	word			 bitmap[4];
};

struct TJueGunResult {
	byte			 bJueGun;
};

struct TCrackParam {
	byte			 bySituation;
	word			 bitmap[4][4];
	byte			 byWinBitmap;
	byte			 byChair1;
	byte			 byType;
	byte			 byRspType;
	byte			 byGrade;
	byte			 byCount;
	dword			 dwCrackDepth;
};

struct TCrackResult {
	byte			 byRspType;
	byte			 byGrade;
	byte			 byCount;
	byte			 byCrackScore;
};

void				 Rocket_Initial();
void				 Rocket_Release();
void				 Rocket_CallLiGun(TLiGunParam *pLiGunParam, TLiGunResult *pLiGunResult);
void				 Rocket_CallJueGun(TJueGunParam *pJueGunParam, TJueGunResult *pJueGunResult);
void				 Rocket_CallCrack(TCrackParam *pCrackParam, TCrackResult *pCrackResult);

#endif
