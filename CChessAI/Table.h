#include "PRIVATE_NAMESPACE_BEGIN"

#ifndef __Table_h__
#define __Table_h__

static byte			*TblNumberOne; //�����Ʊ���1�ĸ���
static byte			*TblFirstBit; //�����Ʊ�����λ��λ��
static byte			*TblSecondBit; //�����Ʊ��ĵڶ�λ��λ��
static byte			*TblThirdBit; //�����Ʊ��ĵ���λ��λ��
static byte			*TblLastBit; //�����Ʊ���ĩλ��λ��
static byte			*TblLagendBit; //�����Ʊ��ĵ���λ��λ��
static byte			*TblLathidBit; //�����Ʊ��ĵ���λ��λ��
static int			*TblHighMask; //�����Ʊ��ĸ�λ����
static int			*TblLowMask; //�����Ʊ��ĵ�λ����
static int			*TblBitrim; //ֱ���ϵ㵽�������
static bool			*TblSameLine; //�������������Ƿ�ͬ��
static int			*TblHorseLeg; //����������ֹ�������
static byte			*TblHrsChkIdx; //�����������뽫֮��Ĺ�ϵ����
static int			*TblHrsChk11; //һ��ֱ����������������
static int			*TblHrsChk12; //һ��ֱ�������
static int			*TblHrsChk13; //һ��ֱ��������յ�������
static int			*TblHrsChk21; //����ֱ����������������
static int			*TblHrsChk22; //����ֱ�������
static int			*TblHrsChk23; //����ֱ��������յ�������
static bool			*TblBishopSphere; //������ƶ���Χ
static bool			*TblGuardSphere; //��ʿ���ƶ���Χ
static int			*TblPieceBitmap; //���ӺϷ�λͼ
static int			*TblPinByRed; //��������ںڽ���λ�ù�ϵ
static byte			*TblCharToPiece; //�ַ�ת��Ϊ��������
static char			*TblPieceToChar; //��������ת��Ϊ�ַ�
static int			*TblExclusion; //��Ӧλ���ų���
static byte			*TblConflictPawn; //˫����Ӧλ

static void			 CreateAllTable();
static void			 DeleteAllTable();

#include "Table.cpp"

#endif

#include "PRIVATE_NAMESPACE_END"
