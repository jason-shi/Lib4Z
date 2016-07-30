#include "PRIVATE_NAMESPACE_BEGIN"

#ifndef __IP6Set_h__
#define __IP6Set_h__

struct IP6Set;
typedef std::vector<IP6Set *>		VECIP6SET;

struct IP6Set {
	byte			 byMAC[6];
	word			 wAddr[8];
	dword			 dwPeriod;
	dword			 dwElapse;

	IP6Set(byte byMAC[6], word wAddr[8], dword dwPeriod, dword dwElapse);
};

IP6Set				*FindIP6Set(VECIP6SET& vecIP6Set, byte byMAC[6]);
IP6Set				*FindIP6Set(VECIP6SET& vecIP6Set, word wAddr[8]);
void				 InsertIP6Set(VECIP6SET& vecIP6Set, IP6Set *pIP6Set);
void				 RemoveIP6Set(VECIP6SET& vecIP6Set, IP6Set *pIP6Set);
void				 ModifyIP6Set(VECIP6SET& vecIP6Set, IP6Set *pIP6Set);

#endif

#include "PRIVATE_NAMESPACE_END"
