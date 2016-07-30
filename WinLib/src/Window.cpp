#include "StdAfx.h"

CWindow *CWindow::WindowListTop;
CWindow *CWindow::WindowInitPtr;

CWindow::CWindow()
{
	App = 0;
	hWnd = 0;
	LastMsg = WM_NULL;
	LastWParam = 0;
	LastLParam = 0;

	hAccelTable = 0;

	Next = 0;
}

CWindow::~CWindow()
{
}

CWindow *CWindow::GetWindow(HWND hwnd)
{
	for (CWindow *p=WindowListTop; p; p=p->Next) {
		if (p->hWnd == hwnd)
			return p;
	}
	return 0;
}

BOOL CWindow::AddWindowList(HWND hWndNew)
{
	Next = WindowListTop;
	WindowListTop = this;
	hWnd = hWndNew;
	return TRUE;
}

HWND CWindow::DeleteWindowList()
{
	for (CWindow **p = &WindowListTop; *p; p = &((*p)->Next)) {
		if (*p == this) {
			*p = Next;
			break;
		}
	}
	return hWnd;
}

BOOL CWindow::PreCreateWindow(CREATESTRUCT &cs)
{
	return TRUE;
}

BOOL CWindow::Create(CWinApp *app, LPCTSTR name, HMENU hMenu, CWindow *parent)
{
	App = app;

	CREATESTRUCT cs;
	cs.dwExStyle = WS_EX_CLIENTEDGE;
	cs.lpszClass = NULL;
	cs.lpszName = name;
	cs.style = WS_OVERLAPPEDWINDOW;
	cs.x = CW_USEDEFAULT;
	cs.y = CW_USEDEFAULT;
	cs.cx = CW_USEDEFAULT;
	cs.cy = CW_USEDEFAULT;
	cs.hwndParent = parent? parent->hWnd: 0;
	cs.hMenu = hMenu;
	cs.hInstance = *app;
	cs.lpCreateParams = 0;

	if (!PreCreateWindow(cs))
		return FALSE;

	if (cs.lpszClass == 0) {
		cs.lpszClass = _T("Window");
		if (!Application->RegisterWndClass(cs.lpszClass, CS_VREDRAW | CS_HREDRAW))
			return FALSE;
	}

	WindowInitPtr = this;
	HWND hWnd = ::CreateWindowEx(cs.dwExStyle, cs.lpszClass,
		cs.lpszName, cs.style, cs.x, cs.y, cs.cx, cs.cy,
		cs.hwndParent, cs.hMenu, cs.hInstance, cs.lpCreateParams);

	if (WindowInitPtr != NULL) {
		WindowInitPtr = NULL;
		return FALSE;
	}
	if (hWnd == NULL)
		return FALSE;
	return TRUE;
}

BOOL CWindow::OnIdle(long count)
{
	return FALSE;
}

BOOL CWindow::LoadAccelTable(LPCTSTR name)
{
	hAccelTable = Application->LoadAccelerators(name);
	return (hAccelTable != NULL);
}

LRESULT CALLBACK CWindow::_WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CWindow *win = GetWindow(hwnd);

	if (win == 0 && WindowInitPtr != 0) {
		win = WindowInitPtr;
		WindowInitPtr->AddWindowList(hwnd);
		WindowInitPtr = 0;
	}
	if (win) {
		win->SetLastParam(uMsg, wParam, lParam);
		return win->WindowProc(uMsg, wParam, lParam);
	}
	return ::DefWindowProc(hwnd, uMsg, wParam, lParam);
}

BOOL CWindow::PreTranslateMessage(MSG *msg)
{
	if (msg->message >= WM_KEYFIRST && msg->message <= WM_KEYLAST) {
		return hAccelTable != NULL && ::TranslateAccelerator(hWnd, hAccelTable, msg);
	}
	return FALSE;
}

LRESULT CWindow::WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		if (!OnCreate((LPCREATESTRUCT)lParam))
			return -1;
		break;

	case WM_PAINT:
		OnPaint();
		break;

	case WM_DESTROY:
		OnDestroy();
		break;

	case WM_COMMAND:
		OnCommand(HIWORD(wParam), LOWORD(wParam), (HWND)lParam);
		break;

	case WM_INITMENUPOPUP:
		OnInitMenuPopup((HMENU)wParam, LOWORD(lParam), HIWORD(lParam));
		break;

	case WM_NCDESTROY:
		DeleteWindowList();
		break;

	default:
		return Default();
	}
	return 0L;
}

BOOL CWindow::OnCreate(CREATESTRUCT *cs)
{
	return TRUE;
}

void CWindow::OnDestroy()
{
	::PostQuitMessage(0);
}

void CWindow::OnPaint()
{
	CPaintDC dc(this); //这是为了调用在 OnPaint 里产生 CPaintDC 的BeginPaint和EndPaint
}

void CWindow::OnCommand(UINT notifyCode, UINT id, HWND ctrl)
{
}

void CWindow::OnInitMenuPopup(HMENU hMenu, UINT, BOOL sysMenu)
{
	//每个菜单项目都要调用OnInitSubMenu
	if (sysMenu)
		return;

	int indexMax = ::GetMenuItemCount(hMenu);
	for (int index=0; index < indexMax; index++) {
		OnInitSubMenu(hMenu, ::GetMenuItemID(hMenu, index));
	}
}

void CWindow::OnInitSubMenu(HMENU hMenu, UINT id)
{
}
