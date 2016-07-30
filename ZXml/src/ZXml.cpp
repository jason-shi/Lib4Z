#include "StdAfx.h"
#include "ZXml.h"

using namespace std;

CZXml::CZXml()
{
	m_pXmlForest = new XmlForest(0);
	m_pXmlForest->m_szKey = "ZXml";
}

CZXml::~CZXml()
{
	delete m_pXmlForest;
	m_pXmlForest = NULL;
}

void CZXml::_InitialMember()
{
	m_vecXmlNode.clear();

	m_byState = LS_BEGIN;
	m_byStatus = LS_BEGIN;

	m_bNegative = false;
	m_nNumber = 0;
	m_szString.clear();

	m_vecNumber.clear();
	m_vecString.clear();
	m_vecState.clear();
	m_vecParen.clear();
}

XmlNode *XMLVisitBaby(vector<XmlNode *> &vecVisit, XmlNode *pNode)
{
	//进入子结点
	if (pNode->m_pBaby)
	{
		pNode = pNode->m_pBaby;
		return pNode;
	}
	while (pNode)
	{
		//进入兄弟结点
		if (pNode->m_pNext)
		{
			pNode = pNode->m_pNext;
			vecVisit.pop_back();
			return pNode;
		}
		//回到父结点
		vecVisit.pop_back();
		if (vecVisit.empty())
			return NULL;
		pNode = vecVisit.back();
	}
	return NULL;
}

XmlNode *XMLVisitNext(vector<XmlNode *> &vecVisit, XmlNode *pNode)
{
	while (pNode)
	{
		//进入兄弟结点
		if (pNode->m_pNext)
		{
			pNode = pNode->m_pNext;
			vecVisit.pop_back();
			return pNode;
		}
		//回到父结点
		vecVisit.pop_back();
		if (vecVisit.empty())
			return NULL;
		pNode = vecVisit.back();
	}
	return NULL;
}

bool CZXml::LoadTree(vector<XmlNode *> &vecResult, vector<string>& vecKey)
{
	if (vecKey.empty())
		return false;

	//按查找条件全匹配
	XmlNode *pNode = m_pXmlForest->m_pBaby;

	vector<string>::iterator iter;
	for (iter = vecKey.begin(); iter != vecKey.end(); iter++)
	{
		XmlNode *pSearch = new XmlNode(LN_ROOT);
		if (Read((byte *)iter->c_str(), pSearch) != 0)
		{
			delete pSearch;
			pSearch = NULL;

			//查找树构建失败
			return false;
		}

		while (pNode)
		{
			XmlNode *pSearchNode = pSearch->m_pBaby;
			//匹配元素关键字
			if (pSearchNode)
			{
				if (pNode->m_szKey != pSearchNode->m_szKey)
					return false;
			}
			//匹配元素属性
			XmlNode *pAttr = pNode->m_pAttr;
			XmlNode *pSearchAttr = pSearchNode->m_pAttr;
			while (pAttr && pSearchAttr)
			{
				if (pAttr->m_szKey == pSearchAttr->m_szKey && pAttr->m_byType == pSearchAttr->m_byType)
				{
					if (pAttr->m_byType == LN_NUMBER && pAttr->m_nNumber == pSearchAttr->m_nNumber
						|| pAttr->m_byType == LN_STRING && pAttr->m_szString == pSearchAttr->m_szString)
					{
						pSearchAttr = pSearchAttr->m_pAttr;
						pAttr = pNode->m_pAttr; //从头开始
						continue;
					}
				}
				pAttr = pAttr->m_pAttr;
			}
			if (!pAttr && pSearchAttr)
			{
				//元素不匹配
				pNode = pNode->m_pNext;
				continue;
			}
			if (iter == vecKey.end() - 1)
			{
				//完全匹配
				vecResult.push_back(pNode);
				pNode = pNode->m_pNext;
				continue;
			}
			break;
		}
		//完成搜索条件的匹配
		if (pNode)
			pNode = pNode->m_pBaby;

		delete pSearch;
		pSearch = NULL;
	}
	return true;
}

bool CZXml::PushXmlData()
{
	if (!m_pXmlNode->IsDataType())
		return false;

	//添加到父结点
	XmlNode *pParent = m_vecXmlNode.back();
	pParent->PushBaby(m_pXmlNode);

	//回到父结点
	m_pXmlNode = pParent;
	m_vecXmlNode.pop_back();
	return true;
}

