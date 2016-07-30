/******************************************************************************
 * 1.HKEY_LOCAL_MACHINE ��������������������õ���Ŀ�����а������ڴ�������
 *   ϵͳ���塢�ڴ���Ѱ�װ�������Ӳ������Ϣ�� �� 
 * 2.HKEY_CLASSES_ROOT �����ĵ����ͺ� OLE\COM ��ص���Ϣ��֧�ּ����������
 *   HKEY_LOCAL_MACHINE�Ĵ������� �� 
 * 3.HKEY_USERS ����Ĭ���û���ѡ���ã�Ҳ��Ϊ�����û�����ѡ���á� �� 
 * 4.HKEY_CLASSES_USER �����ڵ�ǰ����¼���û��������Ϣ�� �� 
 * 5.HKEY_CURRENT_CONFIG �����˵�ǰϵͳ���õ���Ϣ�� �� 
 * 6.����һ����һ�㲻����֣�ֻ�����õľ�������Ż��С�
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
