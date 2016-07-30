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
	//�����������Ͳ˵���Ϣ
	if (msg->message >= WM_NCCREATE
#if(_WIN32_WINNT >= 0x0500)
		&& msg->message <= WM_NCXBUTTONDBLCLK)
#else
		&& msg->message <= WM_NCMBUTTONDBLCLK)
#endif
	{
		return TRUE;
	}
	//������Χ֮�������¼�
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

	//���ڼ����ʧ���������
	m_pActived = g_pZ2coo->GetActived();
	if (m_pActived)
		m_pActived->OnLoseFocus();

	//���ǰ�Ի���
	g_pZ2coo->SetActived(this);

	//������ѭ�������������������¼�
	Run();

	g_pZ2coo->DelObject(this);

	//����֮ǰʧ���
	g_pZ2coo->SetActived(m_pActived);

	//���ضԻ�����
	return DialogResult();
}
