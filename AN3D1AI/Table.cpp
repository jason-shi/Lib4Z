#include "StdAfx.h"
#include "Table.h"

byte				*TblBitCount;
byte				*TblMaxGrade;
byte				*TblMaxScore;
byte				*TblPutScore;
word				*TblContraryMask;
byte				*TblMinorGrade;
byte				*TblMajorGrade;

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

void CreateMaxScore()
{
	TblMaxScore = new byte[2048];

	for (int i = 0; i < 2048; i++)
		TblMaxScore[i] = i & MASK_TK ? 10 : i & MASK_5 ? 5 : 0;
}

void CreatePutScore()
{
	TblPutScore = new byte[2048];

	for (int i = 0; i < 2048; i++)
		TblPutScore[i] = i & MASK_K ? 10 : i & MASK_T ? 7 : i & MASK_5 ? 2 : 0;
}

void CreateContraryMask()
{
	TblContraryMask = new word[17];

	for (int i = 0; i < 16; i++)
		TblContraryMask[i] = ~((1 << (i + 1)) - 1);

	TblContraryMask[16] = 0xFFFF;
}

void CreateMinorGrade()
{
	TblMinorGrade = new byte[4096];

	for (int i = 0; i < 4096; i++)
		TblMinorGrade[i] = TblBitIndex[(i & -i) % 37];
}

void CreateMajorGrade()
{
	TblMajorGrade = new byte[16];

	for (int i = 0; i < 16; i++)
		TblMajorGrade[i] = TblBitIndex[(i & -i) % 37] + 12;
}

void CreateAllTables()
{
	CreateBitCount();
	CreateMaxGrade();
	CreateMaxScore();
	CreatePutScore();
	CreateContraryMask();
	CreateMinorGrade();
	CreateMajorGrade();
}

void DeleteAllTables()
{
	delete TblBitCount;
	delete TblMaxGrade;
	delete TblMaxScore;
	delete TblPutScore;
	delete TblContraryMask;
	delete TblMinorGrade;
	delete TblMajorGrade;
}
