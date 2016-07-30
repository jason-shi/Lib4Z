#include "StdAfx.h"
#include "Table.h"

byte				*TblBitCount;
word				*TblShunZi2;
word				*TblShunZi3;
word				*TblShunZi5;
word				*TblSanShun;
word				*TblFiveShun;
word				*TblShun5PrhL;
word				*TblShun5PrhH;
word				*TblShun3PrhH;
word				*TblShun5PrhS;
word				*TblShun3PrhS;
word				*TblShun5PrhE;
word				*TblShun3PrhE;
word				*TblMiddlePrh;
word				*TblContraryMask;

void CreateBitCount()
{
	int T[16] = { 0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, };

	TblBitCount = new byte[256];
	memset(TblBitCount, 0, sizeof(byte [256]));

	for (int i = 0; i < 256; i++)
		TblBitCount[i] = T[i >> 4] + T[i & 15];
}

void CreateShunZi2()
{
	TblShunZi2 = new word[4096];
	memset(TblShunZi2, 0, sizeof(word [4096]));

	int k;
	for (int i = 0; i < 4096; i++)
	{
		for (int j = 0; j < 11; j++)
		{
			if ((i & (0x03 << j)) != 0x03 << j)
				continue;
			for (k = j + 2; k < 12; k++)
			{
				if (!(i & (1 << k))) //更多连续位
					break;
			}
			//构造从j到k的掩码
			TblShunZi2[i] = ((1 << k) - 1) ^ ((1 << j) - 1);
			break;
		}
	}
}

void CreateShunZi3()
{
	TblShunZi3 = new word[4096];
	memset(TblShunZi3, 0, sizeof(word [4096]));

	int k;
	for (int i = 0; i < 4096; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			if ((i & (0x07 << j)) != 0x07 << j)
				continue;
			for (k = j + 3; k < 12; k++)
			{
				if (!(i & (1 << k))) //更多连续位
					break;
			}
			//构造从j到k的掩码
			TblShunZi3[i] = ((1 << k) - 1) ^ ((1 << j) - 1);
			break;
		}
	}
}

void CreateShunZi5()
{
	TblShunZi5 = new word[4096];
	memset(TblShunZi5, 0, sizeof(word [4096]));

	int k;
	for (int i = 0; i < 4096; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if ((i & (0x1F << j)) != 0x1F << j)
				continue;
			for (k = j + 5; k < 12; k++)
			{
				if (!(i & (1 << k))) //更多连续位
					break;
			}
			//构造从j到k的掩码
			TblShunZi5[i] = ((1 << k) - 1) ^ ((1 << j) - 1);
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
		while (TblShunZi3[bitmap] != 0)
		{
			word shunzi = TblShunZi3[bitmap];
			if (WORD_BIT_COUNT(shunzi) == 3)
				TblSanShun[i] |= shunzi;

			bitmap ^= shunzi;
		}
	}
}

void CreateFiveShun()
{
	TblFiveShun = new word[4096];
	memset(TblFiveShun, 0, sizeof(word [4096]));

	for (int i = 0; i < 4096; i++)
	{
		int bitmap = i;
		while (TblShunZi5[bitmap] != 0)
		{
			word shunzi = TblShunZi5[bitmap];
			if (WORD_BIT_COUNT(shunzi) == 5)
				TblFiveShun[i] |= shunzi;

			bitmap ^= shunzi;
		}
	}
}

void CreateShun5PrhL()
{
	TblShun5PrhL = new word[4096];
	memset(TblShun5PrhL, 0, sizeof(word [4096]));

	for (int i = 0; i < 4096; i++)
	{
		int bitmap = i;
		while (TblShunZi5[bitmap] != 0)
		{
			TblShun5PrhL[i] |= TblShunZi5[bitmap];
			bitmap ^= TblShunZi5[bitmap];
		}
		TblShun5PrhL[i] &= ~(i & -i); //为了放牌
	}
}

void CreateShun5PrhH()
{
	TblShun5PrhH = new word[4096];
	memset(TblShun5PrhH, 0, sizeof(word [4096]));

	for (int i = 0; i < 4096; i++)
	{
		int bitmap = i;
		while (TblShunZi5[bitmap] != 0)
		{
			TblShun5PrhH[i] |= TblShunZi5[bitmap];
			bitmap ^= TblShunZi5[bitmap];
		}

		if (TblShun5PrhH[i] == 0)
			continue;
		for (int j = 0x8000; j; j >>= 1)
		{
			if (j & i)
			{
				TblShun5PrhH[i] &= ~j; //为了顶牌
				break;
			}
		}
	}
}

