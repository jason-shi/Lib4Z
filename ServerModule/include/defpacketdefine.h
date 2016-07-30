#ifndef __defpacketdefine_h__
#define __defpacketdefine_h__

struct pt_header
{
	short			 opcode;
};

#ifdef NO_INLINE_SERIAL

#define NET_PACKET(packet) \
struct packet; \
	CInputStream& operator>>(CInputStream& is, packet& pt); \
	COutputStream& operator<<(COutputStream& os, const packet& pt); \
struct packet : public pt_header

#define NET_GET(packet) \
	CInputStream& operator>>(CInputStream& is, packet& pt)

#define NET_PUT(packet) \
	COutputStream& operator<<(COutputStream& os, const packet& pt)

#else

#define NET_PACKET(packet) \
struct packet; \
	inline CInputStream& operator>>(CInputStream& is, packet& pt); \
	inline COutputStream& operator<<(COutputStream& os, const packet& pt); \
struct packet : public pt_header

#define NET_GET(packet) \
	CInputStream& operator>>(CInputStream& is, packet& pt)

#define NET_PUT(packet) \
	COutputStream& operator<<(COutputStream& os, const packet& pt)

#endif

#define DISPATCH_MESSAGE(packet) \
{ \
	packet obj; \
	is >> obj; \
	OnPacket(obj); \
	break;	\
}

#define DISPATCH_MESSAGE_REF(packet, parm) \
{ \
	packet pt(parm); \
	is >> pt; \
	OnPacket(pt); \
	break;	\
}

enum {
	bd_net_echo_not = 0,
	bd_net_connect,
	bd_net_closeconnect,
};

NET_PACKET(pt_bd_net_echo_not)
{
	unsigned int	 nTime;
};
NET_PACKET(pt_bd_net_connect)
{
	int				 serverID;
};
NET_PACKET(pt_bd_net_closeconnect)
{
};
NET_GET(pt_bd_net_echo_not)
{
	return is >> pt.opcode >> pt.nTime;
}
NET_PUT(pt_bd_net_echo_not)
{
	return os << pt.opcode << pt.nTime;
}

#endif
