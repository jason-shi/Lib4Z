#include "StdAfx.h"
#include "File.h"
#include "ZCtrl.h"

CImage				 ImageCtrl;

CZCtrl::CZCtrl()
{
}

CZCtrl::~CZCtrl()
{
}

bool CZCtrl::Initial()
{
	if (!ImageCtrl.LoadImage(L"PNG\\Ctrl.png"))
		return false;

	return true;
}

void CZCtrl::Release()
{
}
