#include "StdAfx.h"
#include "DLinkCode.h"
#include "DLinkInst.h"
#include "DLinkProc.h"

CDLinkInst::CDLinkInst(CDLinkProc *pDLinkProc)
{
	m_pJSelf = pDLinkProc;
}

CDLinkInst::~CDLinkInst()
{
	m_pJSelf = NULL;
}

void CDLinkInst::SetDLinkInst(TDLinkMsg *pDLinkMsg)
{
	memcpy(DA, pDLinkMsg->DA, 6);
	memcpy(SA, pDLinkMsg->SA, 6);
	wProtocal   = pDLinkMsg->wProtocal;
	byVersion   = pDLinkMsg->byVersion;
	byWhat      = pDLinkMsg->byWhat;
	wServType   = pDLinkMsg->wServType;
	wLength     = pDLinkMsg->wLength;
	byFlag      = pDLinkMsg->byFlag;
	byOffset    = pDLinkMsg->byOffset;
	byServival  = pDLinkMsg->byServival;
	byUDPTCP    = pDLinkMsg->byUDPTCP;
	SIP         = pDLinkMsg->SIP;
	DIP         = pDLinkMsg->DIP;
	SPort       = pDLinkMsg->SPort;
	DPort       = pDLinkMsg->DPort;
}

void CDLinkInst::GetDLinkInst(TDLinkMsg *pDLinkMsg)
{
	memcpy(pDLinkMsg->DA, DA, 6);
	memcpy(pDLinkMsg->SA, SA, 6);
	pDLinkMsg->wProtocal  = wProtocal;
	pDLinkMsg->byVersion  = byVersion;
	pDLinkMsg->byWhat     = byWhat;
	pDLinkMsg->wServType  = wServType;
	pDLinkMsg->wLength    = wLength;
	pDLinkMsg->byFlag     = byFlag;
	pDLinkMsg->byOffset   = byOffset;
	pDLinkMsg->byServival = byServival;
	pDLinkMsg->byUDPTCP   = byUDPTCP;
	pDLinkMsg->SIP        = SIP;
	pDLinkMsg->DIP        = DIP;
	pDLinkMsg->SPort      = SPort;
	pDLinkMsg->DPort      = DPort;
}

void CDLinkInst::Transposition()
{
	byte abyTemp[6];
	memcpy(abyTemp, DA, 6);
	memcpy(DA, SA, 6);
	memcpy(SA, abyTemp, 6);

	dword dwTemp = SIP;
	SIP = DIP;
	DIP = dwTemp;

	word wTemp = SPort;
	SPort = DPort;
	DPort = wTemp;

	byServival = DLINK_DEFAULT_HOPS;
}

void CDLinkInst::OnRecvNetPacket(TDLinkMsg *pDLinkMsg)
{
	SetDLinkInst(pDLinkMsg);
	Transposition(); //收发转换

	m_pJSelf->PostMessage(m_pJSelf->m_dwJDhcp, EV_DLINK_DATA, pDLinkMsg->byData, pDLinkMsg->wDataLen);
}

void CDLinkInst::OnRecvDLinkData(TDLinkData *pDLinkData)
{
	TDLinkMsg tDLinkMsg;
	byte      byDLinkBuff[MAX_DLINK_BUFF_LEN];

	//填写消息头
	GetDLinkInst(&tDLinkMsg);

	//填写数据
	memcpy(tDLinkMsg.byData, pDLinkData->byData, pDLinkData->wDataLen);
	tDLinkMsg.wDataLen = pDLinkData->wDataLen;

	//消息编码
	int nBuffLength = DLinkEncode(byDLinkBuff, MAX_DLINK_BUFF_LEN, &tDLinkMsg);
	if (nBuffLength == -1)
		return;

	//单播或广播
	if (!pDLinkData->bBroadcast)
		m_pJSelf->PostMessage(m_pJSelf->m_dwJDest, EV_NET_PACKET, byDLinkBuff, nBuffLength);
	else
	{
		memcpy(byDLinkBuff, "FFFFFF", 6); //广播地址
		for (VECDWORD::iterator iter = m_pJSelf->m_vecJDest.begin(); iter != m_pJSelf->m_vecJDest.end(); iter++)
		{
			if (!m_pJSelf->PostMessage(*iter, EV_NET_PACKET, byDLinkBuff, nBuffLength))
				return;
		}
	}
}
