#ifndef __Window_h__
#define __Window_h__


class CWinApp;
class CWindow {
  public:
	CWindow();
	virtual ~CWindow();

  public:
	static CWindow	*GetWindow(HWND hwnd);
	operator HWND() const { return hWnd; }
	BOOL			 AddWindowList(HWND hWndNew);
	HWND			 DeleteWindowList();

  public:
	virtual BOOL	 PreCreateWindow(CREATESTRUCT &cs);
	BOOL			 Create(CWinApp *app, LPCTSTR name, HMENU hMenu, CWindow *parent = 0);
	BOOL			 Create(CWinApp *app, LPCTSTR name, CWindow *parent);
	virtual BOOL	 ShowWindow(int cmdShow);
	virtual BOOL	 OnIdle(long count);

  public:
	virtual BOOL	 LoadAccelTable(LPCTSTR name);
	virtual BOOL	 LoadAccelTable(int resId);
	virtual BOOL	 PreTranslateMessage(MSG *msg);
	virtual LRESULT	 WindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
  protected:
	void			 SetLastParam(UINT uMsg, WPARAM wParam, LPARAM lParam);

  public:
	virtual BOOL	 OnCreate(CREATESTRUCT *cs);
	virtual void	 OnDestroy();
	virtual void	 OnPaint();
	virtual void	 OnCommand(UINT notifyCode, UINT id, HWND ctrl);
	virtual void	 OnInitMenuPopup(HMENU hMenu, UINT pos, BOOL sysMenu);
	LRESULT			 Default();

  public:
	virtual void	 OnInitSubMenu(HMENU hMenu, UINT id);
	int				 MessageBox(const TCHAR* text, const TCHAR* title = 0, unsigned style = MB_OK | MB_ICONERROR);
	LRESULT			 SendMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0);
	BOOL			 PostMessage(UINT uMsg, WPARAM wParam = 0, LPARAM lParam = 0);
	BOOL			 ScreenToClient(POINT *point);
	BOOL			 SetMenu(HMENU hMenu);
	HMENU			 GetMenu();
	BOOL			 DrawMenuBar();
	BOOL			 SetWindowPos(HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags);
	BOOL			 GetClientRect(RECT *rc);
	BOOL			 GetWindowRect(RECT *rc);
	UINT			 SetTimer(UINT idTimer, UINT uTimeout);
	BOOL			 KillTimer(UINT idTimer);
	BOOL			 InvalidateRect(CONST RECT* lpRect, BOOL bErase = TRUE);
	BOOL			 UpdateWindow();
	HWND			 SetFocus();

  protected:
	static LRESULT CALLBACK _WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

  protected:
	CWinApp			*App;
	HWND			 hWnd;

	UINT			 LastMsg;
	WPARAM			 LastWParam;
	LPARAM			 LastLParam;

	HACCEL			 hAccelTable;

  private:
	CWindow			*Next;

  private:
	static CWindow	*WindowListTop;
	static CWindow	*WindowInitPtr;

	friend class	 CWinApp;
};

inline void CWindow::SetLastParam(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LastMsg = uMsg;
	LastWParam = wParam;
	LastLParam = lParam;
}

inline BOOL CWindow::Create(CWinApp *app, LPCTSTR name, CWindow *parent)
{
	return Create(app, name, 0, parent);
}

inline BOOL CWindow::LoadAccelTable(int resId)
{
	return LoadAccelTable(MAKEINTRESOURCE(resId));
}

inline LRESULT CWindow::Default()
{
	return ::DefWindowProc(hWnd, LastMsg, LastWParam, LastLParam);
}

inline LRESULT CWindow::SendMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return ::SendMessage(hWnd, uMsg, wParam, lParam);
}

inline BOOL CWindow::PostMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return ::PostMessage(hWnd, uMsg, wParam, lParam);
}

inline int CWindow::MessageBox(const TCHAR* text, const TCHAR* title, unsigned style)
{
	return ::MessageBox(hWnd, text, title, style);
}

inline BOOL CWindow::ShowWindow(int cmdShow)
{
	return ::ShowWindow(hWnd, cmdShow);
}

inline BOOL CWindow::ScreenToClient(POINT *point)
{
	return ::ScreenToClient(hWnd, point);
}

inline BOOL CWindow::SetMenu(HMENU hMenu)
{
	return ::SetMenu(hWnd, hMenu);
}

inline HMENU CWindow::GetMenu()
{
	return ::GetMenu(hWnd);
}

inline BOOL CWindow::DrawMenuBar()
{
	return ::DrawMenuBar(hWnd);
}

inline BOOL CWindow::SetWindowPos(HWND hWndIA, int X, int Y, int cx, int cy, UINT uFlags)
{
	return ::SetWindowPos(hWnd, hWndIA, X, Y, cx, cy, uFlags);
}

inline BOOL CWindow::GetClientRect(RECT *rc)
{
	return ::GetClientRect(hWnd, rc);
}

inline BOOL CWindow::GetWindowRect(RECT *rc)
{
	return ::GetWindowRect(hWnd, rc);
}

inline UINT CWindow::SetTimer(UINT idTimer, UINT uTimeout)
{
	return ::SetTimer(hWnd, idTimer, uTimeout, 0);
}

inline BOOL CWindow::KillTimer(UINT idTimer)
{
	return ::KillTimer(hWnd, idTimer);
}

inline BOOL CWindow::InvalidateRect(CONST RECT *lpRect, BOOL bErase)
{
	return ::InvalidateRect(hWnd, lpRect, bErase);
}

inline BOOL CWindow::UpdateWindow()
{
	return ::UpdateWindow(hWnd);
}

inline HWND CWindow::SetFocus()
{
	return ::SetFocus(hWnd);
}

#endif
