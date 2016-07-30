#include "StdAfx.h"
using namespace std;

FT_Error FaceRequest(FTC_FaceID face_id,
					 FT_Library library,
					 FT_Pointer request_data,
					 FT_Face   *aface)
{
	FT_Error error;
	MyFace face = (MyFace)face_id;

	error = FT_New_Face(library, face->file_path, face->face_index, aface);
	if (error != FT_Err_Ok)
		return error;

	if (face->style & TS_ITALIC)
	{
		//倾斜度，越大就越斜
		float lean = 0.5f;
		FT_Matrix matrix;
		matrix.xx = 0x10000L;
		matrix.xy = lean * 0x10000L;
		matrix.yx = 0;
		matrix.yy = 0x10000L;
		FT_Set_Transform(*aface, &matrix, 0);
	}

	return FT_Err_Ok;
}

CZText::CZText()
{
	FT_Init_FreeType(&m_pLibrary);

	FTC_Manager_New(m_pLibrary, 0, 0, 0, FaceRequest, (FT_Pointer)NULL, &m_pCacheManager);

	FTC_CMapCache_New(m_pCacheManager, &m_pCMapCache);
	FTC_SBitCache_New(m_pCacheManager, &m_pSBitCache);

	//默认空白宽度
	m_nSpaceWidth = 16;

	//获取系统的Fonts目录
	char szBuff[_MAX_PATH];
	::GetWindowsDirectoryA(szBuff, _MAX_PATH);

	sprintf_s(m_szFontsDir, _MAX_PATH, "%s\\Fonts", szBuff);
}

CZText::~CZText()
{
	FACEMAP::iterator iter;
	for (iter = m_mapFaceID.begin(); iter != m_mapFaceID.end(); iter++)
	{
		delete iter->second;
		iter->second = NULL;
	}
	//释放Cache和Library
	FTC_Manager_Done(m_pCacheManager);
	FT_Done_FreeType(m_pLibrary);
}

FTC_FaceID CZText::GetFaceID(char *pFamily, byte byStyle)
{
	//由字体和风格共同组成关键字
	string szKey = pFamily;
	szKey += byStyle;

	FACEMAP::iterator iter = m_mapFaceID.find(szKey);
	if (iter != m_mapFaceID.end())
		return iter->second;

	char szFilePath[_MAX_PATH];
	sprintf_s(szFilePath, _MAX_PATH, "%s\\%s", m_szFontsDir, pFamily);

	MyFaceRec *pFaceID = new MyFaceRec();
	if (pFaceID == NULL)
		return NULL;

	//深度复制文件名
	pFaceID->file_path = new char[strlen(szFilePath) + 1];
	if (pFaceID->file_path == NULL)
		return NULL;
	memcpy((void *)pFaceID->file_path, szFilePath, strlen(szFilePath) + 1);

	pFaceID->style = byStyle;
	pFaceID->face_index = 0;
	
	m_mapFaceID[szKey] = pFaceID;
	return m_mapFaceID[szKey];
}
