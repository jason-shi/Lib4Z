#include "PRIVATE_NAMESPACE_BEGIN"

#ifndef __CChess_h__
#define __CChess_h__

#define MaxMove					32 //最大着法数

#define Board(x, y)				board[(x) + ((y) << 4)]

struct TMove {
	int				 src;
	int				 dest;
	int				 check;
	TPiece			*pDeath;
	bool			 bValid;
};

static int			 board[VBH * VBW]; //棋盘10 * 9有效
static int			 bakboard[VBH * VBW];

static TPiece		*PieceRedR, *PieceRedH, *PieceRedC, *PieceRedS, *PieceRedK;
static TPiece		*PieceBlkB, *PieceBlkS, *PieceBlkK;

static TPiece		*g_hPBoard[VBH * VBW];

static int			 kpos, rkpos; //将帅
static int			 kx, ky;

static int			 kbtm, klft, krgt;
static int			 kb1, kb2, kb3, kl1, kl2, kl3, kr1, kr2, kr3;
static int			 vert[9], horz[10];

static int			 nStep;
static int			 nDepth;

static int			 g_nStartTick;
static int			 g_nActualDepth;

static TCrackParam	 g_tCrackParam;
static TCrackResult	 g_tCrackResult;

static void			 InitBoard();
static void			 FillBoardExtend();

static void			 UpdateKingFamily();
static void			 GetBoardDynData();

static bool			 InCheckRed();
static bool			 InCheckBlk();

static bool			 CalcCheckPos(int &nCheckPos);

#include "CChess.cpp"

#endif

#include "PRIVATE_NAMESPACE_END"
