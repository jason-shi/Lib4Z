#include "StdAfx.h"
#include "Table.h"
#include "Split\Split.h"
#include "CoSplit\CoSplit.h"

bool FastSplit_CheckLegal(byte byFlag[8])
{
	//检测是否有超过4张同样大小的牌
	int nCount = 1;
	for (int i = 0; i < 8; i++)
	{
		nCount = byFlag[i] == 0 ? nCount + 1 : 1;
		if (nCount > 4)
			return false;
	}

	//最后是升时，中间必须含有至少一次跳
	if (byFlag[7] == 1)
	{
		bool bHasSkip = false;
		for (int i = 0; i < 8; i++)
		{
			if (byFlag[i] == 2)
			{
				bHasSkip = true;
				break;
			}
		}
		if (!bHasSkip)
			return false;
	}

	//最后是升，至多到达A，最后是跳，至多达到K
	byte byGrade = 0;
	for (int i = 0; i < 7; i++)
		byGrade += byFlag[i];

	if (byFlag[7] == 1 && byGrade > 12)
		return false;
	if (byFlag[7] != 1 && byGrade > 11)
		return false;

	return true;
}

bool FastSplit_TransToFlag(int nValue, byte byFlag[8])
{
	for (int i = 0; i < 8; i++)
	{
		byFlag[i] = nValue & 0x3;
		nValue >>= 2;
	}

	//滤除同花顺标记->升
	for (int i = 0; i < 8; i++)
	{
		if (byFlag[i] == 3)
			byFlag[i] = 1;
	}
	return FastSplit_CheckLegal(byFlag);
}

void FastSplit_TransToCard(byte byFlag[8], byte byCard[8])
{
	//公式：byCard[i] = byCard[i - 1] + byFlag[i - 1];
	if (byFlag[7] == 1)
	{
		//第一张是2，正着转换，到跳为止
		byCard[0] = 0;
		int nSkip = 0;
		for (int i = 1; i < 8; i++)
		{
			if (byFlag[i - 1] == 2)
			{
				nSkip = i;
				break;
			}
			byCard[i] = byCard[i - 1] + byFlag[i - 1];
		}
		//最后一张是A，倒着转换
		byCard[7] = 12;
		for (int i = 7; i > nSkip; i--)
			byCard[i - 1] = byCard[i] - byFlag[i - 1];
	}
	else
	{
		//第一张是3，正着转换
		byCard[0] = 1;
		for (int i = 1; i < 8; i++)
			byCard[i] = byCard[i - 1] + byFlag[i - 1];
	}
}

void FastSplit_TransToFlushBitmap(int nValue, byte byCard[8], word &wFlushBitmap)
{
	word _bitmap = 0;

	byte bySame[8];
	for (int i = 0; i < 8; i++)
	{
		bySame[i] = nValue & 0x3;
		nValue >>= 2;
	}

	//标记同花顺的牌
	for (int i = 0; i < 8; i++)
	{
		if (bySame[i] == 3)
			_bitmap |= 1 << byCard[i];
	}

	//标记同花顺的最后的一张牌
	byte byCount = 0;
	if (byCard[7] == 12 && bySame[7] == 3)
		byCount = 1;
	for (int i = 0; i < 8; i++)
	{
		switch (bySame[i])
		{
		case 1:
		case 2:
			if (byCount >= 2)
				_bitmap |= 1 << byCard[i];
			byCount = 0;
			break;
		case 3:
			byCount++;
			break;
		}
	}

	//剔除不构成顺子的同花顺标记
	word wA23 = _bitmap & MASK_A23;
	if (wA23 != MASK_A23)
		wA23 = 0;
	word wQKA = _bitmap & MASK_QKA;
	if (wQKA != MASK_QKA)
		wQKA = 0;

	//保留所有的同花顺标记
	wFlushBitmap = TblSanShun[_bitmap & MASK_SHUNZI] | wA23 | wQKA;
}

void FastSplit_TransToSplitIndex(byte byFlag[8], int &nSplitIndex)
{
	nSplitIndex = (byFlag[0] <<  0)
				| (byFlag[1] <<  2)
				| (byFlag[2] <<  4)
				| (byFlag[3] <<  6)
				| (byFlag[4] <<  8)
				| (byFlag[5] << 10)
				| (byFlag[6] << 12)
				| (byFlag[7] << 14);
}

void SaveTblFastSplit(const TCHAR *wcsFileName)
{
	FILE *fp;
	if (!(fp = _wfopen(wcsFileName, L"wb+")))
		return;

	byte szBuff[4096];
	for (int i = 0, j = 0; i < 65536; i++)
	{
		szBuff[j++] = TblFastSplit[i] >> 24;
		szBuff[j++] = TblFastSplit[i] >> 16;
		szBuff[j++] = TblFastSplit[i] >> 8;
		szBuff[j++] = TblFastSplit[i] >> 0;

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

void LoadTblFastSplit(const TCHAR *wcsFileName)
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
			TblFastSplit[i] |= szBuff[j + 0] << 24;
			TblFastSplit[i] |= szBuff[j + 1] << 16;
			TblFastSplit[i] |= szBuff[j + 2] << 8;
			TblFastSplit[i] |= szBuff[j + 3] << 0;
			i++;
		}
		if (i == 65536)
			break;
	}

	fclose(fp);
	fp = NULL;
}

