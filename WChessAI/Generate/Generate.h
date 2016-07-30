#include "PRIVATE_NAMESPACE_BEGIN"

#ifndef __Generate_h__
#define __Generate_h__

#define VERTICAL		 1
#define HORIZONTAL		 2

#define GetPxPy(pos, px, py) \
{ \
	px = pos & 7; \
	py = pos >> 3; \
}

static qword		 pinmask; //拴链位图：5左6右7下

//着法生成器入口
static void			 GenerateRed(bool bRejectCycle);
static void			 GenerateBlk(int check, int away);

//拴链检测
static bool			 IsPinnedByRed(int pos);

//用于产生红方着法
static void			 RookLeaveHly(int pos, int chk);
static void			 RookLeaveVly(int pos, int chk);
static void			 HorseLeaveXly(int pos, int chk);
static void			 BishopLeaveSly(int pos, int chk);
static void			 BishopLeaveBly(int pos, int chk);
static void			 KingLeaveXly(int pos, int chk, int nIndex);

//红方攻击子将军棋入口
static void			 GenRookRed(int pos);
static void			 GenHorseRed(int pos);
static void			 GenBishopRed(int pos);
static void			 GenQueenRed(int pos);
static void			 GenPawnRed(int pos);

//黑方各防御模式入口
static void			 GenBlockLine(int check);
static void			 GenEludeLimited(int check, int away);
static void			 GenTakeChecker(int check);

#include "Generate.cpp"
#include "GenRed\RookRed.cpp"
#include "GenRed\HorseRed.cpp"
#include "GenRed\BishopRed.cpp"
#include "GenRed\QueenRed.cpp"
#include "GenRed\PawnRed.cpp"
#include "GenRed\KingRed.cpp"
#include "GenBlk\TakeBlk.cpp"
#include "GenBlk\BlockBlk.cpp"
#include "GenBlk\EludeBlk.cpp"

#endif

#include "PRIVATE_NAMESPACE_END"
