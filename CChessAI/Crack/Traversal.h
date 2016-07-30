#include "PRIVATE_NAMESPACE_BEGIN"

#ifndef __Traversal_h__
#define __Traversal_h__

static bool			 g_bInterrupt; //中断指示
static bool			 g_bFullAstode; //数据库满指示
static int			 g_nFurtherEnd; //综合指数
static int			 g_nBoundLower; //弹性遍历下限

static void			 ForwardsRed();
static void			 ForwardsBlk();
static void			 BackwardsRed();
static void			 BackwardsBlk();

static int			 Traversal();

#include "Traversal.cpp"

#endif

#include "PRIVATE_NAMESPACE_END"
