#include "StdAfx.h"
#include "AN3D1AI.h"

void AN3D1_Initial()
{
#ifndef _DEBUG
	//随机时间种子
	srand((int)time(NULL));
#endif

	CreateAllTables();
	g_pRCrack->Initial();
}

void AN3D1_Release()
{
	DeleteAllTables();
}

void AN3D1_CallScore(TScoreParam *pScoreParam, TScoreResult *pScoreResult)
{
	g_pRSpecial->CallScore(pScoreParam, pScoreResult);
}

void AN3D1_CallTrump(TTrumpParam *pTrumpParam, TTrumpResult *pTrumpResult)
{
	g_pRSpecial->CallTrump(pTrumpParam, pTrumpResult);
}

void AN3D1_CallMieDi(TMieDiParam *pMieDiParam, TMieDiResult *pMieDiResult)
{
	g_pRSpecial->CallMieDi(pMieDiParam, pMieDiResult);
}

void AN3D1_CallCrack(TCrackParam *pCrackParam, TCrackResult *pCrackResult)
{
	g_pRCrack->CallCrack(pCrackParam, pCrackResult);
}