void CreateFastSplit()
{
	TblFastSplit = new dword[65536];
	memset(TblFastSplit, 0, sizeof(dword [65536]));

	if (_waccess(L"TblFastSplit.tbl", 0) == -1)
	{
		word *OriFlushBitmap = new word[65536];
		memset(OriFlushBitmap, 0, sizeof(word [65536]));

		for (int i = 0; i < 65536; i++)
		{
			//转换成byFlag
			byte byFlag[8];
			if (!FastSplit_TransToFlag(i, byFlag))
				continue;

			//转换成byCard
			byte byCard[8];
			FastSplit_TransToCard(byFlag, byCard);

			//转换成同花顺位图
			word wFlushBitmap;
			FastSplit_TransToFlushBitmap(i, byCard, wFlushBitmap);

			//用于防止重复拆牌
			OriFlushBitmap[i] = wFlushBitmap;

			//获得降等级索引
			int nSplitIndex;
			FastSplit_TransToSplitIndex(byFlag, nSplitIndex);

			//判断是否需要重新拆牌
			if (wFlushBitmap == OriFlushBitmap[nSplitIndex]
				&& TblFastSplit[nSplitIndex] != 0)
			{
				TblFastSplit[i] = TblFastSplit[nSplitIndex];
				continue;
			}

			CSplit tSplit;
			TblFastSplit[i] = tSplit.ForceSplit(byCard, wFlushBitmap);
		}

		delete OriFlushBitmap;
		OriFlushBitmap = NULL;

		//保存到文件
		SaveTblFastSplit(L"TblFastSplit.tbl");
	}
	else
	{
		//从文件加载
		LoadTblFastSplit(L"TblFastSplit.tbl");
	}
}

//-----------------------------------------------------------------------------
void SaveTblCoFastSplit(const TCHAR *wcsFileName)
{
	FILE *fp;
	if (!(fp = _wfopen(wcsFileName, L"wb+")))
		return;

	byte szBuff[4096];
	for (int k = 0; k < 8; k++)
	{
		for (int i = 0, j = 0; i < 65536; i++)
		{
			szBuff[j++] = TblCoFastSplit[k][i] >> 24;
			szBuff[j++] = TblCoFastSplit[k][i] >> 16;
			szBuff[j++] = TblCoFastSplit[k][i] >> 8;
			szBuff[j++] = TblCoFastSplit[k][i] >> 0;

			//分段写入文件
			if (j == 4096)
			{
				fwrite(szBuff, 4096, 1, fp);
				j = 0;
			}
		}
	}

	fclose(fp);
	fp = NULL;
}

void LoadTblCoFastSplit(const TCHAR *wcsFileName)
{
	FILE *fp;
	if (!(fp = _wfopen(wcsFileName, L"rb")))
		return;

	byte szBuff[4096];

	for (int k = 0; k < 8; k++)
	{
		int i = 0;
		//分段读入文件
		while ((fread(szBuff, 4096, 1, fp)) != 0)
		{
			for (int j = 0; j < 4096; j += 4)
			{
				TblCoFastSplit[k][i] |= szBuff[j + 0] << 24;
				TblCoFastSplit[k][i] |= szBuff[j + 1] << 16;
				TblCoFastSplit[k][i] |= szBuff[j + 2] << 8;
				TblCoFastSplit[k][i] |= szBuff[j + 3] << 0;
				i++;
			}
			if (i == 65536)
				break;
		}
	}

	fclose(fp);
	fp = NULL;
}

void CreateCoFastSplit()
{
	for (int i = 0; i < 8; i++)
	{
		TblCoFastSplit[i] = new dword[65536];
		memset(TblCoFastSplit[i], 0, sizeof(dword [65536]));
	}

	if (_waccess(L"TblCoFastSplit.tbl", 0) == -1)
	{
		word *OriFlushBitmap = new word[65536];
		memset(OriFlushBitmap, 0, sizeof(word [65536]));

		for (int i = 0; i < 65536; i++)
		{
			//转换成byFlag
			byte byFlag[8];
			if (!FastSplit_TransToFlag(i, byFlag))
				continue;

			//转换成byCard
			byte byCard[8];
			FastSplit_TransToCard(byFlag, byCard);

			//转换成同花顺位图
			word wFlushBitmap;
			FastSplit_TransToFlushBitmap(i, byCard, wFlushBitmap);

			//用于防止重复拆牌
			OriFlushBitmap[i] = wFlushBitmap;

			//获得降等级索引
			int nSplitIndex;
			FastSplit_TransToSplitIndex(byFlag, nSplitIndex);

			//判断是否需要重新拆牌
			if (wFlushBitmap == OriFlushBitmap[nSplitIndex]
				&& TblCoFastSplit[0][nSplitIndex] != 0)
			{
				for (int j = 0; j < 8; j++)
					TblCoFastSplit[j][i] = TblCoFastSplit[j][nSplitIndex];
				continue;
			}

			//其他的拆牌方法
			dword dwLimitScore = 0xFFFFFFFF;
			int j;
			for (j = 0; j < 8; j++)
			{
				dword dwTempScore = dwLimitScore;

				CCoSplit tCoSplit;
				TblCoFastSplit[j][i] = tCoSplit.ForceSplit(dwLimitScore, byCard, wFlushBitmap);
				if (TblCoFastSplit[j][i] == 0)
				{
					j--;
					break;
				}

				if (dwLimitScore == dwTempScore)
					break;
			}
		}

		delete OriFlushBitmap;
		OriFlushBitmap = NULL;

		//保存到文件
		SaveTblCoFastSplit(L"TblCoFastSplit.tbl");
	}
	else
	{
		//从文件加载
		LoadTblCoFastSplit(L"TblCoFastSplit.tbl");
	}
}
