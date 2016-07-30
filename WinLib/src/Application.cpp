#include "StdAfx.h"

#ifdef __BORLANDC__
static int WINAPI (*WinMainPtr)(HINSTANCE, HINSTANCE, LPSTR, int);
#endif

CWinApp *Application;

CWinApp::CWinApp()
{
#ifdef	_DEBUG
	if (Application != 0) {
		TRACE("CWinApp 已经登录注册完毕。\n");
	}
#endif
	Instance = 0;
	CmdLine = 0;
	CmdShow = 0;

	Application = this;
	msgCur.message = WM_NULL;
	MainWnd = 0;

#ifdef __BORLANDC__
	// WinMain参照的指定
	// 这个动作并无意义
	WinMainPtr = &WinMain;
#endif
}

CWinApp::~CWinApp()
{
	Application = 0;
}

BOOL CWinApp::InitInstance()
{
	return TRUE;
}

BOOL CWinApp::OnIdle(long count)
{
	return MainWnd && MainWnd->OnIdle(count);
}

int CWinApp::Run()
{
	bool idle = false;
	long count = 0;

	for (;;) {
		if (::PeekMessage(&msgCur, NULL, 0, 0, PM_NOREMOVE)) {
			if (!::GetMessage(&msgCur, NULL, 0, 0))
				return msgCur.wParam;

			if (!PreTranslateMessage(&msgCur)) {
				::TranslateMessage(&msgCur);
				::DispatchMessage(&msgCur);
			}
			idle = false;
			count = 0;
		}
		else if (idle) {
			::WaitMessage();
		}
		else {
			if (OnIdle(count++))
				idle = true;
		}
	}
}

BOOL CWinApp::PreTranslateMessage(MSG *msg)
{
	return MainWnd->PreTranslateMessage(msg);
}

//
// 窗口类别登录注册
//
//	同一名称不可重复登录注册
//
BOOL CWinApp::RegisterWndClass(LPCTSTR name, UINT classStyle, HCURSOR hCursor,
	HBRUSH hbrBackground, HICON hIcon)
{
	WNDCLASS	wndclass;

	// 是否已经登录注册
	if (!GetClassInfo(Instance, name, &wndclass)) {		
		wndclass.style = classStyle;
		wndclass.lpfnWndProc = CWindow::_WindowProc;
		wndclass.cbClsExtra = 0;
		wndclass.cbWndExtra = 0;
		wndclass.hInstance = Instance;
		wndclass.hIcon = hIcon;
		wndclass.hCursor = hCursor;
		wndclass.hbrBackground = hbrBackground;
		wndclass.lpszMenuName = 0;
		wndclass.lpszClassName = name;

		if (!::RegisterClass(&wndclass)) {
			TRACE("Can't register window class [%s]\n", name);
			return FALSE;
		}
	}
	return TRUE;
}
