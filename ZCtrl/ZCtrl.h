#ifndef __ZCtrl_h__
#define __ZCtrl_h__

//滚动条
#include <ScrollBar\SlideBar\SlideBtn.h>
#include <ScrollBar\SlideBar\HSlideBar.h>
#include <ScrollBar\SlideBar\VSlideBar.h>
#include <ScrollBar\HScrollBar.h>
#include <ScrollBar\VScrollBar.h>

//浏览面板
#include <BrowsePanel\BrowseBar\BrowseBar.h>
#include <BrowsePanel\BrowseData\BrowseData.h>
#include <BrowsePanel\BrowseView\LinBrowseView.h>
#include <BrowsePanel\BrowseView\PixBrowseView.h>
#include <BrowsePanel\BrowseTitle\BrowseTitle.h>
#include <BrowsePanel\BrowseRuler\BrowseRuler.h>
#include <BrowsePanel\BrowseSequence\BrowseSequence.h>
#include <BrowsePanel\BrowsePanel.h>

//照片面板
#include <PhotoPanel\PhotoVScrl\PhotoVScrl.h>
#include <PhotoPanel\PhotoView\PhotoView.h>
#include <PhotoPanel\PhotoPanel.h>

//页控件
#include <PageManage\PageBase.h>
#include <PageManage\PageManage.h>

//工具条
#include <ToolBar\ToolBtn.h>
#include <ToolBar\ToolBar.h>

//数字面板
#include <DigitPanel\DigitPanel.h>

#ifdef	_MSC_VER
	#ifdef	_DEBUG
		#pragma comment(lib, "ZCtrlD.lib")
	#else
		#pragma comment(lib, "ZCtrl.lib")
	#endif
#endif

extern CImage		 ImageCtrl;

class CZCtrl {
  public:
	SINGLETON(CZCtrl);
	~CZCtrl();

  private:
	CZCtrl();

  public:
	bool			 Initial();
	void			 Release();
	void			 SetWindow(CWindow *_window);
	CWindow			*GetWindow() const;

  private:
	CWindow			*window;
	CRect			 ClientRect;
};

inline void CZCtrl::SetWindow(CWindow *_window)
{
	window = _window;
	window->GetClientRect(&ClientRect);
}

inline CWindow *CZCtrl::GetWindow() const
{
	return window;
}

#define g_pZCtrl			CZCtrl::GetInstancePtr()

#endif
