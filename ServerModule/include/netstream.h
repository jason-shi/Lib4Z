#pragma once
#include <assert.h>
#include <string.h>
#include <sys/types.h>

#ifdef WIN32
#include <Winsock2.h>
#pragma comment(lib, "Ws2_32.lib")
#else
	#include <netinet/in.h>
#endif

#include <string>
#include <vector>
#include <map>
#include <globaldefine.h>

#define DEF_BASETYPE_SERIA_WRITE_1(type) \
	inline COutputStream& operator<<(const type& t) \
	{ \
		WriteData(&t, sizeof(t)); \
		return *this; \
	}
#define DEF_BASETYPE_SERIA_WRITE_2(type) \
	inline COutputStream& operator<<(const type& t) \
	{ \
	type t2 = htons(t); \
	WriteData(&t2, sizeof(type)); \
	return *this; \
	}
#define DEF_BASETYPE_SERIA_WRITE_4(type) \
	inline COutputStream& operator<<(const type& t) \
	{ \
	type t2 = htonl(t); \
	WriteData(&t2, sizeof(type)); \
	return *this; \
	}
#define DEF_BASETYPE_SERIA_WRITE_8(type) \
	inline COutputStream& operator<<(const type& t) \
	{ \
	type t2 = htonll(t); \
	WriteData(&t2, sizeof(type)); \
	return *this; \
	}

#define DEF_BASETYPE_SERIA_WRITE_FLOAT(type) \
	inline COutputStream& operator<<(const type& t) \
	{ \
	type t2 = hton_float(t); \
	WriteData(&t2, sizeof(type)); \
	return *this; \
	}

#define DEF_BASETYPE_SERIA_READ_1(type) \
	inline CInputStream& operator>>(type& t)  \
	{ \
	ReadData(&t, sizeof(type)); \
	return *this; \
	}

#define DEF_BASETYPE_SERIA_READ_2(type) \
	inline CInputStream& operator>>(type& t)  \
	{ \
	ReadData(&t, sizeof(type)); \
	t = ntohs(t); \
	return *this; \
	}

#define DEF_BASETYPE_SERIA_READ_4(type) \
	inline CInputStream& operator>>(type& t)  \
	{ \
	ReadData(&t, sizeof(type)); \
	t = ntohl(t); \
	return *this; \
	}

#define DEF_BASETYPE_SERIA_READ_8(type) \
	inline CInputStream& operator>>(type& t)  \
	{ \
	ReadData(&t, sizeof(type)); \
	t = ntohll(t); \
	return *this; \
	}

#define DEF_BASETYPE_SERIA_READ_FLOAT(type) \
	inline CInputStream& operator>>(type& t)  \
	{ \
	ReadData(&t, sizeof(type)); \
	t = ntoh_float(t); \
	return *this; \
	}





class CInputStream
{
public:
	CInputStream(const char* pData, int nLen)
	{
		m_pData = (char*)pData;
		m_nLen = nLen;
		m_nOffset = 0;
	}
	void Reset() { m_nOffset = 0; }
	bool ReadData(void* pData, int nLen)
	{
		if(m_nOffset + nLen <= m_nLen)
		{
			memcpy(pData, &m_pData[m_nOffset], nLen);
			m_nOffset += nLen;
			return true;
		}
		return false;
	}
	CInputStream& operator>>(std::basic_string<char>& str)
	{
		short len = -1;
		*this >> len;
		if(len < 0 || len > 1024*10 || len > m_nLen - m_nOffset)
		{
			str.clear();
		}
		else
		{
			char szStr[1024*10];
			ReadData(szStr, len);
			str.reserve(len);
			str.assign(szStr, len);
		}
		return *this;
	}
	template<class T>
		CInputStream& operator>>(std::vector<T>& obj)
	{
		short len = -1;
		*this >> len;

		if(len < 0 || len > 512 || len > m_nLen - m_nOffset)
		{
			obj.clear();
		}
		else
		{
			obj.reserve(len);
			for(int i = 0; i < len; ++i)
			{
				T element;
				*this >> element;
				obj.push_back(element);
			}
		}
		return *this;
	}
	template<class T>
		CInputStream& operator>>(std::vector<T*>& obj)
	{
		short len;
		*this >> len = -1;

		if(len < 0 || len > 512 || len > m_nLen - m_nOffset)
		{
			obj.clear();
		}
		else
		{
			obj.reserve(len);
			for(int i = 0; i < len; ++i)
			{
				T* element = new T;
				*this >> *element;
				obj.push_back(element);
			}
		}
		return *this;
	}
	template<class KEY, class T>
		CInputStream& operator>>(std::map<KEY, T>& obj)
	{
		short len = -1;
		*this >> len;
		if(len < 0 || len > 512 || len > m_nLen - m_nOffset)
		{
			obj.clear();
		}
		else
		{
			for(int i = 0; i < len; ++i)
			{
				KEY key;
				T second;
				*this >> key >> second;
				obj[key] = second;
			}
		}
		return *this;
	}

