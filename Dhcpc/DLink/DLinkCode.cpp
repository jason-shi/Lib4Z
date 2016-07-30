#include "StdAfx.h"
#include "DLinkCode.h"

static word CalcChkSum(byte *pBuff, int nLength)
{
	byte byFormer = 0;
	for (int i = 0; i < nLength; i += 2)
		byFormer += pBuff[i];

	byte byLatter = 0;
	for (int i = 1; i < nLength; i += 2)
		byLatter += pBuff[i];

	return (byFormer << 8) + byLatter;
}

int DLinkEncode(byte *pPacket, int nLength, TDLinkMsg *pDLinkMsg)
{
	byte *pTemp = pPacket;

	if (nLength < 42)
		return -1;
	nLength -= 42;

	memcpy(pPacket, pDLinkMsg->DA, 6);
	pPacket += 6;
	memcpy(pPacket, pDLinkMsg->SA, 6);
	pPacket += 6;
	SET_WORD(pPacket, pDLinkMsg->wProtocal);
	pPacket += 2;
	*pPacket++ = pDLinkMsg->byVersion;
	*pPacket++ = pDLinkMsg->byWhat;
	SET_WORD(pPacket, pDLinkMsg->wServType);
	pPacket += 2;
	SET_WORD(pPacket, pDLinkMsg->wLength);
	pPacket += 2;
	*pPacket++ = pDLinkMsg->byFlag;
	*pPacket++ = pDLinkMsg->byOffset;
	*pPacket++ = pDLinkMsg->byServival;
	*pPacket++ = pDLinkMsg->byUDPTCP;
	SET_WORD(pPacket, 0); //预留头部校验和
	pPacket += 2;
	SET_DWORD(pPacket, pDLinkMsg->SIP);
	pPacket += 4;
	SET_DWORD(pPacket, pDLinkMsg->DIP);
	pPacket += 4;
	SET_WORD(pPacket, pDLinkMsg->SPort);
	pPacket += 2;
	SET_WORD(pPacket, pDLinkMsg->DPort);
	pPacket += 2;
	SET_WORD(pPacket, pDLinkMsg->wDataLen);
	pPacket += 2;
	//数据校验和
	word wDataChkSum = ::CalcChkSum(pDLinkMsg->byData, pDLinkMsg->wDataLen);
	SET_WORD(pPacket, wDataChkSum);
	pPacket += 2;
	//头部校验和
	word wHeadChkSum = ::CalcChkSum(pTemp, pPacket - pTemp);
	SET_WORD(pTemp + 24, wHeadChkSum);

	if (nLength < pDLinkMsg->wDataLen)
		return -1;
	nLength -= pDLinkMsg->wDataLen;

	memcpy(pPacket, pDLinkMsg->byData, pDLinkMsg->wDataLen);
	pPacket += pDLinkMsg->wDataLen;

	return pPacket - pTemp;
}

int DLinkDecode(TDLinkMsg *pDLinkMsg, byte *pPacket, int nLength)
{
	byte *pTemp = pPacket;

	if (nLength < 42)
		return -1;
	nLength -= 42;

	memcpy(pDLinkMsg->DA, pPacket, 6);
	pPacket += 6;
	memcpy(pDLinkMsg->SA, pPacket, 6);
	pPacket += 6;
	pDLinkMsg->wProtocal = GET_WORD(pPacket);
	pPacket += 2;
	pDLinkMsg->byVersion = *pPacket++;
	pDLinkMsg->byWhat = *pPacket++;
	pDLinkMsg->wServType = GET_WORD(pPacket);
	pPacket += 2;
	pDLinkMsg->wLength = GET_WORD(pPacket);
	pPacket += 2;
	pDLinkMsg->byFlag = *pPacket++;
	pDLinkMsg->byOffset = *pPacket++;
	pDLinkMsg->byServival = *pPacket++;
	pDLinkMsg->byUDPTCP = *pPacket++;
	pDLinkMsg->wHeadChkSum = GET_WORD(pPacket);
	pPacket += 2;
	pDLinkMsg->SIP = GET_DWORD(pPacket);
	pPacket += 4;
	pDLinkMsg->DIP = GET_DWORD(pPacket);
	pPacket += 4;
	pDLinkMsg->SPort = GET_WORD(pPacket);
	pPacket += 2;
	pDLinkMsg->DPort = GET_WORD(pPacket);
	pPacket += 2;
	pDLinkMsg->wDataLen = GET_WORD(pPacket);
	pPacket += 2;
	pDLinkMsg->wDataChkSum = GET_WORD(pPacket);
	pPacket += 2;

	//头部校验和
	word wHeadChkSum = ::CalcChkSum(pTemp, pPacket - pTemp);
	if (wHeadChkSum != pDLinkMsg->wHeadChkSum)
		return -1;

	if (pDLinkMsg->wDataLen != nLength)
		return -1;
	memcpy(pDLinkMsg->byData, pPacket, nLength);
	pPacket += nLength;

	word wDataChkSum = ::CalcChkSum(pDLinkMsg->byData, nLength);
	if (wDataChkSum != pDLinkMsg->wDataChkSum)
		return -1;

	return pPacket - pTemp;
}
