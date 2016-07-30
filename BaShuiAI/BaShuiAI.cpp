#include "StdAfx.h"
#include "BaShuiAI.h"

void BaShui_Initial()
{
	CreateAllTables();
	g_pRCrack->Initial();
}

void BaShui_Release()
{
	DeleteAllTables();
}

void BaShui_CallCrack(const TCrackParam* pCrackParam, TCrackResult *pCrackResult)
{
	g_pRCrack->CallCrack(pCrackParam, pCrackResult);
}

void BaShui_CallCoCrack(const TCoCrackParam* pCoCrackParam, TCoCrackResult *pCoCrackResult)
{
	g_pCoCrack->CallCrack(pCoCrackParam, pCoCrackResult);
}

int BaShui_ComparePile(int nIndex1, int nIndex2)
{
	return g_pRCrack->ComparePile(nIndex1, nIndex2);
}
