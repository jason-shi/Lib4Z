#include "StdAfx.h"
#include "Table.h"

#define Index2Card(n)			((nIndex >> (n)) & 0x3F) //[2, A]
#define Index2CardNT(n)			((nIndex >> (n)) & 0x0F) //[2, A]

void FastCompare_DescSortCard(VECBYTE &vecCard)
{
	//按大小、花色降序排列
	for (int i = 0; i < (int)vecCard.size() - 1; i++)
	{
		for (int j = i + 1; j < (int)vecCard.size(); j++)
		{
			if ((vecCard[i] & 0x0F) < (vecCard[j] & 0x0F)
				|| (vecCard[i] & 0x0F) == (vecCard[j] & 0x0F) && (vecCard[i] >> 4) > (vecCard[j] >> 4))
			{
				byte byTemp = vecCard[i];
				vecCard[i] = vecCard[j];
				vecCard[j] = byTemp;
			}
		}
	}

	//对子调整：对子在前
	if (vecCard.size() == 3)
	{
		if ((vecCard[1] & 0x0F) == (vecCard[2] & 0x0F))
		{
			byte byTemp = vecCard[0];
			vecCard[0] = vecCard[2];
			vecCard[2] = byTemp;
		}
	}

	//对子调整：按黑桃、红桃、草花、方块的顺序
	if ((vecCard[0] & 0x0F) == (vecCard[1] & 0x0F))
	{
		if ((vecCard[0] >> 4) > (vecCard[1] >> 4))
		{
			byte byTemp = vecCard[0];
			vecCard[0] = vecCard[1];
			vecCard[1] = byTemp;
		}
	}
}

void FastCompare_TransToCard(int nIndex, VECBYTE &vecCard)
{
	if ((Index2Card(0) & 0x0F) <= 12)
		vecCard.push_back(Index2Card(0));

	if ((Index2Card(6) & 0x0F) <= 12)
		vecCard.push_back(Index2Card(6));

	if ((Index2Card(12) & 0x0F) <= 12)
		vecCard.push_back(Index2Card(12));
}

bool FastCompare_IsCardLegal(int nIndex)
{
	VECBYTE vecCard;
	FastCompare_TransToCard(nIndex, vecCard);

	//合法性检查
	switch (vecCard.size())
	{
	case 0:
	case 1:
		return false;
	case 2:
		if (Index2Card(0) != 0x3F && Index2Card(6) != 0x3F && Index2Card(12) != 0x3F) //必须有一张牌无效
			return false;
		return vecCard[0] != vecCard[1];
	case 3:
		return vecCard[0] != vecCard[1] && vecCard[0] != vecCard[2] && vecCard[1] != vecCard[2];
	}
	return false;
}

byte FastCompare_GetPileType(VECBYTE &vecValue)
{
	if (vecValue.size() == 2)
	{
		//对子
		if ((vecValue[0] & 0x0F) == (vecValue[1] & 0x0F))
			return TYPE_PAIR;

		return TYPE_SINGLE;
	}
	else
	{
		//炸弹
		if ((vecValue[0] & 0x0F) == (vecValue[1] & 0x0F) && (vecValue[1] & 0x0F) == (vecValue[2] & 0x0F))
			return TYPE_BOMB;

		//对子
		if ((vecValue[0] & 0x0F) == (vecValue[1] & 0x0F) || (vecValue[1] & 0x0F) == (vecValue[2] & 0x0F))
			return TYPE_PAIR;

		//顺子
		if ((vecValue[0] & 0x0F) == (vecValue[1] & 0x0F) + 1 && (vecValue[1] & 0x0F) == (vecValue[2] & 0x0F) + 1
			|| (vecValue[0] & 0x0F) == 12 && (vecValue[1] & 0x0F) == 1 && (vecValue[2] & 0x0F) == 0)
		{
			//同花顺
			if ((vecValue[0] >> 4) == (vecValue[1] >> 4) && (vecValue[1] >> 4) == (vecValue[2] >> 4))
				return TYPE_S_SHUNZI;

			return TYPE_SHUNZI;
		}
	}
	return TYPE_SINGLE;
}

int FastCompare_CompareNN(const VECBYTE &vecCard1, const VECBYTE &vecCard2)
{
	//先比大小
	for (int i = 0; i < (int)vecCard1.size(); i++)
	{
		if ((vecCard1[i] & 0x0F) > (vecCard2[i] & 0x0F))
			return 1;
		if ((vecCard1[i] & 0x0F) < (vecCard2[i] & 0x0F))
			return -1;
	}
	//再比花色
	for (int i = 0; i < (int)vecCard1.size(); i++)
	{
		if ((vecCard1[i] >> 4) < (vecCard2[i] >> 4))
			return 1;
		if ((vecCard1[i] >> 4) > (vecCard2[i] >> 4))
			return -1;
	}
	return 0;
}

