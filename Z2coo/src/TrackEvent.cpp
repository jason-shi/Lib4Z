#include "StdAfx.h"

void CheckTrackEvent()
{
	UINT st = GetTickCount();
	MSG msg;
	while (true)
	{
		UINT et = GetTickCount();
		if (et - st > 100)
			break;
	}
	::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);

	g_pZ2coo->SendMessage(WM_MOUSELEAVE, msg.wParam, msg.lParam);
	g_pZ2coo->SendMessage(WM_MOUSEHOVER, msg.wParam, msg.lParam);
}
