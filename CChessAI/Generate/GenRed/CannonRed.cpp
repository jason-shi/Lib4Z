#include "StdAfx.h"
#include "Generate\Generate.h"
#include "MoveList\MoveList.h"

static void			 LeaveVForCannon(int pos, int chk);
static void			 LeaveHForCannon(int pos, int chk);
static void			 BlockVForRed(int begin, int end, int chk);
static void			 BlockHForRed(int begin, int end, int chk);

void GenCannonRed(int pos)
{
	int px = pos & 0xF;
	int py = pos >> 4;
	int cross; //交叉点或打将的位置
	if (px == kx)
	{
		if (py > ky)
		{
			//挡将，炮前打将
			::BlockVForRed(ky + 1, py - 1, pos);
			if (board[pos + 16] == R_ROOK)
				::AddKillRed(pos, kb1, pos + 16);
		}
		else
		{
			int mkmidd = vert[kx] & TblBitrim[(py << 4) + ky];
			int mkside;
			switch (TblNumberOne[mkmidd])
			{
			case 0: //挡将，炮前打将，炮后打将
				::BlockVForRed(py + 1, ky - 1, pos);
				if (mkside = (vert[kx] & TblHighMask[ky]))
				{
					if (board[kb2] == R_ROOK)
						::AddKillRed(pos, kx + (TblLastBit[mkside] << 4), kb2);
					else if (board[kb3] == R_CANNON)
						::AddKillRed(pos, kx + (TblLastBit[mkside] << 4), kb3);
				}
				::AddKillRed(pos, kb3, kb3);
				break;
			case 2:
				if (board[kb2] != R_ROOK && board[kb2] != R_KING) //(--各闪车将)(--对光将)
					::LeaveVForCannon(kb1, pos);
				::LeaveVForCannon(kb2, pos);
				break;
			case 3:
				::AddKillRed(pos, kb2, kb2);
				break;
			}
		}
	}
	else if (py == ky)
	{
		int mkmidd = horz[ky] & TblBitrim[(px << 4) + kx];
		if (px < kx)
		{
			switch (TblNumberOne[mkmidd])
			{
			case 0: //挡将，炮前打将，炮后打将
				::BlockHForRed(px + 1, kx - 1, pos);
				if (board[kl2] == R_ROOK)
					::AddKillRed(pos, kr1, kl2);
				else if (board[kl3] == R_CANNON)
					::AddKillRed(pos, kr1, kl3);
				::AddKillRed(pos, kl3, kl3);
				break;
			case 2:
				if (board[kl2] != R_ROOK) //(--各闪车将)
					::LeaveHForCannon(kl1, pos);
				::LeaveHForCannon(kl2, pos);
				break;
			case 3:
				::AddKillRed(pos, kl2, kl2);
				break;
			}
		}
		else
		{
			switch (TblNumberOne[mkmidd])
			{
			case 0: //挡将，炮前打将，炮后打将
				::BlockHForRed(kx + 1, px - 1, pos);
				if (board[kr2] == R_ROOK)
					::AddKillRed(pos, kl1, kr2);
				else if (board[kr3] == R_CANNON)
					::AddKillRed(pos, kl1, kr3);
				::AddKillRed(pos, kr3, kr3);
				break;
			case 2:
				if (board[kr2] != R_ROOK) //(--各闪车将)
					::LeaveHForCannon(kr1, pos);
				::LeaveHForCannon(kr2, pos);
				break;
			case 3:
				::AddKillRed(pos, kr2, kr2);
				break;
			}
		}
	}
	else //in different line
	{
		int htrim = TblBitrim[(px << 4) + kx];
		int vtrim = TblBitrim[(py << 4) + ky];
		cross = px + (ky << 4); //move vertical
		if (TblNumberOne[horz[ky] & htrim] == 1 && TblNumberOne[vert[px] & vtrim] == !!board[cross])
			::AddKillRed(pos, cross, cross);
		cross = kx + (py << 4); //move horizontal
		if (TblNumberOne[vert[kx] & vtrim] == 1 && TblNumberOne[horz[py] & htrim] == !!board[cross])
			::AddKillRed(pos, cross, cross);
	}
}