int CZXml::SaveTree(byte *pBuff, std::string& szRootKey)
{
	//创建一棵树
	XmlNode *pXmlRoot = new XmlNode(LN_ROOT);
	if (!pXmlRoot)
		return -1;
	pXmlRoot->m_szKey = szRootKey;

	if (Read(pBuff, pXmlRoot) != 0)
	{
		delete pXmlRoot;
		pXmlRoot = NULL;
	}

	//添加到森林
	m_pXmlForest->PushBaby(pXmlRoot);
	return 0;
}

int CZXml::Read(byte *pBuff, XmlNode *pXmlNode)
{
	//成员变量初始化
	_InitialMember();

	m_pXmlNode = pXmlNode;
	int nLength = strlen((char *)pBuff);

	//增加保护结点
	m_vecParen.push_back(LS_BEGIN);

	int nOffset = 0;
	char ch;
	while ((ch = *pBuff) != '\0')
	{
		if (isspace(ch) || ch == '\t' || ch == '\n' || ch == '\r')
		{
			m_byStatus = LS_BEGIN;
			XML_ADVANCE(pBuff, nLength, 1);
		}
		else if (isalpha(ch))
		{
			nOffset = ReadString(m_szString, pBuff);
			XML_ADVANCE(pBuff, nLength, nOffset);
		}
		else if (isdigit(ch))
		{
			nOffset = ReadNumber(m_nNumber, pBuff);
			XML_ADVANCE(pBuff, nLength, nOffset);
		}
		else if (ch == '-')
		{
			m_bNegative = true;
			XML_ADVANCE(pBuff, nLength, 1);
		}
		else if (ch == '"')
		{
			m_byStatus = LS_QUOTATION;

			nOffset = ReadString(m_szString, pBuff);
			XML_ADVANCE(pBuff, nLength, nOffset);
		}
		else if (ch == '{')
		{
			ProcessLBrace(pBuff, nLength);
			XML_ADVANCE(pBuff, nLength, 1);
		}
		else if (ch == '}')
		{
			ProcessRBrace(pBuff, nLength);
			XML_ADVANCE(pBuff, nLength, 1);
		}
		else if (ch == '[')
		{
			ProcessLSquare(pBuff, nLength);
			XML_ADVANCE(pBuff, nLength, 1);
		}
		else if (ch == ']')
		{
			ProcessRSquare(pBuff, nLength);
			XML_ADVANCE(pBuff, nLength, 1);
		}
		else if (ch == '(')
		{
			ProcessLBracket(pBuff, nLength);
			XML_ADVANCE(pBuff, nLength, 1);
		}
		else if (ch == ')')
		{
			ProcessRBracket(pBuff, nLength);
			XML_ADVANCE(pBuff, nLength, 1);
		}
		else if (ch == '<')
		{
			ProcessLAngle(pBuff, nLength);
			XML_ADVANCE(pBuff, nLength, 1);
		}
		else if (ch == '>')
		{
			ProcessRAngle(pBuff, nLength);
			XML_ADVANCE(pBuff, nLength, 1);
		}
		else if (ch == '=')
		{
			m_vecState.push_back(m_byState);
			m_byState = LS_DEFINE;

			m_byStatus = LS_BEGIN;
			XML_ADVANCE(pBuff, nLength, 1);
		}
		else if (ch == ',')
		{
			m_byState = m_vecState.back();
			m_vecState.pop_back();

			m_byStatus = LS_BEGIN;
			XML_ADVANCE(pBuff, nLength, 1);
		}
		else if (ch == '/')
		{
			m_vecState.push_back(m_byState);
			m_byState = LS_SLASH;

			m_byStatus = LS_BEGIN;
			XML_ADVANCE(pBuff, nLength, 1);
		}
		else if (ch == '\n')
		{
			XML_ADVANCE(pBuff, nLength, 1);
			break;
		}
	}
	return FinallyProcess();
}

int CZXml::FinallyProcess()
{
	//对剩余的状态进行结算
	m_vecState.push_back(m_byState);
	while (m_vecState.size() != 1)
	{
		m_byState = m_vecState.back();
		m_vecState.pop_back();

		m_vecXmlNode.back()->PushBaby(m_pXmlNode);

		m_pXmlNode = m_vecXmlNode.back();
		m_vecXmlNode.pop_back();
	}

	return 0;
}
