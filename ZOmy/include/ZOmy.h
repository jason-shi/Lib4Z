#ifndef __ZOmy_h__
#define __ZOmy_h__

#pragma warning(disable: 4996)

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }

typedef	unsigned char			byte;
typedef unsigned short			word;
typedef unsigned int			dword;
typedef unsigned long long		qword;


#define CHECK_ADVANCE(pBuff, nOffset) \
	{ \
		if (nOffset == -1) \
			return -1; \
		pBuff += nOffset; \
	}

#define GET_BYTE(pBuff) \
		((pBuff)[0])

#define SET_BYTE(pBuff, value) \
		(pBuff)[0] = (value);

#define GET_WORD(pBuff) \
		(((pBuff)[0] << 8) | pBuff[1])

#define SET_WORD(pBuff, value) \
		{ \
			(pBuff)[0] = (value) >> 8; \
			(pBuff)[1] = (value) & 0xFF; \
		}

#define GET_DWORD(pBuff) \
		(((pBuff)[0] << 24) | ((pBuff)[1] << 16) | ((pBuff)[2] << 8) | (pBuff)[3])

#define SET_DWORD(pBuff, value) \
		{ \
			(pBuff)[0] = (value) >> 24; \
			(pBuff)[1] = (value) >> 16; \
			(pBuff)[2] = (value) >> 8; \
			(pBuff)[3] = (value) & 0xFF; \
		}

#define HEX_2_DEC(hex) \
		((hex) >= '0' && (hex) <= '9' ? (hex) - '0' : \
		(hex) >= 'A' && (hex) <= 'F' ? (hex) - 'A' + 10 : (hex) - 'a' + 10)

#ifdef	_MSC_VER
	#ifdef	_DEBUG
		#pragma comment(lib, "ZOmyD.lib")
	#else
		#pragma comment(lib, "ZOmy.lib")
	#endif
#endif

#endif