void CannonLeaveHly(int pos, int chk)
{
	int px = pos & 0xF;
	int py = pos >> 4;
	//向左移动和向左打吃
	int mside = horz[py] & TblLowMask[px];
	switch (mside ? px - TblFirstBit[mside] : px + 1)
	{
	case 7:
		::AddMoveRed(pos, pos - 6, chk);
	case 6:
		::AddMoveRed(pos, pos - 5, chk);
	case 5:
		::AddMoveRed(pos, pos - 4, chk);
	case 4:
		::AddMoveRed(pos, pos - 3, chk);
	case 3:
		::AddMoveRed(pos, pos - 2, chk);
	case 2:
		::AddMoveRed(pos, pos - 1, chk);
	}
	if (TblSecondBit[mside] != 10)
		::AddKillRed(pos, TblSecondBit[mside] + (py << 4), chk);
	//向右移动和向右打吃
	mside = horz[py] & TblHighMask[px];
	switch (TblLastBit[mside] - px - !mside)
	{
	case 7:
		::AddMoveRed(pos, pos + 6, chk);
	case 6:
		::AddMoveRed(pos, pos + 5, chk);
	case 5:
		::AddMoveRed(pos, pos + 4, chk);
	case 4:
		::AddMoveRed(pos, pos + 3, chk);
	case 3:
		::AddMoveRed(pos, pos + 2, chk);
	case 2:
		::AddMoveRed(pos, pos + 1, chk);
	}
	if (TblLagendBit[mside] != 10)
		::AddKillRed(pos, TblLagendBit[mside] + (py << 4), chk);
}

void CannonLeaveVly(int pos, int chk)
{
	int px = pos & 0xF;
	int py = pos >> 4;
	//向下移动和向下打吃
	int mside = vert[px] & TblLowMask[py];
	switch (mside ? py - TblFirstBit[mside] : py + 1)
	{
	case 10:
		::AddMoveRed(pos, pos - 144, chk);
	case 9:
		::AddMoveRed(pos, pos - 128, chk);
	case 8:
		::AddMoveRed(pos, pos - 112, chk);
	case 7:
		::AddMoveRed(pos, pos - 96, chk);
	case 6:
		::AddMoveRed(pos, pos - 80, chk);
	case 5:
		::AddMoveRed(pos, pos - 64, chk);
	case 4:
		::AddMoveRed(pos, pos - 48, chk);
	case 3:
		::AddMoveRed(pos, pos - 32, chk);
	case 2:
		::AddMoveRed(pos, pos - 16, chk);
	}
	if (TblSecondBit[mside] != 10)
		::AddKillRed(pos, px + (TblSecondBit[mside] << 4), chk);
	//向上移动和向上打吃
	mside = vert[px] & TblHighMask[py];
	switch (TblLastBit[mside] - py)
	{
	case 4:
		::AddMoveRed(pos, pos + 48, chk);
	case 3:
		::AddMoveRed(pos, pos + 32, chk);
	case 2:
		::AddMoveRed(pos, pos + 16, chk);
	}
	if (TblLagendBit[mside] != 10)
		::AddKillRed(pos, px + (TblLagendBit[mside] << 4), chk);
}

void LeaveVForCannon(int pos, int chk)
{
	switch (board[pos])
	{
	case R_ROOK:
		::RookLeaveHly(pos, chk);
		break;
	case R_HORSE:
		::HorseLeaveXly(pos, chk);
		break;
	case R_BISHOP:
		::BishopLeaveXly(pos, chk);
		break;
	case R_GUARD:
		::GuardLeaveXly(pos, chk);
		break;
	case R_CANNON:
		::CannonLeaveHly(pos, chk);
		break;
	case R_PAWN:
		if (pos >= 80)
		{
			::AddKillRed(pos, pos + 1, chk);
			::AddKillRed(pos, pos - 1, chk);
		}
		if (pos + 32 != kpos && board[pos + 16] >= B_ROOK) //(--兵同线将)
			::AddKillRed(pos, pos + 16, chk);
		break;
	case R_KING:
		if ((pos & 0xF) != 5)
			::AddKillRed(pos, pos + 1, chk);
		if ((pos & 0xF) != 3)
			::AddKillRed(pos, pos - 1, chk);
		break;
	}
}

void LeaveHForCannon(int pos, int chk)
{
	switch (board[pos])
	{
	case R_ROOK:
		::RookLeaveVly(pos, chk);
		break;
	case R_HORSE:
		::HorseLeaveXly(pos, chk);
		break;
	case R_CANNON:
		::CannonLeaveVly(pos, chk);
		break;
	case R_PAWN:
		::AddKillRed(pos, pos + 16, chk);
		if (pos + 2 != kpos && board[pos + 1] >= B_ROOK) //(--兵同线将)
			::AddKillRed(pos, pos + 1, chk);
		if (pos - 2 != kpos && board[pos - 1] >= B_ROOK) //(--兵同线将)
			::AddKillRed(pos, pos - 1, chk);
		break;
	}
}

