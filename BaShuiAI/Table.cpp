#include "StdAfx.h"
#include "Table.h"

using namespace std;

word				*TblContraryMask = NULL;
byte				*TblBitCount = NULL;
byte				*TblMaxGrade = NULL;
word				*TblShunZi = NULL;
word				*TblSanShun = NULL;
word				*TblSister = NULL;
byte				*TblSortCard = NULL;
dword				*TblFastCompare = NULL;
dword				*TblFastCompareNT = NULL;
dword				*TblFastSplit = NULL;
dword				*TblCoFastSplit[8] = { NULL };

extern void			 CreateFastCompare();
extern void			 CreateFastCompareNT();
extern void			 CreateFastSplit();
extern void			 CreateCoFastSplit();

void CreateContraryMask()
{
	TblContraryMask = new word[14];
	memset(TblContraryMask, 0, sizeof(word [14]));

	for (int i = 0; i < 13; i++)
		TblContraryMask[i] = ~((1 << (i + 1)) - 1);

	TblContraryMask[13] = 0xFFFF;
}

void CreateBitCount()
{
	byte T[16] = { 0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4, };

	TblBitCount = new byte[256];

	for (int i = 0; i < 256; i++)
		TblBitCount[i] = T[i >> 4] + T[i & 15];
}

void CreateMaxGrade()
{
	byte T[16] = { 0, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3, };

	TblMaxGrade = new byte[256];

	for (int i = 0; i < 256; i++)
		TblMaxGrade[i] = i & 0xF0 ? T[i >> 4] + 4 : T[i];
}

void CreateSister()
{
	TblSister = new word[4096];
	memset(TblSister, 0, sizeof(word [4096]));

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
			TblSister[i] = ((1 << k) - 1) ^ ((1 << j) - 1);
			break;
		}
	}
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
			if ((i & (0x07 << j)) != 0x07 << j)
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
			if (WORD_BIT_COUNT(shunzi) >= 3)
				TblSanShun[i] |= shunzi;

			bitmap ^= shunzi;
		}
	}
}

void CreateSortCard()
{
	TblSortCard = new byte[256];

	for (int i = 0; i < 256; i++)
		TblSortCard[i] = ((i & 0xF) << 4) + ((i >> 4) & 0xF);
}

void CreateAllTables()
{
	CreateContraryMask();
	CreateBitCount();
	CreateMaxGrade();
	CreateSister();
	CreateShunZi();
	CreateSanShun();
	CreateSortCard();
	CreateFastCompare();
	CreateFastCompareNT();
	CreateFastSplit();
	CreateCoFastSplit();
}

void DeleteAllTables()
{
	delete TblContraryMask;
	delete TblBitCount;
	delete TblMaxGrade;
	delete TblSister;
	delete TblShunZi;
	delete TblSanShun;
	delete TblSortCard;
	delete TblFastCompare;
	delete TblFastCompareNT;
	delete TblFastSplit;

	for (int i = 0; i < 8; i++)
		delete TblCoFastSplit[i];
}
