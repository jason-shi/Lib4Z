#include "StdAfx.h"
#include "Crack\Crack.h"

bool WChess_Initial()
{
	return g_pZCrack->Initial();
}

void WChess_Release()
{
	return g_pZCrack->Release();
}

void WChess_SetWindow(CWindow *_window)
{
	g_pZCrack->SetWindow(_window);
}

void WChess_SetMsgId(UINT uMsgId)
{
	g_pZCrack->SetMsgId(uMsgId);
}

bool WChess_InitThreadRFCrack(TCrackParam *pCrackParam)
{
	return g_pZCrack->InitThreadRFCrack(pCrackParam);
}

bool WChess_InitThreadBFCrack(TCrackParam *pCrackParam)
{
	return g_pZCrack->InitThreadBFCrack(pCrackParam);
}

void WChess_WaitForThreadCrack()
{
	g_pZCrack->WaitForThreadCrack();
}

void WChess_ExitThreadCrack()
{
	g_pZCrack->ExitThreadCrack();
}

int WChess_GetThinkDepth()
{
	return g_pZCrack->GetThinkDepth();
}

int WChess_GetAstodeSize()
{
	return g_pZCrack->GetAstodeSize();
}

int WChess_GetSpendTime()
{
	return g_pZCrack->GetSpendTime();
}

TCrackResult& WChess_GetCrackResult()
{
	return g_pZCrack->GetCrackResult();
}
