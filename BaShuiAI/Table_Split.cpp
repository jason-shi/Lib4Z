#include "StdAfx.h"
#include "Table.h"
#include "Split\Split.h"
#include "CoSplit\CoSplit.h"

bool FastSplit_CheckLegal(byte byFlag[8])
{
	//����Ƿ��г���4��ͬ����С����
	int nCount = 1;
	for (int i = 0; i < 8; i++)
	{
		nCount = byFlag[i] == 0 ? nCount + 1 : 1;
		if (nCount > 4)
			return false;
	}

	//�������ʱ���м���뺬������һ����
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

	//������������ൽ��A���������������ﵽK
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

	//�˳�ͬ��˳���->��
	for (int i = 0; i < 8; i++)
	{
		if (byFlag[i] == 3)
			byFlag[i] = 1;
	}
	return FastSplit_CheckLegal(byFlag);
}

void FastSplit_TransToCard(byte byFlag[8], byte byCard[8])
{
	//��ʽ��byCard[i] = byCard[i - 1] + byFlag[i - 1];
	if (byFlag[7] == 1)
	{
		//��һ����2������ת��������Ϊֹ
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
		//���һ����A������ת��
		byCard[7] = 12;
		for (int i = 7; i > nSkip; i--)
			byCard[i - 1] = byCard[i] - byFlag[i - 1];
	}
	else
	{
		//��һ����3������ת��
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

	//���ͬ��˳����
	for (int i = 0; i < 8; i++)
	{
		if (bySame[i] == 3)
			_bitmap |= 1 << byCard[i];
	}

	//���ͬ��˳������һ����
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

	//�޳�������˳�ӵ�ͬ��˳���
	word wA23 = _bitmap & MASK_A23;
	if (wA23 != MASK_A23)
		wA23 = 0;
	word wQKA = _bitmap & MASK_QKA;
	if (wQKA != MASK_QKA)
		wQKA = 0;

	//�������е�ͬ��˳���
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

		//�ֶ�д���ļ�
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
	//�ֶζ����ļ�
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
			//ת����byFlag
			byte byFlag[8];
			if (!FastSplit_TransToFlag(i, byFlag))
				continue;

			//ת����byCard
			byte byCard[8];
			FastSplit_TransToCard(byFlag, byCard);

			//ת����ͬ��˳λͼ
			word wFlushBitmap;
			FastSplit_TransToFlushBitmap(i, byCard, wFlushBitmap);

			//���ڷ�ֹ�ظ�����
			OriFlushBitmap[i] = wFlushBitmap;

			//��ý��ȼ�����
			int nSplitIndex;
			FastSplit_TransToSplitIndex(byFlag, nSplitIndex);

			//�ж��Ƿ���Ҫ���²���
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

		//���浽�ļ�
		SaveTblFastSplit(L"TblFastSplit.tbl");
	}
	else
	{
		//���ļ�����
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

			//�ֶ�д���ļ�
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
		//�ֶζ����ļ�
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
			//ת����byFlag
			byte byFlag[8];
			if (!FastSplit_TransToFlag(i, byFlag))
				continue;

			//ת����byCard
			byte byCard[8];
			FastSplit_TransToCard(byFlag, byCard);

			//ת����ͬ��˳λͼ
			word wFlushBitmap;
			FastSplit_TransToFlushBitmap(i, byCard, wFlushBitmap);

			//���ڷ�ֹ�ظ�����
			OriFlushBitmap[i] = wFlushBitmap;

			//��ý��ȼ�����
			int nSplitIndex;
			FastSplit_TransToSplitIndex(byFlag, nSplitIndex);

			//�ж��Ƿ���Ҫ���²���
			if (wFlushBitmap == OriFlushBitmap[nSplitIndex]
				&& TblCoFastSplit[0][nSplitIndex] != 0)
			{
				for (int j = 0; j < 8; j++)
					TblCoFastSplit[j][i] = TblCoFastSplit[j][nSplitIndex];
				continue;
			}

			//�����Ĳ��Ʒ���
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

		//���浽�ļ�
		SaveTblCoFastSplit(L"TblCoFastSplit.tbl");
	}
	else
	{
		//���ļ�����
		LoadTblCoFastSplit(L"TblCoFastSplit.tbl");
	}
}
