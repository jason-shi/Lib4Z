#include "StdAfx.h"

using namespace std;

static int EncodeByte(byte *pPacket, int nLength, byte byType, byte byValue)
{
	byte *pTemp = pPacket;

	if (nLength < 3)
		return -1;
	*pPacket++ = byType;
	*pPacket++ = 1;
	*pPacket++ = byValue;

	return pPacket - pTemp;
}

static int EncodeWord(byte *pPacket, int nLength, byte byType, word wValue)
{
	byte *pTemp = pPacket;

	if (nLength < 4)
		return -1;
	*pPacket++ = byType;
	*pPacket++ = 2;
	SET_WORD(pPacket, wValue);
	pPacket += 2;

	return pPacket - pTemp;
}

static int EncodeDword(byte *pPacket, int nLength, byte byType, dword dwValue)
{
	byte *pTemp = pPacket;

	if (nLength < 6)
		return -1;
	*pPacket++ = byType;
	*pPacket++ = 4;
	SET_DWORD(pPacket, dwValue);
	pPacket += 4;

	return pPacket - pTemp;
}

static int EncodeVecDword(byte *pPacket, int nLength, byte byType, VECDWORD& vecValue)
{
	byte *pTemp = pPacket;

	int nValueLen = vecValue.size() * 4;
	if (nLength < 2 + nValueLen)
		return -1;
	*pPacket++ = byType;
	*pPacket++ = nValueLen;
	for (int i = 0; i < nValueLen; i += 4)
	{
		SET_DWORD(pPacket, vecValue[i]);
		pPacket += 4;
	}

	return pPacket - pTemp;
}

static int EncodeStaticRoute(byte *pPacket, int nLength, byte byType, VECSTATICROUTE& vecStaticRoute)
{
	byte *pTemp = pPacket;

	int nRouteLen = vecStaticRoute.size() * 8;
	if (nLength < 2 + nRouteLen)
		return -1;
	*pPacket++ = byType;
	*pPacket++ = nRouteLen;
	for (int i = 0; i < nRouteLen; i += 8)
	{
		SET_DWORD(pPacket, vecStaticRoute[i].dwIP);
		pPacket += 4;
		SET_DWORD(pPacket, vecStaticRoute[i].dwNH);
		pPacket += 4;
	}

	return pPacket - pTemp;
}

static int DecodeByte(byte &byValue, byte *pPacket, int nLength, byte byType)
{
	byte *pTemp = pPacket;

	if (nLength < 3)
		return -1;
	if (*pPacket++ != byType)
		return -1;
	if (*pPacket++ != 1)
		return -1;
	byValue = *pPacket++;

	return pPacket - pTemp;
}

static int DecodeWord(word &wValue, byte *pPacket, int nLength, byte byType)
{
	byte *pTemp = pPacket;

	if (nLength < 4)
		return -1;
	if (*pPacket++ != byType)
		return -1;
	if (*pPacket++ != 2)
		return -1;
	wValue = GET_WORD(pPacket);
	pPacket += 2;

	return pPacket - pTemp;
}

static int DecodeDword(dword &dwValue, byte *pPacket, int nLength, byte byType)
{
	byte *pTemp = pPacket;

	if (nLength < 6)
		return -1;
	if (*pPacket++ != byType)
		return -1;
	if (*pPacket++ != 4)
		return -1;
	dwValue = GET_DWORD(pPacket);
	pPacket += 4;

	return pPacket - pTemp;
}

static int DecodeVecDword(VECDWORD& vecValue, byte *pPacket, int nLength, byte byType)
{
	byte *pTemp = pPacket;

	if (nLength < 2)
		return -1;
	if (*pPacket++ != byType)
		return -1;
	int nValueLen = *pPacket++;

	if (nValueLen > nLength || (nValueLen % 4) != 0)
		return -1;
	for (int i = 0; i < nValueLen; i += 4)
	{
		vecValue.push_back(GET_DWORD(pPacket));
		pPacket += 4;
	}

	return pPacket - pTemp;
}

static int DecodeStaticRoute(VECSTATICROUTE& vecStaticRoute, byte *pPacket, int nLength, byte byType)
{
	byte *pTemp = pPacket;

	if (nLength < 2)
		return -1;
	if (*pPacket++ != byType)
		return -1;
	int nRouteLen = *pPacket++;

	if (nRouteLen > nLength || (nRouteLen % 8) != 0)
		return -1;
	for (int i = 0; i < nRouteLen; i += 8)
	{
		TStaticRoute tStaticRoute;
		tStaticRoute.dwIP = GET_DWORD(pPacket);
		pPacket += 4;
		tStaticRoute.dwNH = GET_DWORD(pPacket);
		pPacket += 4;
		vecStaticRoute.push_back(tStaticRoute);
	}

	return pPacket - pTemp;
}

