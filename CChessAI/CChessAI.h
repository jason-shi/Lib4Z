#ifndef __CChessAI_h__
#define __CChessAI_h__

#define VBW						16 //虚拟棋盘宽度
#define VBH						11 //虚拟棋盘高度

#define CLGRID					160 //无效格

enum EPieceKind { NONE,
	R_ROOK, R_HORSE, R_BISHOP, R_GUARD, R_CANNON, R_PAWN, R_KING,
	B_ROOK, B_HORSE, B_BISHOP, B_GUARD, B_CANNON, B_PAWN, B_KING,
	STONE = -1,
};

#define MaxCrackDepth			600

//破解结果公共定义
#define NOANSWER				0
#define REDLOSE					1
#define BLKLOSE					2
#define NOLOSE					3
#define INTERRUPT				4
#define FURTHER					5
#define NOFURTHER				6
#define FULLASTODE				7
#define TIMEOUT					8
#define BADFILE					9

struct TPiece {
	int				 kind;
	int				 pos;
	TPiece			*next;
	TPiece			*prev;
};

struct TCrackParam {
	int				*board;
	bool			 bPesudo;
	int				 nMaxDepth;
	int				 nFurther;
	int				 src;
	int				 dest;
};

struct TCrackResult {
	int				 nRound;
	int				 nAstodeSize;
	int				 nSpendTime;
	int				 src;
	int				 dest;
};

bool				 CChess_Initial();
void				 CChess_Release();
void				 CChess_SetWindow(CWindow *_window);
void				 CChess_SetMsgId(UINT uMsgId);


bool				 CChess_InitThreadRFCrack(TCrackParam *pCrackParam);
bool				 CChess_InitThreadBFCrack(TCrackParam *pCrackParam);
void				 CChess_WaitForThreadCrack();
void				 CChess_ExitThreadCrack();

int					 CChess_GetThinkDepth();
int					 CChess_GetAstodeSize();
int					 CChess_GetSpendTime();
TCrackResult&		 CChess_GetCrackResult();

#endif
