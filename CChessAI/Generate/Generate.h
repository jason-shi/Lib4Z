#include "PRIVATE_NAMESPACE_BEGIN"

#ifndef __Generate_h__
#define __Generate_h__

#define VERTICAL		 1
#define HORIZONTAL		 2

#define GetPxPy(pos, px, py) \
{ \
	px = pos & 0xF; \
	py = pos >> 4; \
}

static int			 pinmask; //拴链位图：1234马5左6右7下

//着法生成器入口
static void			 GenerateRed(bool bRejectCycle);
static void			 GenerateBlk(int check);

//拴链检测
static bool			 IsPinnedByRed(int pos);

//用于产生红方着法
static void			 RookLeaveHly(int pos, int chk);
static void			 RookLeaveVly(int pos, int chk);
static void			 CannonLeaveHly(int pos, int chk);
static void			 CannonLeaveVly(int pos, int chk);
static void			 HorseLeaveXly(int pos, int chk);
static void			 BishopLeaveXly(int pos, int chk);
static void			 GuardLeaveXly(int pos, int chk);

//红方攻击子将军棋入口
static void			 GenRookRed(int pos);
static void			 GenHorseRed(int pos);
static void			 GenCannonRed(int pos);
static void			 GenPawnRed();

//黑方各防御模式入口
static void			 GenRookBlk(int check);
static void			 GenCannonBlk(int check);
static void			 GenEludeUnlimited(); //马和兵将
static void			 GenEludeLimitedH(int limit); //车和炮水平将
static void			 GenEludeLimitedV(int limit); //车和炮垂直将
static void			 GenBlockSingle(int single);
static void			 GenBlockRookH(int begin, int end);
static void			 GenBlockRookV(int begin, int end);
static void			 GenBlockCannonH(int begin, int end, int shell);
static void			 GenBlockCannonV(int begin, int end, int shell);
static void			 GenTakeChecker(int check);
static void			 GenShellEscapeH(int pos);
static void			 GenShellEscapeV(int pos);

#include "Generate.cpp"
#include "GenRed\RookRed.cpp"
#include "GenRed\HorseRed.cpp"
#include "GenRed\CannonRed.cpp"
#include "GenRed\SmallRed.cpp"
#include "GenBlk\GenBlk.cpp"
#include "GenBlk\TakeBlk.cpp"
#include "GenBlk\EscapeBlk.cpp"
#include "GenBlk\BlockBlk.cpp"
#include "GenBlk\EludeBlk.cpp"

#endif

#include "PRIVATE_NAMESPACE_END"