void BlockVForRed(int begin, int end, int chk)
{
	if (begin > end) return; //make certain that end is greater than or equal to begin

	int px, py;
	TPiece *Piece = PieceRedC;
	while (Piece)
	{
		int pos = Piece->pos;
		GetPxPy(pos, px, py);
		if (py >= begin && py <= end && (horz[py] & TblBitrim[(px << 4) + kx]) == 0
			&& (pinmask & TblPinByRed[pos - kpos + 160]) == 0) //(--炮闪马将)
		{
			::AddMoveRed(pos, kx + (py << 4), chk);
		}
		Piece =	Piece->next;
	}
	//红马
	Piece = PieceRedH;
	while (Piece)
	{
		int pos = Piece->pos;
		GetPxPy(pos, px, py);
		if (px - kx == 2 && board[pos - 1] == 0 || px - kx == -2 && board[pos + 1] == 0)
		{
			if (py + 1 >= begin && py + 1 <= end && !::IsPinnedByRed(pos))
				::AddMoveRed(pos, kx + ((py + 1) << 4), chk);
			if (py - 1 >= begin && py - 1 <= end && !::IsPinnedByRed(pos))
				::AddMoveRed(pos, kx + ((py - 1) << 4), chk);
		}
		else if (px - kx == 1 || px - kx == -1)
		{
			if (py + 2 >= begin && py + 2 <= end && board[pos + 16] == 0 && !::IsPinnedByRed(pos))
				::AddMoveRed(pos, kx + ((py + 2) << 4), chk);
			if (py - 2 >= begin && py - 2 <= end && board[pos - 16] == 0 && !::IsPinnedByRed(pos))
				::AddMoveRed(pos, kx + ((py - 2) << 4), chk);
		}
		Piece =	Piece->next;
	}

	Piece = PieceRedS;
	while (Piece)
	{
		int pos = Piece->pos;
		switch (Piece->kind)
		{
		case R_BISHOP:
			if ((chk == 4 || chk == 20) && pos != 32 && pos != 40 && board[(pos + 36) >> 1] == 0)
				::AddMoveRed(pos, 36, chk);
			break;
		case R_GUARD:
			chk == 4 ? ::AddMoveRed(pos, 20, chk) : pos == 20 && begin <= 2 ? ::AddMoveRed(pos, kx + 32, chk) : 0;
			break;
		case R_PAWN: //(--兵闪马将)(--兵异线将)
			GetPxPy(pos, px, py);
			if ((px - kx == 1 || px - kx == -1) && py >= begin && py > 4
				&& (py < end || ky == 7 && py == 8 && end == 8 && board[pos + 16] != R_HORSE && board[pos + px - kx] != R_HORSE))
			{
				::AddMoveRed(pos, kx + (py << 4), chk);
			}
			break;
		}
		Piece =	Piece->next;
	}
}

void BlockHForRed(int begin, int end, int chk)
{
	if (begin > end) return; //make certain that end is greater than or equal to begin

	int px, py;
	TPiece *Piece = PieceRedC;
	while (Piece)
	{
		int pos = Piece->pos;
		GetPxPy(pos, px, py);
		if (px >= begin && px <= end && (vert[px] & TblBitrim[(py << 4) + ky]) == 0
			&& (pinmask & TblPinByRed[pos - kpos + 160]) == 0) //(--炮闪马将)
		{
			::AddMoveRed(pos, px + (ky << 4), chk);
		}
		Piece =	Piece->next;
	}
	//红马
	Piece = PieceRedH;
	while (Piece)
	{
		int pos = Piece->pos;
		GetPxPy(pos, px, py);
		if (py - ky == 2 && board[pos - 16] == 0 || py - ky == -2 && board[pos + 16] == 0)
		{
			if (px + 1 >= begin && px + 1 <= end && !::IsPinnedByRed(pos))
				::AddMoveRed(pos, px + 1 + (ky << 4), chk);
			if (px - 1 >= begin && px - 1 <= end && !::IsPinnedByRed(pos))
				::AddMoveRed(pos, px - 1 + (ky << 4), chk);
		}
		else if (py - ky == 1 || py - ky == -1)
		{
			if (px + 2 >= begin && px + 2 <= end && board[pos + 1] == 0 && !::IsPinnedByRed(pos) && (pos != kpos + 16 || board[kpos + 32] != R_ROOK))
				::AddMoveRed(pos, px + 2 + (ky << 4), chk);
			if (px - 2 >= begin && px - 2 <= end && board[pos - 1] == 0 && !::IsPinnedByRed(pos) && (pos != kpos + 16 || board[kpos + 32] != R_ROOK))
				::AddMoveRed(pos, px - 2 + (ky << 4), chk);
		}
		Piece =	Piece->next;
	}

	Piece = PieceRedS;
	while (Piece)
	{
		if (Piece->kind == R_PAWN)
		{
			int pos = Piece->pos;
			GetPxPy(pos, px, py);
			if (py == ky - 1 && (px >= begin && px < kx - 1 || px > kx + 1 && px <= end)) //(--兵异线将)
				::AddMoveRed(pos, pos + 16, chk);
		}
		Piece =	Piece->next;
	}
}
