#include "StdAfx.h"
#include "Astode.h"

static void SetNewIndex(int nStep, int nOldIndex, int nNewIndex)
{
	for (int n = 2; n <= nStep; n += 2)
	{
		if (g_bAstable[n] && g_nBSIndex[n] == nOldIndex)
		{
			g_nBSIndex[n] = nNewIndex + MaxBufferSize + MaxStoreSize;
			break;
		}
	}
}

static void MarkAttention(int nStep)
{
	for (int n = 2; n <= nStep; n += 2)
	{
		if (g_bAstable[n])
		{
			if (g_nBSIndex[n] < MaxBufferSize)
				MarkBuffer[g_nBSIndex[n]] = true;
			else
				MarkStore[g_nBSIndex[n] - MaxBufferSize] = true;
		}
	}
}

static void SwapTempStore()
{
	for (int i = 0; i < g_nStoreSize; i++)
	{
		TAstode *pTemp = AstodeStore[i];
		AstodeStore[i] = TempStore[i];
		TempStore[i] = pTemp;
	}
}

static void MergeRestBuffer(int nStep, int j, int k)
{
	TAstode *pTemp;
	for (; j < MaxBufferSize; j++)
	{
		if (MarkBuffer[j])
		{
			MarkBuffer[j] = false;
			::SetNewIndex(nStep, j, k);
		}
		pTemp = AstodeBuffer[j];
		AstodeBuffer[j] = TempStore[k];
		TempStore[k++] = pTemp;
	}
	g_nStoreSize = k;
}

static void MergeRestStore(int nStep, int i, int k)
{
	TAstode *pTemp;
	for (; i < g_nStoreSize; i++)
	{
		if (MarkStore[i])
		{
			MarkStore[i] = false;
			::SetNewIndex(nStep, i + MaxBufferSize, k);
		}
		pTemp = AstodeStore[i];
		AstodeStore[i] = TempStore[k];
		TempStore[k++] = pTemp;
	}
	g_nStoreSize = k;
}

void MergeBufferStore(int nStep)
{
	::MarkAttention(nStep);

	int i = 0, j = 0, k = 0;
	TAstode *pTemp;
	while (true)
	{
		int nResult = ::DataCompare(AstodeStore[i]->data, AstodeBuffer[j]->data);
		if (nResult <= 0)
		{
			if (i != g_nStoreSize)
			{
				if (MarkStore[i])
				{
					MarkStore[i] = false;
					::SetNewIndex(nStep, i + MaxBufferSize, k);
				}
				pTemp = TempStore[k];
				TempStore[k++] = AstodeStore[i];
				AstodeStore[i++] = pTemp;
			}
			if (i == g_nStoreSize)
			{
				::MergeRestBuffer(nStep, j, k);
				::SwapTempStore();
				for (int n = 2; n <= nStep; n += 2)
					g_nBSIndex[n] -= MaxStoreSize;
				break;
			}
		}
		else
		{
			if (j != MaxBufferSize)
			{
				if (MarkBuffer[j])
				{
					MarkBuffer[j] = false;
					::SetNewIndex(nStep, j, k);
				}
				pTemp = TempStore[k];
				TempStore[k++] = AstodeBuffer[j];
				AstodeBuffer[j++] = pTemp;
			}
			if (j == MaxBufferSize)
			{
				::MergeRestStore(nStep, i, k);
				::SwapTempStore();
				for (int n = 2; n <= nStep; n += 2)
					g_nBSIndex[n] -= MaxStoreSize;
				break;
			}
		}
	}
}
