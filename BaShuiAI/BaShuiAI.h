#ifndef __BaShuiAI_h__
#define __BaShuiAI_h__

#define TotalPlayer				4

//特殊牌型
#define TYPE_NORMAL				0
#define TYPE_S1_SHUNZI			1
#define TYPE_DBL_BOMB			2
#define TYPE_S3_SHUNZI			3
#define TYPE_ONE_SHUNZI			4
#define TYPE_ONE_BOMB			5
#define TYPE_FOUR_PAIR			6
#define TYPE_TRI_SHUNZI			7

//分堆牌型
#define TYPE_NONE				0
#define TYPE_SINGLE				1
#define TYPE_PAIR				2
#define TYPE_SHUNZI				3
#define TYPE_BOMB				4
#define TYPE_S_SHUNZI			5

struct TCrackParam {
	byte			 byCard[8];
};

struct TCrackResult {
	byte			 bySpecType;
	byte			 byType[3];
	byte			 byCard[8];
};

struct TCoCrackParam {
	byte			 byCard[TotalPlayer - 1][8];
	byte			 byParam1;
	byte			 byParam2;
	byte			 byParam3;
	byte			 byParam4;
};

struct TCoCrackResult {
	byte			 bySpecType[TotalPlayer - 1];
	byte			 byType[TotalPlayer - 1][3];
	byte			 byCard[TotalPlayer - 1][8];
	byte			 byCoordinate; //0：没有合作 1：合作失败 2：合作成功
};

void				 BaShui_Initial();
void				 BaShui_Release();
void				 BaShui_CallCrack(const TCrackParam* pCrackParam, TCrackResult *pCrackResult);
void				 BaShui_CallCoCrack(const TCoCrackParam* pCoCrackParam, TCoCrackResult *pCoCrackResult);
int					 BaShui_ComparePile(int nIndex1, int nIndex2);

#endif