void CreateShun3PrhH()
{
	TblShun3PrhH = new word[4096];
	memset(TblShun3PrhH, 0, sizeof(word [4096]));

	for (int i = 0; i < 4096; i++)
	{
		int bitmap = i;
		while (TblShunZi3[bitmap] != 0)
		{
			TblShun3PrhH[i] |= TblShunZi3[bitmap];
			bitmap ^= TblShunZi3[bitmap];
		}

		if (TblShun3PrhH[i] == 0)
			continue;
		for (int j = 0x8000; j; j >>= 1)
		{
			if (j & i)
			{
				TblShun3PrhH[i] &= ~j; //为了顶牌
				break;
			}
		}
	}
}

void CreateShun5PrhS()
{
	TblShun5PrhS = new word[4096];
	memset(TblShun5PrhS, 0, sizeof(word [4096]));

	for (int i = 0; i < 4096; i++)
	{
		int bitmap = i;
		while (TblShunZi5[bitmap] != 0)
		{
			TblShun5PrhS[i] |= TblShunZi5[bitmap];
			bitmap ^= TblShunZi5[bitmap];
		}
	}
}

void CreateShun3PrhS()
{
	TblShun3PrhS = new word[4096];
	memset(TblShun3PrhS, 0, sizeof(word [4096]));

	for (int i = 0; i < 4096; i++)
	{
		int bitmap = i;
		while (TblShunZi3[bitmap] != 0)
		{
			TblShun3PrhS[i] |= TblShunZi3[bitmap];
			bitmap ^= TblShunZi3[bitmap];
		}
	}
}

void CreateShun5PrhE()
{
	TblShun5PrhE = new word[4096];
	memset(TblShun5PrhE, 0, sizeof(word [4096]));

	for (int i = 0; i < 4096; i++)
	{
		int bitmap = TblShun5PrhS[i];
		while (TblShunZi5[bitmap] != 0)
		{
			//最低一位
			word _bitmap = bitmap & -bitmap;
			TblShun5PrhE[i] |= _bitmap;

			//最高一位
			int nCount = WORD_BIT_COUNT(TblShunZi5[bitmap]);
			_bitmap <<= nCount - 1;
			TblShun5PrhE[i] |= _bitmap;

			bitmap ^= TblShunZi5[bitmap];
		}
	}
}

void CreateShun3PrhE()
{
	TblShun3PrhE = new word[4096];
	memset(TblShun3PrhE, 0, sizeof(word [4096]));

	for (int i = 0; i < 4096; i++)
	{
		int bitmap = TblShun3PrhS[i];
		while (TblShunZi3[bitmap] != 0)
		{
			//最低一位
			word _bitmap = bitmap & -bitmap;
			TblShun3PrhE[i] |= _bitmap;

			//最高一位
			int nCount = WORD_BIT_COUNT(TblShunZi3[bitmap]);
			_bitmap <<= nCount - 1;
			TblShun3PrhE[i] |= _bitmap;

			bitmap ^= TblShunZi3[bitmap];
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
	memset(TblContraryMask, 0, sizeof(word [16]));

	for (int i = 0; i < 15; i++)
		TblContraryMask[i] = ~((1 << (i + 1)) - 1);

	TblContraryMask[15] = 0xFFFF;
}

void CreateAllTables()
{
	CreateBitCount();
	CreateShunZi2();
	CreateShunZi3();
	CreateShunZi5();
	CreateSanShun();
	CreateFiveShun();
	CreateShun5PrhL();
	CreateShun5PrhH();
	CreateShun3PrhH();
	CreateShun5PrhS();
	CreateShun3PrhS();
	CreateShun5PrhE();
	CreateShun3PrhE();
	CreateMiddlePrh();
	CreateContraryMask();
}

void DeleteAllTables()
{
	delete TblBitCount;
	delete TblShunZi2;
	delete TblShunZi3;
	delete TblShunZi5;
	delete TblSanShun;
	delete TblFiveShun;
	delete TblShun5PrhL;
	delete TblShun5PrhH;
	delete TblShun3PrhH;
	delete TblShun5PrhS;
	delete TblShun3PrhS;
	delete TblShun5PrhE;
	delete TblShun3PrhE;
	delete TblMiddlePrh;
	delete TblContraryMask;
}
