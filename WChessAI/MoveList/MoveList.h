#include "PRIVATE_NAMESPACE_BEGIN"

#ifndef __Move_h__
#define __Move_h__

static TMove		*MoveList[MaxCrackDepth][MaxMove] = { 0 };

static int			 Trial[MaxCrackDepth];
static int			 Taken[MaxCrackDepth];
static int			 trial[MaxCrackDepth];

static std::vector<TMove>		 vecTestMove;


static bool			 CreateMoveList();
static void			 DeleteMoveList();

static void			 AddMoveRed(int src, int dest, int check, int bian = 0);
static void			 AddKillRed(int src, int dest, int check, int bian = 0);
static void			 AddMoveBlk(int src, int dest, int bian = 0);
static void			 AddKillBlk(int src, int dest, int bian = 0);

#include "MoveList.cpp"

#endif

#include "PRIVATE_NAMESPACE_END"
