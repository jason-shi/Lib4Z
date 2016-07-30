#include "StdAfx.h"

BOOL FullPath(LPTSTR path, LPCTSTR file)
{
	LPTSTR part;
	if (!GetFullPathName(file, _MAX_PATH, path, &part))
	{
		_tcsncpy_s(path, _MAX_PATH, file, _MAX_PATH);
		return FALSE;
	}
	return TRUE;
}

void ChangeSpaceToNull(char *pszData)
{
	char *pszCheck = pszData;

	while (*pszCheck)
	{
		if (*pszCheck == 0x20)
		{
			*pszCheck = '\0';
			break;
		}

		pszCheck++;
	}
}

int memlen(const char *str)
{
    const char *eos = str;

    while(*eos++);

    return((int)(eos - str));
}

int AnsiStrToVal(const char *nptr)
{
    int c = (int)(unsigned char)*nptr++;
    int total = 0;

    while (c >= '0' && c <= '9') 
	{
        total = 10 * total + (c - '0');     /* accumulate digit */
        c = (int)(unsigned char)*nptr++;    /* get next char */
    }

	return total;
}

char *ValToAnsiStr(unsigned long val, char *buf)
{
    char *p;                /* pointer to traverse string */
    char *firstdig;         /* pointer to first digit */
    char temp;              /* temp char */
	char *next;
    unsigned digval;        /* value of digit */

    p = buf;

    firstdig = p;           /* save pointer to first digit */

    do {
        digval = (unsigned) (val % 10);
        val /= 10;	       /* get next digit */

        /* convert to ascii and store */
        if (digval > 9)
            *p++ = (char) (digval - 10 + 'a');  /* a letter */
        else
            *p++ = (char) (digval + '0');       /* a digit */
    } while (val > 0);

    /* We now have the digit of the number in the buffer, but in reverse
       order.  Thus we reverse them now. */

	next = p;
    *p-- = '\0';            /* terminate string; p points to last digit */

    do {
        temp = *p;
        *p = *firstdig;
        *firstdig = temp;   /* swap *p and *firstdig */
        --p;
        ++firstdig;         /* advance to next two digits */
    } while (firstdig < p); /* repeat until halfway */

	return next;
}

bool IsDigit(WPARAM wParam)
{
	return wParam >= 48 && wParam <= 57 || wParam >= 96 && wParam <= 105;
}

HFONT CreateSimpleFont(int cHeight)
{
	return CreateFont(cHeight, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, _T("宋体"));
}

#ifdef	_DEBUG

//
// 显示出错事件
//
void __cdecl DebugPrintf(const char *fmt, ...)
{
	char	tmp[4096];
	va_list	ap;
	va_start(ap, fmt);
	vsprintf_s(tmp, 4096, fmt, ap);

	WCHAR szTmp[4096];
	MultiByteToWideChar(CP_ACP, 0, tmp, -1, szTmp, sizeof(szTmp) / sizeof(WCHAR));
	OutputDebugString(szTmp);

	va_end(ap);
}

//
// ASSERT的辅助函式
//
bool AssertFailed(const char *exp, const char *file, unsigned line)
{
	DebugPrintf("Assertion failed: %s, file %s, line %d", exp, file, line);

	char	tmp[1024];
	sprintf_s(tmp, 1024, "%s, file %s, line %d", exp, file, line);

	HWND hWnd = ::GetActiveWindow();
	if (hWnd != NULL)
		hWnd = ::GetLastActivePopup(hWnd);

	MSG msg;
	BOOL bPostQuitNeed = ::PeekMessage(&msg, NULL, WM_QUIT, WM_QUIT, PM_REMOVE);

	WCHAR szTmp[1024];
	MultiByteToWideChar(CP_ACP, 0, tmp, -1, szTmp, sizeof(szTmp) / sizeof(WCHAR));
	int result = ::MessageBox(hWnd, szTmp, _T("Assertion Failed"),
					MB_TASKMODAL|MB_ICONHAND|MB_ABORTRETRYIGNORE|MB_SETFOREGROUND);

	if (bPostQuitNeed)
		PostQuitMessage(msg.wParam);

	switch (result) {
	  case IDIGNORE:
		return false;

	  case IDRETRY:
		return true;

	  default:
		abort();
	}
	return false;
}

#endif
