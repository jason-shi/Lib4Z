#include "StdAfx.h"
#include "Registry.h"

CRegistry::CRegistry()
{
	m_hKey = NULL;
}

CRegistry::CRegistry(HKEY hKey, const char* lpszSubKey)
{
	Open(hKey, lpszSubKey);
}

CRegistry::~CRegistry()
{
	Close();
}

BOOL CRegistry::Open(HKEY hKey, const char* lpszSubKey)
{
	LONG lRet = ::RegOpenKeyExA(hKey, lpszSubKey, 0, KEY_ALL_ACCESS, &m_hKey);
	m_dwLastError = GetLastError();

	return lRet == ERROR_SUCCESS;
}

BOOL CRegistry::Close()
{
	if (m_hKey == NULL)
		return FALSE;

	LONG lRet = ::RegCloseKey(m_hKey);
	if (lRet == ERROR_SUCCESS)
		m_hKey = NULL;
	m_dwLastError = GetLastError();

	return lRet == ERROR_SUCCESS;
}

BOOL CRegistry::Read(const char* lpszValueName, LPVOID lpReturnBuffer)
{
	if (m_hKey == NULL)
		return FALSE;

	DWORD dwSize = 5000;
	LONG lRet = ::RegQueryValueExA(m_hKey, lpszValueName, NULL, NULL, (unsigned char *)lpReturnBuffer, &dwSize);
	m_dwLastError = GetLastError();

	return lRet == ERROR_SUCCESS;
}

BOOL CRegistry::ReadDWORD(const char* lpszValueName, DWORD *pdwData, DWORD *pdwLastError)
{
	if (m_hKey == NULL)
		return FALSE;

	BOOL bRet = Read(lpszValueName, pdwData);
	if (pdwLastError != NULL)
		*pdwLastError = m_dwLastError;

	return bRet;
}

BOOL CRegistry::ReadString(const char* lpszValueName, LPVOID lpReturnBuffer, DWORD *pdwLastError)
{
	if (m_hKey == NULL)
		return FALSE;

	BOOL bRet = Read(lpszValueName, lpReturnBuffer);
	if (pdwLastError != NULL)
		*pdwLastError = m_dwLastError;

	return bRet;
}

BOOL CRegistry::WriteDWORD(const char* lpszValueName, DWORD dwData, DWORD *pdwLastError)
{
	if (m_hKey == NULL)
		return FALSE;

	BOOL bRet = Write(lpszValueName, &dwData, REG_DWORD, sizeof(DWORD));
	if (pdwLastError != NULL)
		*pdwLastError = m_dwLastError;

	return bRet;
}

BOOL CRegistry::WriteString(const char* lpszValueName, LPVOID lpData, DWORD *pdwLastError)
{
	if (m_hKey == NULL)
		return FALSE;

	BOOL bRet = Write(lpszValueName, lpData, REG_SZ, (DWORD)strlen((const char *)lpData) + 1);
	if (pdwLastError != NULL)
		*pdwLastError = m_dwLastError;

	return bRet;
}

BOOL CRegistry::Write(const char* lpszValueName, LPVOID lpData, DWORD dwType, int nSize)
{
	if (m_hKey == NULL)
		return FALSE;

	DWORD dwSize = 5000;
	LONG lRet = ::RegSetValueExA(m_hKey, lpszValueName, 0, dwType, (unsigned char *)lpData, nSize);
	m_dwLastError = GetLastError();

	return lRet == ERROR_SUCCESS;
}

BOOL CRegistry::CreateKey(HKEY hKey, const char* lpszSubKey, const char* lpszClass)
{
	HKEY hOpenedKey;
	DWORD dwDisposition;

	LONG lRet = ::RegCreateKeyExA(hKey, lpszSubKey, 0, (char *)lpszClass,
								 REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS,
								 NULL, &hOpenedKey, &dwDisposition);
	if (lRet != ERROR_SUCCESS)
		::RegCloseKey(hOpenedKey);

	return lRet == ERROR_SUCCESS;
}

BOOL CRegistry::DeleteKey(HKEY hKey, const char* lpszSubKey)
{
	LONG lRet = ::RegDeleteKeyA(hKey, lpszSubKey);

	return lRet == ERROR_SUCCESS;
}
