#pragma once
#ifdef WIN32
#include <Winsock2.h>
#pragma comment(lib, "Ws2_32.lib")
#pragma	warning(disable: 4244)
#else
#include <netinet/in.h>
#endif

#ifndef  SINGLETON
#define  SINGLETON(type) \
	static inline type* GetInstancePtr() \
	{ \
		static type _instance; \
		return &_instance; \
	}
#endif

#define MAX_PACKET_LEN 1024 * 100
#define MAX_UDP_PACKET_LEN 1024 * 2

#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#define ARRAY(a)	sizeof(a)/sizeof(a[0])

//服务器最大在线人数
#define MAX_PLAYER_CAPABILITY 1200
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;

typedef long long int64;

inline int64 htonll(int64 n) {
	return (((int64)htonl(n)) << 32) | htonl(n >> 32);
}
inline int64 ntohll(int64 n) {
	return (((int64)ntohl(n)) << 32) | ntohl(n >> 32);
}
inline float ntoh_float(float net_float) {
	int host_int;
	host_int = ntohl(*((int *) &net_float));
	return *((float *) &host_int);
}

inline float hton_float(float host_float) {
	int net_int32;
	net_int32 = htonl(*((int *) &host_float));
	return *((float *) &net_int32);
}
inline double ntoh_double(double net_double) {
	int64 host_int64;
	host_int64 = ntohll(*((int64 *) &net_double));
	return *((double *) &host_int64);
}

inline double hton_double(double host_double) {
	int64 net_int64;
	net_int64 = htonll(*((int64 *) &host_double));
	return *((double *) &net_int64);
}
