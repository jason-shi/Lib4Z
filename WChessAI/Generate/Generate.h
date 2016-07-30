#include "PRIVATE_NAMESPACE_BEGIN"

#ifndef __Generate_h__
#define __Generate_h__

#define VERTICAL		 1
#define HORIZONTAL		 2

#define GetPxPy(pos, px, py) \
{ \
	px = pos & 7; \
	py = pos >> 3; \
}

static qword		 pinmask; //˩��λͼ��5��6��7��

//�ŷ����������
static void			 GenerateRed(bool bRejectCycle);
static void			 GenerateBlk(int check, int away);

//˩�����
static bool			 IsPinnedByRed(int pos);

//���ڲ����췽�ŷ�
static void			 RookLeaveHly(int pos, int chk);
static void			 RookLeaveVly(int pos, int chk);
static void			 HorseLeaveXly(int pos, int chk);
static void			 BishopLeaveSly(int pos, int chk);
static void			 BishopLeaveBly(int pos, int chk);
static void			 KingLeaveXly(int pos, int chk, int nIndex);

//�췽�����ӽ��������
static void			 GenRookRed(int pos);
static void			 GenHorseRed(int pos);
static void			 GenBishopRed(int pos);
static void			 GenQueenRed(int pos);
static void			 GenPawnRed(int pos);

//�ڷ�������ģʽ���
static void			 GenBlockLine(int check);
static void			 GenEludeLimited(int check, int away);
static void			 GenTakeChecker(int check);

#include "Generate.cpp"
#include "GenRed\RookRed.cpp"
#include "GenRed\HorseRed.cpp"
#include "GenRed\BishopRed.cpp"
#include "GenRed\QueenRed.cpp"
#include "GenRed\PawnRed.cpp"
#include "GenRed\KingRed.cpp"
#include "GenBlk\TakeBlk.cpp"
#include "GenBlk\BlockBlk.cpp"
#include "GenBlk\EludeBlk.cpp"

#endif

#include "PRIVATE_NAMESPACE_END"
