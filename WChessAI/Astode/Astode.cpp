#include "StdAfx.h"
#include "Astode.h"

#define GetByte0(v)	 (v & 0xFF)
#define GetByte1(v)	 ((v & 0xFF00) >> 8)
#define GetByte2(v)	 ((v & 0xFF0000) >> 16)
#define GetByte3(v)	 ((unsigned)v >> 24)

bool CreateAstode()
{
	for (int i = 0; i < MaxBufferSize; i++)
	{
		AstodeBuffer[i] = new TAstode;
		if (!AstodeBuffer[i])
			return false;
	}
	for (int i = 0; i < MaxStoreSize; i++)
	{
		AstodeStore[i] = new TAstode;
		if (!AstodeStore[i])
			return false;
	}
	for (int i = 0; i < MaxStoreSize; i++)
	{
		TempStore[i] = new TAstode;
		if (!TempStore[i])
			return false;
	}
	return true;
}

void DeleteAstode()
{
	for (int i = 0; i < MaxBufferSize; i++)
	{
		delete AstodeBuffer[i];
		AstodeBuffer[i] = NULL;
	}
	for (int i = 0; i < MaxStoreSize; i++)
	{
		delete AstodeStore[i];
		AstodeStore[i] = NULL;
	}
	for (int i = 0; i < MaxStoreSize; i++)
	{
		delete TempStore[i];
		TempStore[i] = NULL;
	}
}

void InitAstode()
{
	g_nBufferSize = 0;
	g_nStoreSize = 0;
	memset(g_bAstable, 0, sizeof(g_bAstable));
	memset(g_nBSIndex, 0, sizeof(g_nBSIndex));
}

void WhatAstode()
{
	for (int i = 0; i < g_nBufferSize; i++)
	{
		TAstode *pAstode = AstodeBuffer[i];
		if (pAstode->tag1 != REDLOSE && pAstode->tag1 != NOFURTHER)
		{
			pAstode->nLoseD = 0;
			pAstode->nWinD = MaxCrackDepth;
			pAstode->nVagueD = 0;
			//pAstode->tag1 = NOANSWER;
		}
	}
	for (int i = 0; i < g_nStoreSize; i++)
	{
		TAstode *pAstode = AstodeStore[i];
		if (pAstode->tag1 != REDLOSE && pAstode->tag1 != NOFURTHER)
		{
			pAstode->nLoseD = 0;
			pAstode->nWinD = MaxCrackDepth;
			pAstode->nVagueD = 0;
			//pAstode->tag1 = NOANSWER;
		}
	}
}

int DataCompare(qword dA[3], qword dB[3])
{
	if (*dA < *dB) return -1;
	if (*dA++ > *dB++) return 1;
	if (*dA < *dB) return -1;
	if (*dA++ > *dB++) return 1;
	if (*dA < *dB) return -1;
	if (*dA++ > *dB++) return 1;

	return 0;
}

static bool BinSearchStore(qword data[3], int &nIndex)
{
	int low = 0, high = g_nStoreSize - 1, mid;
	while (low <= high)
	{
		mid = (low + high) >> 1;
		int nResult = ::DataCompare(data, AstodeStore[mid]->data);
		if (nResult < 0)
			high = mid - 1;
		else if (nResult > 0)
			low = mid + 1;
		else
		{
			nIndex = mid;
			return true;
		}
	}
	nIndex = 0;
	return false;
}

static bool BinSearchBuffer(qword data[3], int &nIndex)
{
	int low = 0, high = g_nBufferSize - 1, mid;
	while (low <= high)
	{
		mid = (low + high) >> 1;
		int nResult = ::DataCompare(data, AstodeBuffer[mid]->data);
		if (nResult < 0)
			high = mid - 1;
		else if (nResult > 0)
			low = mid + 1;
		else
		{
			nIndex = mid;
			return true;
		}
	}
	nIndex = low;
	return false;
}

static void AddAstode(int nIndex, TAstode *pAstode, int nStep, int nRest)
{
	//指向Buffer的索引偏移
	for (int i = 2; i < nStep; i += 2)
	{
		if (g_nBSIndex[i] >= nIndex && g_nBSIndex[i] < MaxBufferSize)
			g_nBSIndex[i]++;
	}
	g_nBSIndex[nStep] = nIndex;
	//向后移动节点
	TAstode *pTemp = AstodeBuffer[g_nBufferSize];
	for (int i = g_nBufferSize - 1; i >= nIndex; i--)
		AstodeBuffer[i + 1] = AstodeBuffer[i];
	AstodeBuffer[nIndex] = pTemp;
	//填写节点
	pAstode->nLoseD = nRest;
	pAstode->nWinD = MaxCrackDepth;
	pAstode->nVagueD = 0;
	pAstode->tag1 = NOANSWER;
	pAstode->tag2 = 0xFFFFFFFF;
	memcpy(AstodeBuffer[nIndex], pAstode, sizeof(TAstode));
	//是否归并
	if (++g_nBufferSize == MaxBufferSize)
	{
		::MergeBufferStore(nStep);
		g_nBufferSize = 0;
	}
}

int CallAstode(TAstode &tAstode, int nStep, int nRest)
{
	g_bAddAstode = false;
	//存储区检索
	int nIndex;
	if (::BinSearchStore(tAstode.data, nIndex))
	{
		g_nBSIndex[nStep] = nIndex + MaxBufferSize;
		if (AstodeStore[nIndex]->tag1 == NOFURTHER)
			return NOFURTHER;
		if (nRest <= AstodeStore[nIndex]->nLoseD)
			return REDLOSE;
		if (nRest >= AstodeStore[nIndex]->nWinD)
			return BLKLOSE;
		AstodeStore[nIndex]->nVagueD = AstodeStore[nIndex]->nLoseD;
		AstodeStore[nIndex]->nLoseD = nRest;
		return NOLOSE;
	}
	//剩余空间不足
	if (g_nStoreSize > MaxStoreSize - MaxBufferSize)
		return FULLASTODE;
	//缓冲区检索
	if (::BinSearchBuffer(tAstode.data, nIndex))
	{
		g_nBSIndex[nStep] = nIndex;
		if (AstodeBuffer[nIndex]->tag1 == NOFURTHER)
			return NOFURTHER;
		if (nRest <= AstodeBuffer[nIndex]->nLoseD)
			return REDLOSE;
		if (nRest >= AstodeBuffer[nIndex]->nWinD)
			return BLKLOSE;
		AstodeBuffer[nIndex]->nVagueD = AstodeBuffer[nIndex]->nLoseD;
		AstodeBuffer[nIndex]->nLoseD = nRest;
		return NOLOSE;
	}
	g_bAddAstode = true;
	::AddAstode(nIndex, &tAstode, nStep, nRest);
	return NOLOSE;
}

void ShrinkAstode(qword data[3])
{
	data[0] = Z;
	data[1] = data[2] = 0;

	for (int i = 0, j = 1, k = 0; i < 64; i++)
	{
		if (board[i])
		{
			data[j] |= (qword)board[i] << k;
			k += 4;
			if (k == 64) { k = 0; j++; }
		}
	}
}

void ExpandAstode(qword data[3])
{
	memset(board, 0, sizeof(board));
	for (int i = 0, j = 1, k = 0; i < 64; i++)
	{
		if (data[0] & ((qword)1 << i))
		{
			board[i] = (data[j] >> k) & 0xF;
			k += 4;
			if (k == 64) { k = 0; j++; }
		}
	}
}