int OptionEncode(byte *pPacket, int nLength, TOption *pOption)
{
	byte *pTemp = pPacket;

	//DHCP_OPT_SUBNET_MASK
	if (pOption->dwSubnetMask != 0)
	{
		int nOffset = ::EncodeDword(pPacket, nLength, DHCP_OPT_SUBNET_MASK, pOption->dwSubnetMask);
		nLength -= nOffset;
		CHECK_ADVANCE(pPacket, nOffset);
	}
	//DHCP_OPT_ROUTER
	if (!pOption->vecRouter.empty())
	{
		int nOffset = ::EncodeVecDword(pPacket, nLength, DHCP_OPT_ROUTER, pOption->vecRouter);
		nLength -= nOffset;
		CHECK_ADVANCE(pPacket, nOffset);
	}
	//DHCP_OPT_DNS_SERVER
	if (!pOption->vecDNSServer.empty())
	{
		int nOffset = ::EncodeVecDword(pPacket, nLength, DHCP_OPT_DNS_SERVER, pOption->vecDNSServer);
		nLength -= nOffset;
		CHECK_ADVANCE(pPacket, nOffset);
	}
	//DHCP_OPT_LOG_SERVER
	if (!pOption->vecLogServer.empty())
	{
		int nOffset = ::EncodeVecDword(pPacket, nLength, DHCP_OPT_LOG_SERVER, pOption->vecLogServer);
		nLength -= nOffset;
		CHECK_ADVANCE(pPacket, nOffset);
	}
	//DHCP_OPT_INTERF_MTU
	if (pOption->wInterfMTU != 0)
	{
		int nOffset = ::EncodeWord(pPacket, nLength, DHCP_OPT_INTERF_MTU, pOption->wInterfMTU);
		nLength -= nOffset;
		CHECK_ADVANCE(pPacket, nOffset);
	}
	//DHCP_OPT_STATIC_ROUTE
	if (!pOption->vecStaticRoute.empty())
	{
		int nOffset = ::EncodeStaticRoute(pPacket, nLength, DHCP_OPT_STATIC_ROUTE, pOption->vecStaticRoute);
		nLength -= nOffset;
		CHECK_ADVANCE(pPacket, nOffset);
	}
	//DHCP_OPT_ARP_CACHE_TIME
	if (pOption->dwARPCacheTime != 0)
	{
		int nOffset = ::EncodeDword(pPacket, nLength, DHCP_OPT_ARP_CACHE_TIME, pOption->dwARPCacheTime);
		nLength -= nOffset;
		CHECK_ADVANCE(pPacket, nOffset);
	}
	//DHCP_OPT_NTP_SERVER
	if (!pOption->vecNTPServer.empty())
	{
		int nOffset = ::EncodeVecDword(pPacket, nLength, DHCP_OPT_NTP_SERVER, pOption->vecNTPServer);
		nLength -= nOffset;
		CHECK_ADVANCE(pPacket, nOffset);
	}
	//DHCP_OPT_IP_LEASE_TIME
	if (pOption->dwIPLeaseTime != 0)
	{
		int nOffset = ::EncodeDword(pPacket, nLength, DHCP_OPT_IP_LEASE_TIME, pOption->dwIPLeaseTime);
		nLength -= nOffset;
		CHECK_ADVANCE(pPacket, nOffset);
	}
	//DHCP_OPT_MSG_TYPE
	if (pOption->byMsgType != 0)
	{
		int nOffset = ::EncodeByte(pPacket, nLength, DHCP_OPT_MSG_TYPE, pOption->byMsgType);
		nLength -= nOffset;
		CHECK_ADVANCE(pPacket, nOffset);
	}
	//DHCP_OPT_SERVER_ID
	if (pOption->dwServerID != 0)
	{
		int nOffset = ::EncodeDword(pPacket, nLength, DHCP_OPT_SERVER_ID, pOption->dwServerID);
		nLength -= nOffset;
		CHECK_ADVANCE(pPacket, nOffset);
	}

	return pTemp - pPacket;
}

int OptionDecode(TOption *pOption, byte *pPacket, int nLength)
{
	byte *pTemp = pPacket;
	
	int nOffset;
	while (nLength != 0)
	{
		switch (*pPacket)
		{
		case DHCP_OPT_SUBNET_MASK:
			nOffset = ::DecodeDword(pOption->dwSubnetMask, pPacket, nLength, *pPacket);
			break;
		case DHCP_OPT_ROUTER:
			nOffset = ::DecodeVecDword(pOption->vecRouter, pPacket, nLength, *pPacket);
			break;
		case DHCP_OPT_DNS_SERVER:
			nOffset = ::DecodeVecDword(pOption->vecDNSServer, pPacket, nLength, *pPacket);
			break;
		case DHCP_OPT_LOG_SERVER:
			nOffset = ::DecodeVecDword(pOption->vecLogServer, pPacket, nLength, *pPacket);
			break;
		case DHCP_OPT_INTERF_MTU:
			nOffset = ::DecodeWord(pOption->wInterfMTU, pPacket, nLength, *pPacket);
			break;
		case DHCP_OPT_STATIC_ROUTE:
			nOffset = ::DecodeStaticRoute(pOption->vecStaticRoute, pPacket, nLength, *pPacket);
			break;
		case DHCP_OPT_ARP_CACHE_TIME:
			nOffset = ::DecodeDword(pOption->dwARPCacheTime, pPacket, nLength, *pPacket);
			break;
		case DHCP_OPT_NTP_SERVER:
			nOffset = ::DecodeVecDword(pOption->vecNTPServer, pPacket, nLength, *pPacket);
			break;
		case DHCP_OPT_IP_LEASE_TIME:
			nOffset = ::DecodeDword(pOption->dwIPLeaseTime, pPacket, nLength, *pPacket);
			break;
		case DHCP_OPT_MSG_TYPE:
			nOffset = ::DecodeByte(pOption->byMsgType, pPacket, nLength, *pPacket);
			break;
		case DHCP_OPT_SERVER_ID:
			nOffset = ::DecodeDword(pOption->dwServerID, pPacket, nLength, *pPacket);
			break;
		default:
			nOffset = 2 + pPacket[1];
			if (nOffset > nLength)
				nOffset = -1;
			break;
		}
		nLength -= nOffset;
		CHECK_ADVANCE(pPacket, nOffset);
	}

	return pTemp - pPacket;
}
