/******************************************************************************
 * 1.HKEY_LOCAL_MACHINE 包含描述计算机及其配置的条目。其中包括关于处理器、
 *   系统主板、内存和已安装的软件和硬件的信息。 　 
 * 2.HKEY_CLASSES_ROOT 是与文档类型和 OLE\COM 相关的信息的支持键。这个键是
 *   HKEY_LOCAL_MACHINE的从属键。 　 
 * 3.HKEY_USERS 用作默认用户首选设置，也作为单个用户的首选设置。 　 
 * 4.HKEY_CLASSES_USER 是用于当前（登录）用户的相关信息。 　 
 * 5.HKEY_CURRENT_CONFIG 包含了当前系统配置的信息。 　 
 * 6.还有一个，一般不会出现，只有配置的局域网后才会有。
 *****************************************************************************/
#ifndef __Registry_h__
#define __Registry_h__

class CRegistry {
  public:
	CRegistry();
	CRegistry(HKEY hKey, const char* lpszSubKey);
	virtual ~CRegistry();

  public:
	BOOL			 Open(HKEY hKey, const char* lpszSubKey);
	BOOL			 Close();

  public:
	BOOL			 Read(const char* lpszValueName, LPVOID lpReturnBuffer);
	BOOL			 ReadDWORD(const char* lpszValueName, DWORD *pdwData, DWORD *pdwLastError);
	BOOL			 ReadString(const char* lpszValueName, LPVOID lpReturnBuffer, DWORD *pdwLastError);

  public:
	BOOL			 Write(const char* lpszValueName, LPVOID lpData, DWORD dwType, int nSize);
	BOOL			 WriteDWORD(const char* lpszValueName, DWORD dwData, DWORD *pdwLastError);
	BOOL			 WriteString(const char* lpszValueName, LPVOID lpData, DWORD *pdwLastError);

  public:
	static BOOL		 CreateKey(HKEY hKey, const char* lpszSubKey, const char* lpszClass);
	static BOOL		 DeleteKey(HKEY hKey, const char* lpszSubKey);

  protected:
	HKEY			 m_hKey;
	DWORD			 m_dwLastError;
};

#endif
