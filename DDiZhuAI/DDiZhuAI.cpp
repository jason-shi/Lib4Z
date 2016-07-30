#include "StdAfx.h"
#include "DDiZhuAI.h"

void DDiZhu_Initial()
{
#ifndef _DEBUG
	//���ʱ������
	srand((int)time(NULL));
#endif

	CreateAllTables();
	g_pRCrack->Initial();
}

void DDiZhu_Release()
{
	DeleteAllTables();
}

void DDiZhu_CallScore(TScoreParam *pScoreParam, TScoreResult *pScoreResult)
{
	g_pRSpecial->CallScore(pScoreParam, pScoreResult);
}

void DDiZhu_CallCrack(TCrackParam *pCrackParam, TCrackResult *pCrackResult)
{
	g_pRCrack->CallCrack(pCrackParam, pCrackResult);
}
