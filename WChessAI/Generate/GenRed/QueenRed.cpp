#include "StdAfx.h"
#include "Generate\Generate.h"
#include "MoveList\MoveList.h"

void GenQueenRed(int pos)
{
	int nIndex = (pos << 6) + kpos;
	if (TblQueenPath[nIndex] != (qword)-1)
	{
		qword _bitmap = Z & TblQueenPath[nIndex];
		if ((_bitmap & _bitmap - 1) == 0)
		{
			int dest = TblBitIndex[_bitmap % 67];
			if (board[dest] >= B_ROOK)
				::AddKillRed(pos, dest, dest);
			else
			{
				switch (TblDirection[nIndex])
				{
				case 1:
					::LeaveHForRook(dest, pos);
					break;
				case 2:
					::LeaveVForRook(dest, pos);
					break;
				case 3:
					::LeaveSForBishop(dest, pos);
					break;
				case 4:
					::LeaveBForBishop(dest, pos);
					break;
				}
			}
		}
	}

	qword temp = TblQueenCrossGridBtmp[(pos << 6) + kpos];
	while (temp)
	{
		int dest = TblBitIndex[(temp & -temp) % 67];
		temp &= temp - 1;

		//Á½ÌõÂ·¾¶
		qword path1 = TblQueenPath[(dest << 6) + pos];
		qword path2 = TblQueenPath[(dest << 6) + kpos];
		if ((Z & (path1 | path2)) == 0)
			::AddKillRed(pos, dest, dest);
	}
}
