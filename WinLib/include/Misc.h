#ifndef	__MISC_H__
#define	__MISC_H__

#include <mbctype.h>

BOOL				 FullPath(LPTSTR path, LPCTSTR file);
void				 ChangeSpaceToNull(char *pszData);
int					 memlen(const char *str);
int					 AnsiStrToVal(const char *nptr);
char				*ValToAnsiStr(unsigned long val, char *buf);
bool				 IsDigit(WPARAM wParam);
HFONT				 CreateSimpleFont(int cHeight);

inline int Rand(int factor)
{
	return rand() % factor;
}

inline double RandDbl(double up, double base, double precision = 0.001)
{
	return rand() % (int)((up - base) / precision) * precision + base;
}

class CSecurityAttributes: public _SECURITY_ATTRIBUTES {
  public:
	CSecurityAttributes(void *sd=NULL, BOOL ih=FALSE)
	{
		nLength = sizeof(SECURITY_ATTRIBUTES);
		lpSecurityDescriptor = sd;
		bInheritHandle = ih;
	}
};

#ifdef _DEBUG

void __cdecl DebugPrintf(const char *fmt, ...);
#define	TRACE	::DebugPrintf

bool AssertFailed(const char *expr, const char *file, unsigned line);

#define	ASSERT(exp)	\
	do \
		if (!(exp) && AssertFailed(#exp, __FILE__, __LINE__)) \
			DebugBreak(); \
	while (0)

#else

inline void __cdecl DebugPrintf(const char *, ...) { }
#define	TRACE	1? (void)0: ::DebugPrintf

#define ASSERT(exp)	(void)(0)

#endif

#pragma warning(disable: 4127)

#endif
