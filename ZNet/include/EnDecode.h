#ifndef __EnDecode_h__
#define __EnDecode_h__

#include <windows.h>

#define _DEFBLOCKSIZE		16

#ifdef _UNICODE
#define fnEncode6BitBuf	fnEncode6BitBufW
#define fnDecode6BitBuf fnDecode6BitBufW
#else
#define fnEncode6BitBuf	fnEncode6BitBufA
#define fnDecode6BitBuf fnDecode6BitBufA
#endif

int WINAPI			 fnEncode6BitBufW(unsigned char *pszSrc, TCHAR *pszDest, int nSrcLen, int nDestLen);
int WINAPI			 fnDecode6BitBufW(TCHAR *pwszSrc, char *pszDest, int nDestLen);

int WINAPI			 fnEncode6BitBufA(unsigned char *pszSrc, char *pszDest, int nSrcLen, int nDestLen);
int WINAPI			 fnDecode6BitBufA(char *pwszSrc, char *pszDest, int nDestLen);

#endif
