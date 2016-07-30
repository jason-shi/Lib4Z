#include "PRIVATE_NAMESPACE_BEGIN"

#ifndef __Generate_h__
#define __Generate_h__

#define VERTICAL		 1
#define HORIZONTAL		 2

#define GetPxPy(pos, px, py) \
{ \
	px = pos & 0xF; \
	py = pos >> 4; \
}

static int			 pinmask; //˩��λͼ��1234��5��6��7��

//�ŷ����������
static void			 GenerateRed(bool bRejectCycle);
static void			 GenerateBlk(int check);

//˩�����
static bool			 IsPinnedByRed(int pos);

//���ڲ����췽�ŷ�
static void			 RookLeaveHly(int pos, int chk);
static void			 RookLeaveVly(int pos, int chk);
static void			 CannonLeaveHly(int pos, int chk);
static void			 CannonLeaveVly(int pos, int chk);
static void			 HorseLeaveXly(int pos, int chk);
static void			 BishopLeaveXly(int pos, int chk);
static void			 GuardLeaveXly(int pos, int chk);

//�췽�����ӽ��������
static void			 GenRookRed(int pos);
static void			 GenHorseRed(int pos);
static void			 GenCannonRed(int pos);
static void			 GenPawnRed();

//�ڷ�������ģʽ���
static void			 GenRookBlk(int check);
static void			 GenCannonBlk(int check);
static void			 GenEludeUnlimited(); //��ͱ���
static void			 GenEludeLimitedH(int limit); //������ˮƽ��
static void			 GenEludeLimitedV(int limit); //�����ڴ�ֱ��
static void			 GenBlockSingle(int single);
static void			 GenBlockRookH(int begin, int end);
static void			 GenBlockRookV(int begin, int end);
static void			 GenBlockCannonH(int begin, int end, int shell);
static void			 GenBlockCannonV(int begin, int end, int shell);
static void			 GenTakeChecker(int check);
static void			 GenShellEscapeH(int pos);
static void			 GenShellEscapeV(int pos);

#include "Generate.cpp"
#include "GenRed\RookRed.cpp"
#include "GenRed\HorseRed.cpp"
#include "GenRed\CannonRed.cpp"
#include "GenRed\SmallRed.cpp"
#include "GenBlk\GenBlk.cpp"
#include "GenBlk\TakeBlk.cpp"
#include "GenBlk\EscapeBlk.cpp"
#include "GenBlk\BlockBlk.cpp"
#include "GenBlk\EludeBlk.cpp"

#endif

#include "PRIVATE_NAMESPACE_END"
