#include "PRIVATE_NAMESPACE_BEGIN"

#ifndef __DhcpCode_h__
#define __DhcpCode_h__

struct TDhcpMsg;

static int			 DhcpdEncode(byte *pPacket, int nLength, TDhcpMsg *pDhcpMsg);
static int			 DhcpdDecode(TDhcpMsg *pDhcpMsg, byte *pPacket, int nLength);

#include "DhcpCode.cpp"

#endif

#include "PRIVATE_NAMESPACE_END"
