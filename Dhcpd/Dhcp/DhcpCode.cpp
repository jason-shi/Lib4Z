#include "StdAfx.h"
#include "OptionCode.h"

int DhcpdEncode(byte *pPacket, int nLength, TDhcpMsg *pDhcpMsg)
{
	byte *pTemp = pPacket;

	if (nLength < 236)
		return -1;
	nLength -= 236;

	*pPacket++ = pDhcpMsg->byOpCode;
	*pPacket++ = pDhcpMsg->byHWType;
	*pPacket++ = pDhcpMsg->byHWLen;
	*pPacket++ = pDhcpMsg->byHops;
	SET_DWORD(pPacket, pDhcpMsg->dwTransID);
	pPacket += 4;
	SET_WORD(pPacket, pDhcpMsg->wSeconds);
	pPacket += 2;
	SET_WORD(pPacket, pDhcpMsg->wFlags);
	pPacket += 2;
	SET_DWORD(pPacket, pDhcpMsg->dwClientIP);
	pPacket += 4;
	SET_DWORD(pPacket, pDhcpMsg->dwUserIP);
	pPacket += 4;
	SET_DWORD(pPacket, pDhcpMsg->dwServerIP);
	pPacket += 4;
	SET_DWORD(pPacket, pDhcpMsg->dwAgentIP);
	pPacket += 4;
	memcpy(pPacket, pDhcpMsg->byHWAddr, 16);
	pPacket += 16;
	memcpy(pPacket, pDhcpMsg->szHostName, 64);
	pPacket += 64;
	memcpy(pPacket, pDhcpMsg->szBootFile, 128);
	pPacket += 128;

	int nOffset = ::OptionEncode(pPacket, nLength, &pDhcpMsg->tOption);
	nLength -= nOffset;
	CHECK_ADVANCE(pPacket, nOffset);

	return pPacket - pTemp;
}

int DhcpdDecode(TDhcpMsg *pDhcpMsg, byte *pPacket, int nLength)
{
	byte *pTemp = pPacket;

	if (nLength < 236)
		return -1;
	nLength -= 236;

	pDhcpMsg->byOpCode = *pPacket++;
	pDhcpMsg->byHWType = *pPacket++;
	pDhcpMsg->byHWLen  = *pPacket++;
	pDhcpMsg->byHops   = *pPacket++;
	pDhcpMsg->dwTransID = GET_DWORD(pPacket);
	pPacket += 4;
	pDhcpMsg->wSeconds = GET_WORD(pPacket);
	pPacket += 2;
	pDhcpMsg->wFlags = GET_WORD(pPacket);
	pPacket += 2;
	pDhcpMsg->dwClientIP = GET_DWORD(pPacket);
	pPacket += 4;
	pDhcpMsg->dwUserIP = GET_DWORD(pPacket);
	pPacket += 4;
	pDhcpMsg->dwServerIP = GET_DWORD(pPacket);
	pPacket += 4;
	pDhcpMsg->dwAgentIP = GET_DWORD(pPacket);
	pPacket += 4;
	memcpy(pDhcpMsg->byHWAddr, pPacket, 16);
	pPacket += 16;
	memcpy(pDhcpMsg->szHostName, pPacket, 64);
	pPacket += 64;
	memcpy(pDhcpMsg->szBootFile, pPacket, 128);
	pPacket += 128;

	int nOffset = ::OptionDecode(&pDhcpMsg->tOption, pPacket, nLength);
	nLength += nOffset;
	CHECK_ADVANCE(pPacket, nOffset);

	return pPacket - pTemp;
}
