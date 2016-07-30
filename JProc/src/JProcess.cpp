#include "StdAfx.h"
#include "JMsgManage.h"
#include "JReqManage.h"
#include "JProcess.h"
#include "JProcManage.h"
#include "JQuitManage.h"

JProcess::JProcess(dword dwProcID, byte byPriority)
			: m_dwProcID(dwProcID), m_byPriority(byPriority)
{
	m_byState = 0;

	m_byStatus = JP_NEWPROC;
	m_pJSender = NULL;

	m_pNext = NULL;
	m_pJMsgManage = NULL;
	m_pJReqManage = NULL;
}

JProcess::~JProcess()
{
	SAFE_DELETE(m_pJMsgManage);
	SAFE_DELETE(m_pJReqManage);
}

bool JProcess::InitMsgManage(int nMsgLength, int nMsgSize)
{
	m_pJMsgManage = new JMsgManage();
	if (!m_pJMsgManage || !m_pJMsgManage->InitMsgManage(nMsgLength, nMsgSize))
		return false;

	return true;
}

bool JProcess::InitReqManage(int nReqLength, int nReqSize)
{
	m_pJReqManage = new JReqManage();
	if (!m_pJReqManage || !m_pJReqManage->InitReqManage(nReqLength, nReqSize))
		return false;

	return true;
}

void JProcess::OnDestroy()
{
	JMessage msg;
	while (PeekMessage(&msg, 0, 0, true));

	//添加退出标记
	g_pJQuitManage->AddJQuit(m_dwProcID);

	//发送异步消息
	PostMessage(m_dwProcID, EV_QUIT);
}

int JProcess::SendMessage(dword dwProcID, dword dwEvent, byte *pBuff, int nLength)
{
	JProcess *pJRecver = g_pJProcManage->FindProcess(dwProcID);
	if (pJRecver == NULL)
		return false;

	if (pJRecver == this)
	{
		ProcEntry(m_byState, dwEvent, pBuff, nLength);
		return true;
	}
	else
	{
		return pJRecver->m_pJMsgManage->SendMessage(dwEvent, pBuff, nLength);
	}

	return false;
}

bool JProcess::PostMessage(dword dwProcID, dword dwEvent, byte *pBuff, int nLength)
{
	JProcess *pJRecver = g_pJProcManage->FindProcess(dwProcID);
	if (pJRecver == NULL)
		return false;

	return pJRecver->m_pJMsgManage->PostMessage(dwEvent, pBuff, nLength);
}

bool JProcess::GetMessage(JMessage *pJMessage, dword dwFilterMin, dword dwFilterMax)
{
	return m_pJMsgManage->GetMessage(pJMessage, dwFilterMin, dwFilterMax);
}

bool JProcess::PeekMessage(JMessage *pJMessage, dword dwFilterMin, dword dwFilterMax, bool bRemove)
{
	return m_pJMsgManage->PeekMessage(pJMessage, dwFilterMin, dwFilterMax, bRemove);
}

int JProcess::SendRequest(dword dwProcID, dword dwEvent, byte *pBuff, int nLength)
{
	JProcess *pJRecver = g_pJProcManage->FindProcess(dwProcID);
	if (pJRecver == NULL)
		return false;

	if (m_pJSender == this)
	{
		ProcEntry(m_byState, dwEvent, pBuff, nLength);
		return true;
	}
	else
	{
		return pJRecver->m_pJReqManage->SendRequest(dwEvent, pBuff, nLength);
	}

	return false;
}

bool JProcess::PostRequest(dword dwProcID, dword dwEvent, byte *pBuff, int nLength)
{
	JProcess *pJRecver = g_pJProcManage->FindProcess(dwProcID);
	if (pJRecver == NULL)
		return false;

	return pJRecver->m_pJReqManage->PostRequest(dwEvent, pBuff, nLength);
}