	template<class KEY, class T>
		CInputStream& operator>>(std::map<KEY, T*>& obj)
	{
		short len = -1;
		*this >> len;
		if(len < 0 || len > 512 || len > m_nLen - m_nOffset)
		{
			obj.clear();
		}
		else
		{
			for(int i = 0; i < len; ++i)
			{
				KEY key;
				T* second = new T;
				*this >> key >> *second;
				obj[key] = second;
			}
		}
		return *this;
	}
	DEF_BASETYPE_SERIA_READ_1(char);
	DEF_BASETYPE_SERIA_READ_1(unsigned char);
	DEF_BASETYPE_SERIA_READ_2(short int);
	DEF_BASETYPE_SERIA_READ_2(unsigned short int);
	DEF_BASETYPE_SERIA_READ_4(int);
	DEF_BASETYPE_SERIA_READ_4(unsigned int);
	DEF_BASETYPE_SERIA_READ_FLOAT(float);
	DEF_BASETYPE_SERIA_READ_8(int64);

private:
	char* m_pData;
	int m_nLen;
	int m_nOffset;
};




class COutputStream
{
public:
	COutputStream()
	{
		m_pData = m_pOwnerBuffer;
		m_nMaxLen = MAX_PACKET_LEN - 6;
		m_nOffset = 0;
	}
	COutputStream(char* pData, int nMaxLen)
	{
		m_pData = pData;
		m_nMaxLen = nMaxLen;
		m_nOffset = 0;
	}
	bool WriteData(const void* pData, int nLen)
	{
		if(m_nOffset + nLen <= m_nMaxLen)
		{
			memcpy(&m_pData[m_nOffset], pData, nLen);
			m_nOffset += nLen;
			return true;
		}
		return false;
	}
	COutputStream& operator<<(const std::basic_string<char>& str)
	{
		short len = (short)str.size();
		if(len > m_nMaxLen - m_nOffset || len > 1024*10)
		{
			len = 0;
			*this << len;
		}
		else
		{
			*this << len;
			WriteData(str.data(), (int)str.length());
		}
		return *this;
	}
	template<class T>
		COutputStream& operator<<(const std::vector<T>& obj)
	{
		short len = (short)obj.size();
		if(len > m_nMaxLen - m_nOffset || len > 512)
		{
			len = 0;
			*this << len;
		}
		else
		{
			*this << len;
			for(int i = 0; i < len; ++i)
			{
				*this << obj[i];
			}
		}
		return *this;
	}
	template<class T>
		COutputStream& operator<<(const std::vector<T*>& obj)
	{
		short len = (short)obj.size();
		if(len > m_nMaxLen - m_nOffset || len > 512)
		{
			len = 0;
			*this << len;
		}
		else
		{
			*this << len;
			for(int i = 0; i < len; ++i)
			{
				*this << *obj[i];
			}
		}
		return *this;
	}

	template<typename KEY, typename T>
		COutputStream& operator<<(const std::map<KEY, T>& obj)
	{
		short len = (short)obj.size();
		if(len > m_nMaxLen - m_nOffset || len > 512)
		{
			len = 0;
			*this << len;
		}
		else
		{
			*this << len;
			typename std::map<KEY, T>::const_iterator iter = obj.begin();
			for(; iter != obj.end(); ++iter)
			{
				*this << iter->first << iter->second;
			}
		}
		return *this;
	}

	template<typename KEY, typename T>
		COutputStream& operator<<(const std::map<KEY, T*>& obj)
	{
		short len = (short)obj.size();
		if(len > m_nMaxLen - m_nOffset || len > 512)
		{
			len = 0;
			*this << len;
		}
		else
		{
			*this << len;
			typename std::map<KEY, T*>::const_iterator iter = obj.begin();
			for(; iter != obj.end(); ++iter)
			{
				*this << iter->first << *(iter->second);
			}
		}
		return *this;
	}
	char* GetData() { return m_pData; }
	int GetLength() { return m_nOffset; }
	DEF_BASETYPE_SERIA_WRITE_1(char);
	DEF_BASETYPE_SERIA_WRITE_1(unsigned char);
	DEF_BASETYPE_SERIA_WRITE_2(short int);
	DEF_BASETYPE_SERIA_WRITE_2(unsigned short int);
	DEF_BASETYPE_SERIA_WRITE_4(int);
	DEF_BASETYPE_SERIA_WRITE_4(unsigned int);
	DEF_BASETYPE_SERIA_WRITE_FLOAT(float);
	DEF_BASETYPE_SERIA_WRITE_8(int64);

private:
	char* m_pData;
	char m_pOwnerBuffer[MAX_PACKET_LEN];
	int m_nMaxLen;
	int m_nOffset;
};
