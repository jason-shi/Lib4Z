#include "PRIVATE_NAMESPACE_BEGIN"

#ifndef __OptionCode_h__
#define __OptionCode_h__

struct TOption;

static int			 OptionEncode(byte *pPacket, int nLength, TOption *pOption);
static int			 OptionDecode(TOption *pOption, byte *pPacket, int nLength);


#include "OptionCode.cpp"

#endif

#include "PRIVATE_NAMESPACE_END"
