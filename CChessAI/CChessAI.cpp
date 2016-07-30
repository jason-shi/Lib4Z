#include "StdAfx.h"
#include "Crack\Crack.h"

bool CChess_Initial()
{
	return g_pZCrack->Initial();
}

void CChess_Release()
{
	return g_pZCrack->Release();
}

void CChess_SetWindow(CWindow *_window)
{
	g_pZCrack->SetWindow(_window);
}

void CChess_SetMsgId(UINT uMsgId)
{
	g_pZCrack->SetMsgId(uMsgId);
}

bool CChess_InitThreadRFCrack(TCrackParam *pCrackParam)
{
	return g_pZCrack->InitThreadRFCrack(pCrackParam);
}

bool CChess_InitThreadBFCrack(TCrackParam *pCrackParam)
{
	return g_pZCrack->InitThreadBFCrack(pCrackParam);
}

void CChess_WaitForThreadCrack()
{
	g_pZCrack->WaitForThreadCrack();
}

void CChess_ExitThreadCrack()
{
	g_pZCrack->ExitThreadCrack();
}

int CChess_GetThinkDepth()
{
	return g_pZCrack->GetThinkDepth();
}

int CChess_GetAstodeSize()
{
	return g_pZCrack->GetAstodeSize();
}

int CChess_GetSpendTime()
{
	return g_pZCrack->GetSpendTime();
}

TCrackResult& CChess_GetCrackResult()
{
	return g_pZCrack->GetCrackResult();
}
