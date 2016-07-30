#include "StdAfx.h"
#include "Generate\Generate.h"
#include "MoveList\MoveList.h"

void GenEludeUnlimited()
{
	if (ky != 9)
		::AddKillBlk(kpos, kpos + 16);
	if (kx != 3)
		::AddKillBlk(kpos, kpos - 1);
	if (kx != 5)
		::AddKillBlk(kpos, kpos + 1);
	if (ky != 7)
		::AddKillBlk(kpos, kpos - 16);
}

void GenEludeLimitedH(int limit)
{
	if (ky != 9)
		::AddKillBlk(kpos, kpos + 16);
	if (kx != 3 && limit == kpos - 1)
		::AddKillBlk(kpos, kpos - 1);
	if (kx != 5 && limit == kpos + 1)
		::AddKillBlk(kpos, kpos + 1);
	if (ky != 7)
		::AddKillBlk(kpos, kpos - 16);
}

void GenEludeLimitedV(int limit)
{
	if (ky != 9 && limit == kpos + 16)
		::AddKillBlk(kpos, kpos + 16);
	if (kx != 3)
		::AddKillBlk(kpos, kpos - 1);
	if (kx != 5)
		::AddKillBlk(kpos, kpos + 1);
	if (ky != 7 && limit == kpos - 16)
		::AddKillBlk(kpos, kpos - 16);
}
