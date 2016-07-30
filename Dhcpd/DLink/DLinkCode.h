#include "PRIVATE_NAMESPACE_BEGIN"

#ifndef __DLinkCode_h__
#define __DLinkCode_h__

struct TDLinkMsg;

static int			 DLinkEncode(byte *pPacket, int nLength, TDLinkMsg *pDLinkMsg);
static int			 DLinkDecode(TDLinkMsg *pDLinkMsg, byte *pPacket, int nLength);

#include "DLinkCode.cpp"

#endif

#include "PRIVATE_NAMESPACE_END"
