#include "PRIVATE_NAMESPACE_BEGIN"

#ifndef __Astode_h__
#define __Astode_h__

//Astode容量大小
#define MaxBufferSize	 4096

#ifdef _DEBUG
#define MaxStoreSize	 65536
#else
#define MaxStoreSize	 1048576 * 8
#endif

//Astode实际大小
static int			 g_nBufferSize;
static int			 g_nStoreSize;

//Astode指示，Astode索引
static bool			 g_bAstable[MaxCrackDepth];
static int			 g_nBSIndex[MaxCrackDepth];

//Astode新增指示
static bool			 g_bAddAstode;

static bool			 MarkBuffer[MaxBufferSize] = { 0 };
static bool			 MarkStore[MaxStoreSize] = { 0 };

struct TAstode {
	int			 data[8];
	int			 nLoseD;
	int			 nWinD;
	int			 nVagueD;
	int			 tag1;
	int			 tag2;
};

static TAstode		*AstodeBuffer[MaxBufferSize] = { 0 };
static TAstode		*AstodeStore[MaxStoreSize] = { 0 };
static TAstode		*TempStore[MaxStoreSize] = { 0 };

static int			 DataCompare(int dA[8], int dB[8]);

static void			 MergeBufferStore(int nStep);

//对外接口
static bool			 CreateAstode();
static void			 DeleteAstode();

static void			 InitAstode();
static int			 CallAstode(TAstode &tAstode, int nStep, int nRest);

static void			 ShrinkAstodeRed(int data[8]);
static void			 ShrinkAstodeBlk(int data[8]);
static void			 ExpandAstode(int data[8]);

#include "Astode.cpp"
#include "AstodeMerge.cpp"

#endif

#include "PRIVATE_NAMESPACE_END"
