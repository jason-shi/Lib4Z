#include "StdAfx.h"

CDC::~CDC()
{
}

CClientDC::CClientDC(CWindow *win)
{
	hWnd = *win;
	hDC = ::GetDC(hWnd);
}

CClientDC::~CClientDC()
{
	::ReleaseDC(hWnd, hDC);
}

CPaintDC::CPaintDC(CWindow *win)
{
	hWnd = *win;
	hDC = ::BeginPaint(hWnd, &ps);
}

CPaintDC::~CPaintDC()
{
	::EndPaint(hWnd, &ps);
}

CMemoryDC::CMemoryDC(HDC hdc)
{
	hDC = ::CreateCompatibleDC(hdc);
}

CMemoryDC::~CMemoryDC()
{
	::DeleteDC(hDC);
}

CScreenDC::CScreenDC()
{
	hDC = ::GetDC(0);
}

CScreenDC::~CScreenDC()
{
	::ReleaseDC(0, hDC);
}
