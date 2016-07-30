#include "PRIVATE_NAMESPACE_BEGIN"

#ifndef __Traversal_h__
#define __Traversal_h__

static bool			 g_bInterrupt; //�ж�ָʾ
static bool			 g_bFullAstode; //���ݿ���ָʾ
static int			 g_nFurtherEnd; //�ۺ�ָ��
static int			 g_nBoundLower; //���Ա�������

static void			 ForwardsRed();
static void			 ForwardsBlk();
static void			 BackwardsRed();
static void			 BackwardsBlk();

static int			 Traversal();

#include "Traversal.cpp"

#endif

#include "PRIVATE_NAMESPACE_END"
