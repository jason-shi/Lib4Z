#ifndef __FILE_H__
#define __FILE_H__

#include <io.h>
#include <stdio.h>
#include <share.h>

//
//	CFile Class
//
class CFile {
  public:
	enum	{
		begin = SEEK_SET,
		current = SEEK_CUR,
		end = SEEK_END,
	} ;

  public:
	CFile(): fp(0) {}
	CFile(const char *file, const char *mode="r");
	~CFile();

	bool Open(const char *file, const char *mode="r");
	bool Close();

	int Read(void *buffer, int length);
	int Write(const void *buffer, int length);
	char* Gets(char *buffer, int length);
	int Puts(char *buffer);
	long Seek(long offset, int from);
	void Rewind();
	FILE* GetFileHandle() { return fp; }

#ifdef _MSC_VER
	long GetFileSize() { return _filelength(_fileno(fp)); }
#else
  #ifdef __BORLANDC__
	long GetFileSize() { return filelength(_fileno(fp)); }
  #endif
#endif

	bool IsOk() const { return fp != 0; }
	bool Error() { return ferror(fp) != 0; }

	bool operator !() { return fp == 0 || ferror(fp) != 0; }

  protected:
	FILE *fp;
} ;

inline CFile::CFile(const char *file, const char *mode)
{
	Open(file, mode);
}

inline CFile::~CFile()
{
	Close();
}

inline bool CFile::Open(const char *file, const char *mode)
{
#ifdef _MSC_VER
	#ifdef _UNICODE
		WCHAR szFile[_MAX_PATH];
		WCHAR szMode[4];
		MultiByteToWideChar( CP_ACP, 0, file, -1, szFile, sizeof( szFile ) / sizeof( WCHAR ) );
		MultiByteToWideChar( CP_ACP, 0, mode, -1, szMode, sizeof( szMode ) / sizeof( WCHAR ) );
		fp = _wfsopen(szFile, szMode, _SH_DENYWR);
	#else
		fp = _fsopen(file, mode, _SH_DENYWR);
	#endif
#else
	fp = fopen(file, mode);
#endif
	return fp != 0;
}

inline bool CFile::Close()
{
	if (fp != 0) {
		fclose(fp);
		fp = 0;
	}
	return true;
}

inline int CFile::Read(void *buffer, int length)
{
	return fread(buffer, 1, length, fp);
}

inline int CFile::Write(const void *buffer, int length)
{
	return fwrite(buffer, 1, length, fp);
}

inline char* CFile::Gets(char *buffer, int length)
{
	return fgets(buffer, length, fp);
}

inline int CFile::Puts(char *buffer)
{
	return fputs(buffer, fp);
}

inline long CFile::Seek(long offset, int from)
{
	return fseek(fp, offset, from);
}

inline void CFile::Rewind()
{
	return rewind(fp);
}

#endif