bool JProcess::GetRequest(JRequest *pJRequest, dword dwFilterMin, dword dwFilterMax)
{
	return m_pJReqManage->GetRequest(pJRequest, dwFilterMin, dwFilterMax);
}

bool JProcess::PeekRequest(JRequest *pJRequest, dword dwFilterMin, dword dwFilterMax, bool bRemove)
{
	return m_pJReqManage->PeekRequest(pJRequest, dwFilterMin, dwFilterMax, bRemove);
}

static UINT WINAPI ThreadJProcess(LPVOID lpParameter)
{
	JProcess *pJProcess = (JProcess *)lpParameter;

	JMessage msg;
	msg.pBuffer = new byte[pJProcess->m_pJMsgManage->GetMsgLength()];

	JRequest req;
	req.pBuffer = new byte[pJProcess->m_pJReqManage->GetReqLength()];

	while (true)
	{
		//优先处理消息队列
		if (pJProcess->PeekMessage(&msg, 0, 0, false))
		{
			if (!pJProcess->GetMessage(&msg, 0, 0))
				break;
			pJProcess->ProcEntry(pJProcess->GetState(), msg.dwEvent, msg.pBuffer, msg.nLength);
			continue;
		}
		//其次处理请求队列，从而控制请求的数量
		if (pJProcess->PeekRequest(&req, 0, 0, false))
		{
			if (!pJProcess->GetRequest(&req, 0, 0))
				break;
			pJProcess->ProcEntry(pJProcess->GetState(), req.dwEvent, req.pBuffer, req.nLength);
			continue;
		}

		::Sleep(10);
	}

	//收到EV_QUIT消息：退出消息循环，删除进程，杀死进程
	delete msg.pBuffer;
	delete req.pBuffer;

	dword dwProcID = pJProcess->GetProcID();

	g_pJProcManage->DelProcess(pJProcess);
	SAFE_DELETE(pJProcess);

	//删除退出标记
	g_pJQuitManage->DelJQuit(dwProcID);
	return 0;
}

static bool InitThreadJProcess(JProcess *pJProcess)
{
	HANDLE m_hJProcess = NULL;
	if (!m_hJProcess)
	{
		m_hJProcess = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadJProcess, pJProcess, 0, NULL);
		if (m_hJProcess)
		{
			CloseHandle(m_hJProcess);
			m_hJProcess = NULL;
			return true;
		}
	}
	return false;
}

JProcess *CreateJProcess(JProcessCreator *pCreator,
						 dword dwProcID, byte byPriority, int nMsgLength, int nMsgSize, int nReqLength, int nReqSize)
{
	JProcess *pJProcess = pCreator->CreateJProcess(dwProcID, byPriority);
	if (!pJProcess)
		return NULL;

	//添加进程队列
	g_pJProcManage->AddProcess(pJProcess);

	//创建消息队列
	if (!pJProcess->InitMsgManage(nMsgLength, nMsgSize))
		return NULL;
	if (!pJProcess->InitReqManage(nReqLength, nReqSize))
		return NULL;

	//初始化线程
	if (!InitThreadJProcess(pJProcess))
		return NULL;

	//延时重送事件
	PostMessageWait(pJProcess->GetProcID(), EV_CREATE);
	return pJProcess;
}

void WaitForJProcess(dword dwProcID)
{
	while (true)
	{
		if (g_pJQuitManage->CheckJQuit(dwProcID))
			break;
		::Sleep(110);
	}
}

static bool _PostMessage(dword dwProcID, dword dwEvent, byte *pBuff, int nLength)
{
	JProcess *pJRecver = g_pJProcManage->FindProcess(dwProcID);
	if (pJRecver == NULL)
		return false;

	return pJRecver->m_pJMsgManage->PostMessage(dwEvent, pBuff, nLength);
}

void PostMessageWait(dword dwProcID, dword dwEvent, byte *pBuff, int nLength)
{
	while (!_PostMessage(dwProcID, dwEvent, pBuff, nLength))
		::Sleep(110);
}
