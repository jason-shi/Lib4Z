#include "PRIVATE_NAMESPACE_BEGIN"

#ifndef __Table_h__
#define __Table_h__

static byte			*TblNumberOne; //二进制表达的1的个数
static byte			*TblFirstBit; //二进制表达的首位的位置
static byte			*TblSecondBit; //二进制表达的第二位的位置
static byte			*TblThirdBit; //二进制表达的第三位的位置
static byte			*TblLastBit; //二进制表达的末位的位置
static byte			*TblLagendBit; //二进制表达的倒二位的位置
static byte			*TblLathidBit; //二进制表达的倒三位的位置
static int			*TblHighMask; //二进制表达的高位掩码
static int			*TblLowMask; //二进制表达的低位掩码
static int			*TblBitrim; //直线上点到点的掩码
static bool			*TblSameLine; //盘面坐标点与点是否同线
static int			*TblHorseLeg; //盘面坐标起止点的马腿
static byte			*TblHrsChkIdx; //盘面坐标马与将之间的关系索引
static int			*TblHrsChk11; //一、直将起点与落点间的马腿
static int			*TblHrsChk12; //一、直将的落点
static int			*TblHrsChk13; //一、直将落点与终点间的马腿
static int			*TblHrsChk21; //二、直将起点与落点间的马腿
static int			*TblHrsChk22; //二、直将的落点
static int			*TblHrsChk23; //二、直将落点与终点间的马腿
static bool			*TblBishopSphere; //黑象的移动范围
static bool			*TblGuardSphere; //黑士的移动范围
static int			*TblPieceBitmap; //棋子合法位图
static int			*TblPinByRed; //红子相对于黑将的位置关系
static byte			*TblCharToPiece; //字符转换为棋子类型
static char			*TblPieceToChar; //棋子类型转换为字符
static int			*TblExclusion; //对应位的排除码
static byte			*TblConflictPawn; //双兵对应位

static void			 CreateAllTable();
static void			 DeleteAllTable();

#include "Table.cpp"

#endif

#include "PRIVATE_NAMESPACE_END"
