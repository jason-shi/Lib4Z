#include "StdAfx.h"
#include "Astode.h"

#define GetByte0(v)	 ((v & 0xFF) - 1)
#define GetByte1(v)	 (((v & 0xFF00) >> 8) - 1)
#define GetByte2(v)	 (((v & 0xFF0000) >> 16) - 1)
#define GetByte3(v)	 (((unsigned)v >> 24) - 1)

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

int DataCompare(int dA[8], int dB[8])
{
	if (*dA < *dB) return -1;
	if (*dA++ > *dB++) return 1;
	if (*dA < *dB) return -1;
	if (*dA++ > *dB++) return 1;
	if (*dA < *dB) return -1;
	if (*dA++ > *dB++) return 1;
	if (*dA < *dB) return -1;
	if (*dA++ > *dB++) return 1;
	if (*dA < *dB) return -1;
	if (*dA++ > *dB++) return 1;
	if (*dA < *dB) return -1;
	if (*dA++ > *dB++) return 1;
	if (*dA < *dB) return -1;
	if (*dA++ > *dB++) return 1;
	if (*dA < *dB) return -1;
	if (*dA++ > *dB++) return 1;

	return 0;
}

static bool BinSearchStore(int data[8], int &nIndex)
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

static bool BinSearchBuffer(int data[8], int &nIndex)
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

void ShrinkAstodeRed(int data[8])
{
	data[0] = data[1] = data[2] = data[3] = 0;
	//通过红方棋子进行缩写
	if (PieceRedR)
	{
		data[0] |= (PieceRedR->pos + 1) << 16;
		if (PieceRedR->next)
			data[0] |= (PieceRedR->next->pos + 1) << 24;
	}
	if (PieceRedC)
	{
		data[2] |= (PieceRedC->pos + 1) << 16;
		if (PieceRedC->next)
			data[2] |= (PieceRedC->next->pos + 1) << 24;
	}
	if (PieceRedH)
	{
		data[0] |= PieceRedH->pos + 1;
		if (PieceRedH->next)
			data[0] |= (PieceRedH->next->pos + 1) << 8;
	}
	TPiece *piece = PieceRedS;
	while (piece)
	{
		int value = piece->pos + 1;
		switch (piece->kind)
		{
		case R_BISHOP:
			data[1] |= value << (data[1] & 0xFF0000 ? 24 : 16);
			break;
		case R_GUARD:
			data[1] |= data[1] & 0xFF ? value << 8 : value;
			break;
		default:
			if (!(data[2] & 0xFF))			data[2] |= value;
			else if (!(data[2] & 0xFF00))	data[2] |= (value << 8);
			else if (!(data[3] & 0xFF00))	data[3] |= (value << 8);
			else if (!(data[3] & 0xFF0000))	data[3] |= (value << 16);
			else							data[3] |= (value << 24);
			break;
		}
		piece = piece->next;
	}
	data[3] |= PieceRedK->pos + 1;
}

void ShrinkAstodeBlk(int data[8])
{
	data[4] = data[5] = data[6] = data[7] = 0;
	//通过黑方棋子进行缩写
	TPiece *piece = PieceBlkB;
	while (piece)
	{
		int value = piece->pos + 1;
		switch (piece->kind)
		{
		case B_ROOK:
			data[4] |= value << (data[4] & 0xFF0000 ? 24 : 16);
			break;
		case B_HORSE:
			data[4] |= data[4] & 0xFF ? value << 8 : value;
			break;
		default:
			data[6] |= value << (data[6] & 0xFF0000 ? 24 : 16);
			break;
		}
		piece = piece->next;
	}
	piece = PieceBlkS;
	while (piece)
	{
		int value = piece->pos + 1;
		switch (piece->kind)
		{
		case B_BISHOP:
			data[5] |= value << (data[5] & 0xFF0000 ? 24 : 16);
			break;
		case B_GUARD:
			data[5] |= data[5] & 0xFF ? value << 8 : value;
			break;
		default:
			if (!(data[6] & 0xFF))			data[6] |= value;
			else if (!(data[6] & 0xFF00))	data[6] |= (value << 8);
			else if (!(data[7] & 0xFF00))	data[7] |= (value << 8);
			else if (!(data[7] & 0xFF0000))	data[7] |= (value << 16);
			else							data[7] |= (value << 24);
			break;
		}
		piece = piece->next;
	}
	data[7] |= PieceBlkK->pos + 1;
}

