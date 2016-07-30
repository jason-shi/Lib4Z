#include "PRIVATE_NAMESPACE_BEGIN"

#ifndef __Common_h__
#define __Common_h__

#undef HEX_2_DEC
#define HEX_2_DEC(hex) \
		((hex) >= L'0' && (hex) <= L'9' ? (hex) - L'0' : \
		(hex) >= L'A' && (hex) <= L'F' ? (hex) - L'A' + 10 : (hex) - L'a' + 10)

#endif

#include "PRIVATE_NAMESPACE_END"
