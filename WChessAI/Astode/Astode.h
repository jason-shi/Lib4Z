#include "PRIVATE_NAMESPACE_BEGIN"

#ifndef __Astode_h__
#define __Astode_h__

//Astode������С
#define MaxBufferSize	 4096

#ifdef _DEBUG
#define MaxStoreSize	 1048576
#else
#define MaxStoreSize	 1048576 * 8
#endif

//Astodeʵ�ʴ�С
static int			 g_nBufferSize;
static int			 g_nStoreSize;

//Astodeָʾ��Astode����
static bool			 g_bAstable[MaxCrackDepth];
static int			 g_nBSIndex[MaxCrackDepth];

//Astode����ָʾ
static bool			 g_bAddAstode;

static bool			 MarkBuffer[MaxBufferSize] = { 0 };
static bool			 MarkStore[MaxStoreSize] = { 0 };

struct TAstode {
	qword		 data[3];
	int			 nLoseD;
	int			 nWinD;
	int			 nVagueD;
	int			 tag1;
	int			 tag2;
};

static TAstode		*AstodeBuffer[MaxBufferSize] = { 0 };
static TAstode		*AstodeStore[MaxStoreSize] = { 0 };
static TAstode		*TempStore[MaxStoreSize] = { 0 };

static int			 DataCompare(qword dA[5], qword dB[5]);

static void			 MergeBufferStore(int nStep);

//����ӿ�
static bool			 CreateAstode();
static void			 DeleteAstode();

static void			 InitAstode();
static void			 WhatAstode();
static int			 CallAstode(TAstode &tAstode, int nStep, int nRest);

static void			 ShrinkAstode(qword data[3]);
static void			 ExpandAstode(qword data[3]);

#include "Astode.cpp"
#include "AstodeMerge.cpp"

#endif

#include "PRIVATE_NAMESPACE_END"
