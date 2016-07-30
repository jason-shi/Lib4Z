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

static void			 AddMoveRed(int src, int dest, int check);
static void			 AddKillRed(int src, int dest, int check);
static void			 AddMoveBlk(int src, int dest);
static void			 AddKillBlk(int src, int dest);

#include "MoveList.cpp"

#endif

#include "PRIVATE_NAMESPACE_END"