void ExpandAstode(int data[8])
{
	InitBoard();
	if (data[0] & 0xFF0000)
	{
		board[GetByte2(data[0])] = R_ROOK;
		if (data[0] & 0xFF000000)
			board[GetByte3(data[0])] = R_ROOK;
	}
	if (data[0] & 0xFF)
	{
		board[GetByte0(data[0])] = R_HORSE;
		if (data[0] & 0xFF00)
			board[GetByte1(data[0])] = R_HORSE;
	}
	if (data[1] & 0xFF0000)
	{
		board[GetByte2(data[1])] = R_BISHOP;
		if (data[1] & 0xFF000000)
			board[GetByte3(data[1])] = R_BISHOP;
	}
	if (data[1] & 0xFF)
	{
		board[GetByte0(data[1])] = R_GUARD;
		if (data[1] & 0xFF00)
			board[GetByte1(data[1])] = R_GUARD;
	}
	if (data[2] & 0xFF0000)
	{
		board[GetByte2(data[2])] = R_CANNON;
		if (data[2] & 0xFF000000)
			board[GetByte3(data[2])] = R_CANNON;
	}
	if (data[2] & 0xFF)
	{
		board[GetByte0(data[2])] = R_PAWN;
		if (data[2] & 0xFF00)
		{
			board[GetByte1(data[2])] = R_PAWN;
			if (data[3] & 0xFF00)
			{
				board[GetByte1(data[3])] = R_PAWN;
				if (data[3] & 0xFF0000)
				{
					board[GetByte2(data[3])] = R_PAWN;
					if (data[3] & 0xFF000000)
						board[GetByte3(data[3])] = R_PAWN;
				}
			}
		}
	}
	board[GetByte0(data[3])] = R_KING;
	if (data[4] & 0xFF0000)
	{
		board[GetByte2(data[4])] = B_ROOK;
		if (data[4] & 0xFF000000)
			board[GetByte3(data[4])] = B_ROOK;
	}
	if (data[4] & 0xFF)
	{
		board[GetByte0(data[4])] = B_HORSE;
		if (data[4] & 0xFF00)
			board[GetByte1(data[4])] = B_HORSE;
	}
	if (data[5] & 0xFF0000)
	{
		board[GetByte2(data[5])] = B_BISHOP;
		if (data[5] & 0xFF000000)
			board[GetByte3(data[5])] = B_BISHOP;
	}
	if (data[5] & 0xFF)
	{
		board[GetByte0(data[5])] = B_GUARD;
		if (data[5] & 0xFF00)
			board[GetByte1(data[5])] = B_GUARD;
	}
	if (data[6] & 0xFF0000)
	{
		board[GetByte2(data[6])] = B_CANNON;
		if (data[6] & 0xFF000000)
			board[GetByte3(data[6])] = B_CANNON;
	}
	if (data[6] & 0xFF)
	{
		board[GetByte0(data[6])] = B_PAWN;
		if (data[6] & 0xFF00)
		{
			board[GetByte1(data[6])] = B_PAWN;
			if (data[7] & 0xFF00)
			{
				board[GetByte1(data[7])] = B_PAWN;
				if (data[7] & 0xFF0000)
				{
					board[GetByte2(data[7])] = B_PAWN;
					if (data[7] & 0xFF000000)
						board[GetByte3(data[7])] = B_PAWN;
				}
			}
		}
	}
	board[GetByte0(data[7])] = B_KING;
}