int FastCompare_CompareMN(const VECBYTE &vecCard1, const VECBYTE &vecCard2)
{
	int nMinSize = min(vecCard1.size(), vecCard2.size());
	for (int i = 0; i < nMinSize; i++)
	{
		if ((vecCard1[i] & 0x0F) > (vecCard2[i] & 0x0F))
			return 1;
		if ((vecCard1[i] & 0x0F) < (vecCard2[i] & 0x0F))
			return -1;
	}
	return vecCard1.size() > vecCard2.size(); //两张小于三张
}

int FastCompare_ComparePile(dword nIndex1, dword nIndex2)
{
	VECBYTE vecCard1;
	VECBYTE vecCard2;
	FastCompare_TransToCard(nIndex1, vecCard1);
	FastCompare_TransToCard(nIndex2, vecCard2);

	FastCompare_DescSortCard(vecCard1);
	FastCompare_DescSortCard(vecCard2);

	return vecCard1.size() == vecCard2.size() ? FastCompare_CompareNN(vecCard1, vecCard2)
											  : FastCompare_CompareMN(vecCard1, vecCard2);
}

void FastCompare_QuickSort(dword *TblSortPile, int left, int right)
{
	if (left < right)
	{
		int key = TblSortPile[left];
		int low = left;
		int high = right;
		while (low < high)
		{
			while (low < high && FastCompare_ComparePile(TblSortPile[high], key) >= 0)
				high--;
			TblSortPile[low] = TblSortPile[high];

			while (low < high && FastCompare_ComparePile(TblSortPile[low], key) <= 0)
				low++;
			TblSortPile[high] = TblSortPile[low];
		}
		TblSortPile[low] = key;
		FastCompare_QuickSort(TblSortPile, left, low - 1);
		FastCompare_QuickSort(TblSortPile, low + 1, right);
	}
}

void SaveTblFastCompare(const TCHAR *wcsFileName)
{
	FILE *fp;
	if (!(fp = _wfopen(wcsFileName, L"wb+")))
		return;

	byte szBuff[4096];
	for (int i = 0, j = 0; i < 64 * 64 * 64; i++)
	{
		szBuff[j++] = TblFastCompare[i] >> 24;
		szBuff[j++] = TblFastCompare[i] >> 16;
		szBuff[j++] = TblFastCompare[i] >> 8;
		szBuff[j++] = TblFastCompare[i] >> 0;

		//分段写入文件
		if (j == 4096)
		{
			fwrite(szBuff, 4096, 1, fp);
			j = 0;
		}
	}

	fclose(fp);
	fp = NULL;
}

void LoadTblFastCompare(const TCHAR *wcsFileName)
{
	FILE *fp;
	if (!(fp = _wfopen(wcsFileName, L"rb")))
		return;

	byte szBuff[4096];
	int i = 0;
	//分段读入文件
	while ((fread(szBuff, 4096, 1, fp)) != 0)
	{
		for (int j = 0; j < 4096; j += 4)
		{
			TblFastCompare[i] |= szBuff[j + 0] << 24;
			TblFastCompare[i] |= szBuff[j + 1] << 16;
			TblFastCompare[i] |= szBuff[j + 2] << 8;
			TblFastCompare[i] |= szBuff[j + 3] << 0;
			i++;
		}
		if (i == 64 * 64 * 64)
			break;
	}

	fclose(fp);
	fp = NULL;
}

void FastCompare_CreatePileType(byte *TblPileType)
{
	for (int i = 0; i < 64 * 64 * 64; i++)
	{
		if (!FastCompare_IsCardLegal(i))
			continue;

		VECBYTE vecCard;
		FastCompare_TransToCard(i, vecCard);
		FastCompare_DescSortCard(vecCard);
		TblPileType[i] = FastCompare_GetPileType(vecCard);
	}
}

void FastCompare_CreateSortPile(dword *TblSortPile, byte *TblPileType)
{
	int nRight = 0;
	for (int i = 0; i < 64 * 64 * 64; i++)
	{
		if (TblPileType[i] == 0)
			TblSortPile[nRight++] = 0;
	}

	//生成单张、对子、顺子、炸弹、同花顺，并分别排序
	for (byte byType = TYPE_SINGLE; byType <= TYPE_S_SHUNZI; byType++)
	{
		int nLeft = nRight;
		for (int i = 0; i < 64 * 64 * 64; i++)
		{
			if (TblPileType[i] == byType)
				TblSortPile[nRight++] = i | (byType << 18);
		}
		FastCompare_QuickSort(TblSortPile, nLeft, nRight - 1);
	}
}

void CreateFastCompare()
{
	TblFastCompare = new dword[64 * 64 * 64];
	memset(TblFastCompare, 0, sizeof(dword [64 * 64 * 64]));

	if (_waccess(L"TblFastCompare.tbl", 0) == -1)
	{
		dword *TblSortPile = new dword[64 * 64 * 64];

		byte *TblPileType = new byte[64 * 64 * 64];
		memset(TblPileType, 0, sizeof(byte [64 * 64 * 64]));

		FastCompare_CreatePileType(TblPileType);
		FastCompare_CreateSortPile(TblSortPile, TblPileType);

		//将有序的结果映射为排序索引：高位为类型
		for (int i = 0; i < 64 * 64 * 64; i++)
			TblFastCompare[TblSortPile[i] & 0x3FFFF] = i | (TblSortPile[i] >> 18 << 18);

		delete TblPileType;
		TblPileType = NULL;

		delete TblSortPile;
		TblSortPile = NULL;

		//保存到文件
		SaveTblFastCompare(L"TblFastCompare.tbl");
	}
	else
	{
		//从文件加载
		LoadTblFastCompare(L"TblFastCompare.tbl");
	}
}

