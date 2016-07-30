#include "StdAfx.h"
#include "RocketAI.h"

void Rocket_Initial()
{
#ifndef _DEBUG
	//随机时间种子
	srand((int)time(NULL));
#endif

	//创建速查表
	CreateAllTable();

	g_pRCrack4->Initial();
	g_pRCrack3->Initial();
	g_pRCrack2->Initial();
}

void Rocket_Release()
{
	DeleteAllTable();
}

void Rocket_CallLiGun(TLiGunParam *pLiGunParam, TLiGunResult *pLiGunResult)
{
	g_pRSpecial->CallLiGun(pLiGunParam, pLiGunResult);
}

void Rocket_CallJueGun(TJueGunParam *pJueGunParam, TJueGunResult *pJueGunResult)
{
	g_pRSpecial->CallJueGun(pJueGunParam, pJueGunResult);
}

void Rocket_CallCrack(TCrackParam *pCrackParam, TCrackResult *pCrackResult)
{
	switch (pCrackParam->bySituation)
	{
	case RR_NORMAL:
		g_pRCrack4->CallCrack(pCrackParam, pCrackResult);
		break;
	case RR_LIGUN:
		g_pRCrack3->CallCrack(pCrackParam, pCrackResult);
		break;
	case RR_JUEGUN:
		g_pRCrack2->CallCrack(pCrackParam, pCrackResult);
		break;
	}
}
