#include "StdAfx.h"

CDialog::CDialog() : CPanel()
{
	m_pActived = NULL;
}

CDialog::CDialog(CImage *pImage, CPoint xDrawPos, CSize xSize)
			: CPanel(pImage, xDrawPos, CPoint(0, 0), xSize, 0)
{
	m_pActived = NULL;
}

CDialog::CDialog(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize)
			: CPanel(pImage, xDrawPos, xSrcPos, xSize, 0)
{
}

CDialog::~CDialog()
{
	m_pActived = NULL;
}

BOOL CDialog::PreTranslateMessage(MSG *msg)
{
	//丢弃标题栏和菜单消息
	if (msg->message >= WM_NCCREATE
#if(_WIN32_WINNT >= 0x0500)
		&& msg->message <= WM_NCXBUTTONDBLCLK)
#else
		&& msg->message <= WM_NCMBUTTONDBLCLK)
#endif
	{
		return TRUE;
	}
	//丢弃范围之外的鼠标事件
	if (msg->message >= WM_MOUSEFIRST
		&& msg->message <= WM_MOUSELAST)
	{
		int xPos = LOWORD(msg->lParam);
		int yPos = HIWORD(msg->lParam);
		if (!PtIn(CPoint(xPos, yPos)))
			return TRUE;
	}
	return FALSE;
}

int CDialog::Run()
{
	bool idle = false;
	long count = 0;
	MSG msgCur;

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
	return 0;
}

void CDialog::OnClose()
{
	PostQuitMessage(0);
}

INT_PTR CDialog::DoModal()
{
	g_pZ2coo->AddObject(this);

	//处于激活的失活并保存起来
	m_pActived = g_pZ2coo->GetActived();
	if (m_pActived)
		m_pActived->OnLoseFocus();

	//激活当前对话框
	g_pZ2coo->SetActived(this);

	//阻塞父循环，丢弃窗口外的鼠标事件
	Run();

	g_pZ2coo->DelObject(this);

	//激活之前失活的
	g_pZ2coo->SetActived(m_pActived);

	//返回对话框结果
	return DialogResult();
}
