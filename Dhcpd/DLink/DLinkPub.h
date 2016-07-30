#include "PRIVATE_NAMESPACE_BEGIN"

#ifndef __DLinkPub_h__
#define __DLinkPub_h__

#define MAX_DLINK_HEAD_LEN		64
#define MAX_DLINK_DATA_LEN		2000
#define MAX_DLINK_BUFF_LEN		(MAX_DLINK_HEAD_LEN + MAX_DLINK_DATA_LEN)

#define EV_NET_PACKET			(EV_USER + 1)
#define EV_DLINK_DATA			(EV_USER + 2)

#define EV_JPROC				(EV_USER + 0x00010000)

#define EV_SET_JDLINK			(EV_JPROC + 1)
#define EV_SET_JDHCP			(EV_JPROC + 2)
#define EV_ADD_JDEST			(EV_JPROC + 3)
#define EV_DEL_JDEST			(EV_JPROC + 4)

#define DLINK_DEFAULT_HOPS		8

struct TDLinkMsg {
	char			 DA[6];
	char			 SA[6];
	word			 wProtocal;
	byte			 byVersion;
	byte			 byWhat;
	word			 wServType;
	word			 wLength;
	byte			 byFlag;
	byte			 byOffset;
	byte			 byServival;
	byte			 byUDPTCP;
	word			 wHeadChkSum;
	dword			 SIP;
	dword			 DIP;
	word			 SPort;
	word			 DPort;
	word			 wDataLen;
	word			 wDataChkSum;
	byte			 byData[MAX_DLINK_DATA_LEN];
};

struct TDLinkData {
	bool			 bBroadcast;

	byte			 byData[MAX_DLINK_DATA_LEN];
	word			 wDataLen;
};

#endif

#include "PRIVATE_NAMESPACE_END"
