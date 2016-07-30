#include "PRIVATE_NAMESPACE_BEGIN"

#ifndef __Table_h__
#define __Table_h__

static int			*TblBitIndex; //二进制表达的1的位置
static int			*TblPieceBitmap; //棋子合法位图
static byte			*TblCharToPiece; //字符转换为棋子类型
static char			*TblPieceToChar; //棋子类型转换为字符
static qword		*TblQueenPath; //后的路径掩码
static qword		*TblRookPath; //车的路径掩码
static qword		*TblBishopPath; //相的路径掩码
static int			*TblDirection; //方向
static byte			*TblRookCrossGrid[2]; //车交叉点
static byte			*TblBishopCrossGrid[2]; //相交叉点
static qword		*TblRookCrossPath[2]; //车交叉路径
static qword		*TblBishopCrossPath[2]; //相交叉路径
static qword		*TblQueenCrossGridBtmp; //后交叉点位图
static byte			*TblHorseChkGrid1; //一、直将的落点
static byte			*TblHorseChkGrid2; //二、直将的落点
static byte			*TblHorseChkIdx; //盘面坐标马与将之间的关系索引
static byte			*TblKingChkIdx; //盘面坐标将与将之间的关系索引
static byte			*TblEludeChkIdx;
static byte			*TblRookBlock1;
static byte			*TblRookBlock2;
static byte			*TblHorseBlock1;
static byte			*TblHorseBlock2;
static byte			*TblBishopBlock1;
static byte			*TblBishopBlock2;
static byte			*TblBlkPawnBlock;
static byte			*TblBlkPawnBlock2;
static byte			*TblHorseMove;
static byte			*TblKingMove;
static byte			*TblKingAway;
static byte			*TblMaxKingAway;

static void			 CreateAllTable();
static void			 DeleteAllTable();

#include "Table.cpp"

#endif

#include "PRIVATE_NAMESPACE_END"