//-----------------------------------------------------------------------------
void FastCompareNT_TransToCard(int nIndex, VECBYTE &vecCard)
{
	if (Index2CardNT(0) <= 12)
		vecCard.push_back(Index2CardNT(0));

	if (Index2CardNT(4) <= 12)
		vecCard.push_back(Index2CardNT(4));

	if (Index2CardNT(8) <= 12)
		vecCard.push_back(Index2CardNT(8));
}

bool FastCompareNT_IsCardLegal(int nIndex)
{
	VECBYTE vecCard;
	FastCompareNT_TransToCard(nIndex, vecCard);

	//合法性检查
	if (vecCard.size() < 2)
		return false;

	if (vecCard.size() == 2)
	{
		 //必须有一张牌无效
		return Index2CardNT(0) == 0x0F
			|| Index2CardNT(4) == 0x0F
			|| Index2CardNT(8) == 0x0F;
	}
	return true;
}

void FastCompareNT_DescSortCard(VECBYTE &vecCard)
{
	//按大小降序排列
	for (int i = 0; i < (int)vecCard.size() - 1; i++)
	{
		for (int j = i + 1; j < (int)vecCard.size(); j++)
		{
			if (vecCard[i] < vecCard[j])
			{
				byte byTemp = vecCard[i];
				vecCard[i] = vecCard[j];
				vecCard[j] = byTemp;
			}
		}
	}

	//对子调整：对子在前
	if (vecCard.size() == 3)
	{
		if (vecCard[1] == vecCard[2])
		{
			byte byTemp = vecCard[0];
			vecCard[0] = vecCard[2];
			vecCard[2] = byTemp;
		}
	}
}

void SaveTblFastCompareNT(const TCHAR *wcsFileName)
{
	FILE *fp;
	if (!(fp = _wfopen(wcsFileName, L"wb+")))
		return;

	byte szBuff[4096];
	for (int i = 0, j = 0; i < 16 * 16 * 16; i++)
	{
		szBuff[j++] = TblFastCompareNT[i] >> 24;
		szBuff[j++] = TblFastCompareNT[i] >> 16;
		szBuff[j++] = TblFastCompareNT[i] >> 8;
		szBuff[j++] = TblFastCompareNT[i] >> 0;

		//分段写入文件
		if (j == 4096)
		{
			fwrite(szBuff, 4096, 1, fp);
			j = 0;
		}
	}

	fclose(fp);
	fp = NULL;
}

void LoadTblFastCompareNT(const TCHAR *wcsFileName)
{
	FILE *fp;
	if (!(fp = _wfopen(wcsFileName, L"rb")))
		return;

	byte szBuff[4096];
	int i = 0;
	//分段读入文件
	while ((fread(szBuff, 4096, 1, fp)) != 0)
	{
		for (int j = 0; j < 4096; j += 4)
		{
			TblFastCompareNT[i] |= szBuff[j + 0] << 24;
			TblFastCompareNT[i] |= szBuff[j + 1] << 16;
			TblFastCompareNT[i] |= szBuff[j + 2] << 8;
			TblFastCompareNT[i] |= szBuff[j + 3] << 0;
			i++;
		}
		if (i == 16 * 16 * 16)
			break;
	}

	fclose(fp);
	fp = NULL;
}

void CreateFastCompareNT()
{
	TblFastCompareNT = new dword[16 * 16 * 16];
	memset(TblFastCompareNT, 0, sizeof(dword [16 * 16 * 16]));

	if (_waccess(L"TblFastCompareNT.tbl", 0) == -1)
	{
		for (int i = 0; i < 16 * 16 * 16; i++)
		{
			if (!FastCompareNT_IsCardLegal(i))
				continue;

			VECBYTE vecCard;
			FastCompareNT_TransToCard(i, vecCard);

			//补充无效牌
			if (vecCard.size() == 2)
				vecCard.push_back(0x0F);

			//添加不同的花色
			vecCard[0] |= 0x10;
			vecCard[1] |= 0x20;
			vecCard[2] |= 0x30; //注：0x30 | 0x0F = 0x3F是无效牌

			//降序排列
			FastCompareNT_DescSortCard(vecCard);

			//高位为类型
			TblFastCompareNT[i] = TblFastCompare[(vecCard[0] << 12) + (vecCard[1] << 6) + vecCard[2]];
		}

		//保存到文件
		SaveTblFastCompareNT(L"TblFastCompareNT.tbl");
	}
	else
	{
		//从文件加载
		LoadTblFastCompareNT(L"TblFastCompareNT.tbl");
	}
}
