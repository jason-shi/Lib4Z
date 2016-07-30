#ifndef __WChessAI_h__
#define __WChessAI_h__

#define VBW						8 //棋盘宽度
#define VBH						8 //棋盘高度

enum EPieceKind { NONE,
	R_ROOK, R_HORSE, R_BISHOP, R_QUEEN, R_PAWN, R_KING,
	B_ROOK, B_HORSE, B_BISHOP, B_QUEEN, B_PAWN, B_KING,
};

#define MaxCrackDepth			100

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
	int				 bian;
};

struct TCrackResult {
	int				 nRound;
	int				 nAstodeSize;
	int				 nSpendTime;
	int				 src;
	int				 dest;
	int				 bian;
};

bool				 WChess_Initial();
void				 WChess_Release();
void				 WChess_SetWindow(CWindow *_window);
void				 WChess_SetMsgId(UINT uMsgId);


bool				 WChess_InitThreadRFCrack(TCrackParam *pCrackParam);
bool				 WChess_InitThreadBFCrack(TCrackParam *pCrackParam);
void				 WChess_WaitForThreadCrack();
void				 WChess_ExitThreadCrack();

int					 WChess_GetThinkDepth();
int					 WChess_GetAstodeSize();
int					 WChess_GetSpendTime();
TCrackResult&		 WChess_GetCrackResult();

#endif
