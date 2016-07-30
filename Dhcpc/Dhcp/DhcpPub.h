#include "PRIVATE_NAMESPACE_BEGIN"

#ifndef __DhcpPub_h__
#define __DhcpPub_h__

typedef std::vector<dword>		VECDWORD;

#define ST_DHCP_INITIAL			0
#define ST_DHCP_SELECT			1
#define ST_DHCP_REQUEST			2
#define ST_DHCP_BOUND			3
#define ST_DHCP_RENEW			4
#define ST_DHCP_REBIND			5
#define MAX_DHCP_STATE			6

#define ID_DHCP_DISCOVER		1
#define ID_DHCP_OFFER			2
#define ID_DHCP_REQUEST			3
#define ID_DHCP_DECLINE			4
#define ID_DHCP_ACK				5
#define ID_DHCP_NACK			6
#define ID_DHCP_RELEASE			7
#define ID_DHCP_INFORM			8

#define DHCP_OPCODE_UP			1
#define DHCP_OPCODE_DOWN		2

#define DHCP_HWTYPE_ETH			1

#define DHCP_OPT_SUBNET_MASK	0x01 // 4
#define DHCP_OPT_ROUTER			0x03 // n * 4
#define DHCP_OPT_DNS_SERVER		0x06 // n * 4
#define DHCP_OPT_LOG_SERVER		0x07 // n * 4
#define DHCP_OPT_INTERF_MTU		0x1A // 2
#define DHCP_OPT_STATIC_ROUTE	0x21 // n * 8
#define DHCP_OPT_ARP_CACHE_TIME	0x23 // 4
#define DHCP_OPT_NTP_SERVER		0x2A // n * 4
#define DHCP_OPT_IP_LEASE_TIME	0x33 // 4
#define DHCP_OPT_MSG_TYPE		0x35 // 1
#define DHCP_OPT_SERVER_ID		0x36 // 4

#define TM_RENT_UPDATE			0x01
#define TM_RENT_REBIND			0x02
#define TM_RENT_RELEASE			0x03
#define TM_RENT_APPLY			0x04

struct TStaticRoute {
	dword			 dwIP;
	dword			 dwNH;
};

typedef std::vector<TStaticRoute>	VECSTATICROUTE;

struct TOption {
	dword			 dwSubnetMask;
	VECDWORD		 vecRouter;
	VECDWORD		 vecDNSServer;
	VECDWORD		 vecLogServer;
	word			 wInterfMTU;
	VECSTATICROUTE	 vecStaticRoute;
	dword			 dwARPCacheTime;
	VECDWORD		 vecNTPServer;
	dword			 dwIPLeaseTime;
	byte			 byMsgType;
	dword			 dwServerID;
};

struct TDhcpMsg {
	byte			 byOpCode;
	byte			 byHWType;
	byte			 byHWLen;
	byte			 byHops;
	dword			 dwTransID;
	word			 wSeconds;
	word			 wFlags;
	dword			 dwClientIP;
	dword			 dwUserIP;
	dword			 dwServerIP;
	dword			 dwAgentIP;
	byte			 byHWAddr[16];
	char			 szHostName[64];
	char			 szBootFile[128];
	TOption			 tOption;
};

#endif

#include "PRIVATE_NAMESPACE_END"
