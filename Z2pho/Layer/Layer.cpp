#include "StdAfx.h"
#include "Layer.h"

CLayer::CLayer(CImage *pImage)
{
	m_pImage = pImage;
	m_szName = "";
	m_xDrawPos.x = 0;
	m_xDrawPos.y = 0;
	m_nOpacity = 255;
	m_bLock = false;
	m_bVisible = true;
	m_pNext = NULL;
}

CLayer::CLayer(CImage *pImage, std::string szName)
{
	m_pImage = pImage;
	m_szName = szName;
	m_xDrawPos.x = 0;
	m_xDrawPos.y = 0;
	m_nOpacity = 255;
	m_bLock = false;
	m_bVisible = true;
	m_pNext = NULL;
}

CLayer::CLayer(CImage *pImage, std::string szName, CPoint xDrawPos)
{
	m_pImage = pImage;
	m_szName = szName;
	m_xDrawPos = xDrawPos;
	m_nOpacity = 255;
	m_bLock = false;
	m_bVisible = true;
	m_pNext = NULL;
}

CLayer::CLayer(CImage *pImage, std::string szName, CPoint xDrawPos, int nOpacity)
{
	m_pImage = pImage;
	m_szName = szName;
	m_xDrawPos = xDrawPos;
	m_nOpacity = nOpacity;
	m_bLock = false;
	m_bVisible = true;
	m_pNext = NULL;
}

CLayer::~CLayer()
{
	m_pImage = NULL;
	m_pNext = NULL;
}
