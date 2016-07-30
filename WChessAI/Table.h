#include "PRIVATE_NAMESPACE_BEGIN"

#ifndef __Table_h__
#define __Table_h__

static int			*TblBitIndex; //�����Ʊ���1��λ��
static int			*TblPieceBitmap; //���ӺϷ�λͼ
static byte			*TblCharToPiece; //�ַ�ת��Ϊ��������
static char			*TblPieceToChar; //��������ת��Ϊ�ַ�
static qword		*TblQueenPath; //���·������
static qword		*TblRookPath; //����·������
static qword		*TblBishopPath; //���·������
static int			*TblDirection; //����
static byte			*TblRookCrossGrid[2]; //�������
static byte			*TblBishopCrossGrid[2]; //�ཻ���
static qword		*TblRookCrossPath[2]; //������·��
static qword		*TblBishopCrossPath[2]; //�ཻ��·��
static qword		*TblQueenCrossGridBtmp; //�󽻲��λͼ
static byte			*TblHorseChkGrid1; //һ��ֱ�������
static byte			*TblHorseChkGrid2; //����ֱ�������
static byte			*TblHorseChkIdx; //�����������뽫֮��Ĺ�ϵ����
static byte			*TblKingChkIdx; //�������꽫�뽫֮��Ĺ�ϵ����
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
