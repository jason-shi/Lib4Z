#include "StdAfx.h"
#include "Table.h"

byte				*TblBitCount;
word				*TblShunZi;
word				*TblSanShun;
word				*TblShunPrhL;
word				*TblShunPrhH;
word				*TblShunPrhS;
word				*TblShunPrhE;
word				*TblMiddlePrh;
word				*TblContraryMask;

void CreateBitCount()
{
	int T[16] = { 0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, };

	TblBitCount = new byte[256];

	for (int i = 0; i < 256; i++)
		TblBitCount[i] = T[i >> 4] + T[i & 15];
}

void CreateShunZi()
{
	TblShunZi = new word[4096];
	memset(TblShunZi, 0, sizeof(word [4096]));

	int k;
	for (int i = 0; i < 4096; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if (!(i & (1 << j)) || !(i & (1 << (j + 1))) || !(i & (1 << (j + 2)))) //连续3位
				continue;
			for (k = j + 3; k < 12; k++)
			{
				if (!(i & (1 << k))) //更多连续位
					break;
			}
			//构造从j到k的掩码
			TblShunZi[i] = ((1 << k) - 1) ^ ((1 << j) - 1);
			break;
		}
	}
}

void CreateSanShun()
{
	TblSanShun = new word[4096];
	memset(TblSanShun, 0, sizeof(word [4096]));

	for (int i = 0; i < 4096; i++)
	{
		int bitmap = i;
		while (TblShunZi[bitmap] != 0)
		{
			word shunzi = TblShunZi[bitmap];
			if (WORD_BIT_COUNT(shunzi) == 3)
				TblSanShun[i] |= shunzi;

			bitmap ^= shunzi;
		}
	}
}

void CreateShunPrhL()
{
	TblShunPrhL = new word[4096];
	memset(TblShunPrhL, 0, sizeof(word [4096]));

	for (int i = 0; i < 4096; i++)
	{
		int bitmap = i;
		while (TblShunZi[bitmap] != 0)
		{
			TblShunPrhL[i] |= TblShunZi[bitmap];
			bitmap ^= TblShunZi[bitmap];
		}
		TblShunPrhL[i] &= ~(i & -i); //为了放牌
	}
}

void CreateShunPrhH()
{
	TblShunPrhH = new word[4096];
	memset(TblShunPrhH, 0, sizeof(word [4096]));

	for (int i = 0; i < 4096; i++)
	{
		int bitmap = i;
		while (TblShunZi[bitmap] != 0)
		{
			TblShunPrhH[i] |= TblShunZi[bitmap];
			bitmap ^= TblShunZi[bitmap];
		}

		if (TblShunPrhH[i] == 0)
			continue;
		for (int j = 0x8000; j; j >>= 1)
		{
			if (j & i)
			{
				TblShunPrhH[i] &= ~j; //为了顶牌
				break;
			}
		}
	}
}

void CreateShunPrhS()
{
	TblShunPrhS = new word[4096];
	memset(TblShunPrhS, 0, sizeof(word [4096]));

	for (int i = 0; i < 4096; i++)
	{
		int bitmap = i;
		while (TblShunZi[bitmap] != 0)
		{
			TblShunPrhS[i] |= TblShunZi[bitmap];
			bitmap ^= TblShunZi[bitmap];
		}
	}
}

void CreateShunPrhE()
{
	TblShunPrhE = new word[4096];
	memset(TblShunPrhE, 0, sizeof(word [4096]));

	for (int i = 0; i < 4096; i++)
	{
		int bitmap = TblShunPrhS[i];
		while (TblShunZi[bitmap] != 0)
		{
			//最低一位
			word _bitmap = bitmap & -bitmap;
			TblShunPrhE[i] |= _bitmap;

			//最高一位
			int nCount = WORD_BIT_COUNT(TblShunZi[bitmap]);
			_bitmap <<= nCount - 1;
			TblShunPrhE[i] |= _bitmap;

			bitmap ^= TblShunZi[bitmap];
		}
	}
}

void CreateMiddlePrh()
{
	TblMiddlePrh = new word[32768];
	memset(TblMiddlePrh, 0, sizeof(word [32768]));

	for (int i = 1, temp = i; i < 32768; i++, temp = i)
	{
		TblMiddlePrh[i] |= temp & -temp; //末位
		if (temp &= ~TblMiddlePrh[i])
		{
			TblMiddlePrh[i] |= temp & -temp; //次末位
			if (temp &= ~TblMiddlePrh[i])
			{
				while (WORD_BIT_COUNT(temp) != 1)
					temp ^= temp & -temp;
				TblMiddlePrh[i] |= temp;
			}
		}
	}
}

void CreateContraryMask()
{
	TblContraryMask = new word[16];

	for (int i = 0; i < 15; i++)
		TblContraryMask[i] = ~((1 << (i + 1)) - 1);

	TblContraryMask[15] = 0xFFFF;
}

void CreateAllTable()
{
	CreateBitCount();
	CreateShunZi();
	CreateSanShun();
	CreateShunPrhL();
	CreateShunPrhH();
	CreateShunPrhS();
	CreateShunPrhE();
	CreateMiddlePrh();
	CreateContraryMask();
}

void DeleteAllTable()
{
	delete TblBitCount;
	delete TblShunZi;
	delete TblSanShun;
	delete TblShunPrhL;
	delete TblShunPrhH;
	delete TblShunPrhS;
	delete TblShunPrhE;
	delete TblMiddlePrh;
	delete TblContraryMask;
}