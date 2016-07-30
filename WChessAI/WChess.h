#include "PRIVATE_NAMESPACE_BEGIN"

#ifndef __WChess_h__
#define __WChess_h__

#define MaxMove					160 //最大着法数

#define Board(x, y)				board[(x) + ((y) << 3)]

struct TMove {
	int				 src;
	int				 dest;
	int				 check;
	int				 bian;
	TPiece			*pDeath;
	bool			 bValid;
};

static int			 board[65];
static int			 bakboard[65];

static TPiece		*PieceRedB, *PieceRedK;
static TPiece		*PieceBlkB, *PieceBlkK;

static TPiece		*g_hPBoard[65];

static int			 kpos, rkpos, _kpos; //将帅
static qword		 Z;

static int			 nStep;
static int			 nDepth;

static int			 g_nStartTick;
static int			 g_nActualDepth;

static TCrackParam	 g_tCrackParam;
static TCrackResult	 g_tCrackResult;

static void			 InitBoard();
static void			 GetBoardDynData();

static bool			 InCheckRed();
static bool			 InCheckBlk();

static bool			 CalcCheckPos(int &nCheckPos);

#include "WChess.cpp"

#endif

#include "PRIVATE_NAMESPACE_END"
