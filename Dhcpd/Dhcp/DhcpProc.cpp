#include "StdAfx.h"
#include "DhcpCode.h"
#include "DhcpInst.h"
#include "DhcpProc.h"
#include "DhcpManage.h"

CDhcpProc::CDhcpProc(dword dwProcID, byte byPriority)
				: JProcess(dwProcID, byPriority)
{
}

CDhcpProc::~CDhcpProc()
{
}

void CDhcpProc::ProcEntry(byte byState, dword dwEvent, byte *pBuff, int nLength)
{
	switch (byState)
	{
	case DHCP_INIT_STATE:
		switch (dwEvent)
		{
		case EV_CREATE:
			OnCreate();
			break;
		}
		break;
	case DHCP_WORK_STATE:
		switch (dwEvent)
		{
		case EV_DESTROY:
			OnDestroy();
			break;
		case EV_SET_JDLINK:
			OnSetJDLink(pBuff, nLength);
			break;
		case EV_DLINK_DATA:
			OnRecvDLinkData(pBuff, nLength);
			break;
		}
		break;
	}
}

void CDhcpProc::OnCreate()
{
	m_pDhcpManage = new CDhcpManage(this);
	if (!m_pDhcpManage)
		return;

	NextState(DHCP_WORK_STATE);
}

void CDhcpProc::OnDestroy()
{
	SAFE_DELETE(m_pDhcpManage);
	return JProcess::OnDestroy();
}

void CDhcpProc::OnSetJDLink(byte *pPacket, int nLength)
{
	if (nLength != 4)
		return;
	m_dwJDLink = (int)pPacket;
}

void CDhcpProc::OnRecvDLinkData(byte *pPacket, int nLength)
{
	TDhcpMsg tDhcpMsg;

	//消息解码
	if (DhcpdDecode(&tDhcpMsg, pPacket, nLength) == -1)
		return;

	//获得实例
	CDhcpInst *pDhcpInst;
	if ((pDhcpInst = m_pDhcpManage->ObtainInst(tDhcpMsg.dwTransID)) == NULL)
		return;

	//消息校验
	if (tDhcpMsg.byOpCode != DHCP_OPCODE_UP)
		return;

	//消息处理
	switch (tDhcpMsg.tOption.byMsgType)
	{
	case ID_DHCP_DISCOVER:
		pDhcpInst->OnRecvDiscover(&tDhcpMsg);
		break;
	case ID_DHCP_REQUEST:
		pDhcpInst->OnRecvRequest(&tDhcpMsg);
		break;
	case ID_DHCP_DECLINE:
		pDhcpInst->OnRecvDecline(&tDhcpMsg);
		break;
	case ID_DHCP_RELEASE:
		pDhcpInst->OnRecvRelease(&tDhcpMsg);
		break;
	}
}
