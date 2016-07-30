#include "StdAfx.h"
#include "Generate\Generate.h"

void GenRookBlk(int check)
{
	int chx = check & 0xF;
	int chy = check >> 4;
	int dist = check - kpos;
	if (chx == kx)
	{
		::GenEludeLimitedV(check); //黑将限制性逃跑
		if (dist < -32)
			::GenBlockRookV(chy + 1, ky - 1);
		else if (dist == -32)
			::GenBlockSingle(check + 16);
		else if (dist == 32)
			::GenBlockSingle(check - 16);
	}
	else
	{
		::GenEludeLimitedH(check); //黑将限制性逃跑
		if (dist < -2)
			::GenBlockRookH(chx + 1, kx - 1);
		else if (dist == -2)
			::GenBlockSingle(check + 1);
		else if (dist > 2)
			::GenBlockRookH(kx + 1, chx - 1);
		else if (dist == 2)
			::GenBlockSingle(check - 1);
	}
}

void GenCannonBlk(int check)
{
	int chx = check & 0xF;
	int chy = check >> 4;
	int dist = check - kpos;
	int shell;
	if (chx == kx)
	{
		shell = dist < 0 ? TblFirstBit[kbtm] : 8;
		::GenEludeLimitedV(kx + (shell << 4)); //黑将限制性逃跑
		if (dist < -48)
			::GenBlockCannonV(chy + 1, ky - 1, shell);
		else if (dist == -48)
			::GenBlockSingle(check + ((ky - shell) << 4));
		::GenShellEscapeV(kx + (shell << 4)); //闪开炮架
	}
	else
	{
		shell = dist < 0 ? TblFirstBit[klft] : TblLastBit[krgt];
		::GenEludeLimitedH(shell + (ky << 4)); //黑将限制性逃跑
		if (dist < -3)
			::GenBlockCannonH(chx + 1, kx - 1, shell);
		else if (dist == -3)
			::GenBlockSingle(check + kx - shell);
		else if (dist > 3)
			::GenBlockCannonH(kx + 1, chx - 1, shell);
		else if (dist == 3)
			::GenBlockSingle(check + kx - shell);
		::GenShellEscapeH(shell + (ky << 4)); //闪开炮架
	}
}
