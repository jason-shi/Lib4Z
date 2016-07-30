#ifndef __Application_h__
#define __Application_h__

#include "Window.h"
#include "TCHAR.h"

class CWinApp {
  public:
	CWinApp();
	~CWinApp();

  public:
	void			 SetWinMainArgs(HINSTANCE hInstance, LPSTR lpszCmdLine, int nCmdShow);

	BOOL			 RegisterWndClass(LPCTSTR name, UINT classStyle, HCURSOR hCursor = 0,
									  HBRUSH hbrBackground = (HBRUSH)(COLOR_WINDOW + 1), HICON hIcon = 0);

  public:
	virtual BOOL	 InitInstance();
	virtual BOOL	 OnIdle(long count);
	virtual BOOL	 PreTranslateMessage(MSG *msg);
	virtual int		 Run();

  public:
	HRSRC			 FindResource(LPCTSTR name, LPCTSTR type);
	HGLOBAL			 LoadResource(HRSRC hres);
	HACCEL			 LoadAccelerators(LPCTSTR name);
	HMENU			 LoadMenu(LPCTSTR name);
	HMENU			 LoadMenu(int resId);
	HICON			 LoadIcon(LPCTSTR name);
	HICON			 LoadIcon(int resId);

  public:
	HINSTANCE		 GetInstance() const { return Instance; }
	operator		 HINSTANCE() const { return Instance; }

  public:
	CWindow			*GetMainWnd() { return MainWnd; }

  protected:
	HINSTANCE		 Instance;
	LPSTR			 CmdLine;
	int				 CmdShow;

  protected:
	MSG				 msgCur;
	CWindow			*MainWnd;
};

inline void CWinApp::SetWinMainArgs(HINSTANCE hInstance, LPSTR lpszCmdLine, int nCmdShow)
{
	Instance = hInstance;
	CmdLine = lpszCmdLine;
	CmdShow = nCmdShow;
}

inline HRSRC CWinApp::FindResource(LPCTSTR name, LPCTSTR type)
{
	return ::FindResource(Instance, name, type);
}

inline HGLOBAL CWinApp::LoadResource(HRSRC hres)
{
	return ::LoadResource(Instance, hres);
}

inline HACCEL CWinApp::LoadAccelerators(LPCTSTR name)
{
	return ::LoadAccelerators(Instance, name);
}

inline HMENU CWinApp::LoadMenu(LPCTSTR name)
{
	return ::LoadMenu(Instance, name);
}

inline HMENU CWinApp::LoadMenu(int resId)
{
	return ::LoadMenu(Instance, MAKEINTRESOURCE(resId));
}

inline HICON CWinApp::LoadIcon(LPCTSTR name)
{
	return ::LoadIcon(Instance, name);
}

inline HICON CWinApp::LoadIcon(int resId)
{
	return ::LoadIcon(Instance, MAKEINTRESOURCE(resId));
}

extern CWinApp			*Application;

#endif
