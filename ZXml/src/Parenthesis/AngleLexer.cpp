#include "StdAfx.h"
#include "ZXml.h"

using namespace std;

int CZXml::ProcessLAngle(byte *pBuff ,int nLength)
{
	m_vecParen.push_back(LS_ANGLE);

	m_vecState.push_back(m_byState);
	m_byState = LS_ANGLE;

	m_byStatus = LS_BEGIN;
	return 1;
}

int CZXml::ProcessRAngle(byte *pBuff ,int nLength)
{
	//括号不匹配
	if (m_vecParen.back() != LS_ANGLE)
		return -1;
	m_vecParen.pop_back();

	m_nError = SettleUpAngle();
		;;;;if (m_nError != 0) return -1;

	m_byState = m_vecState.back();
	m_vecState.pop_back();

	m_byStatus = LS_BEGIN;
	return 1;
}

int CZXml::SettleUpAngle()
{
	vector<byte> vecState;
	vector<int> vecNumber;
	vector<string> vecString;

	//将<>里面的内容导出
	while (m_byState != LS_ANGLE)
	{
		if (m_byState == LS_NUMBER)
		{
			vecNumber.push_back(m_vecNumber.back());
			m_vecNumber.pop_back();
		}
		if (m_byState == LS_STRING || m_byState == LS_NAMESTR)
		{
			vecString.push_back(m_vecString.back());
			m_vecString.pop_back();
		}
		vecState.push_back(m_byState);

		m_byState = m_vecState.back();
		m_vecState.pop_back();
	}

	if (vecState.back() == LS_SLASH)
	{
		//例</Library>
		vecState.pop_back();
			;;;;if (vecState.size() != 1) return -1;
		m_byState = vecState.back();
		vecState.pop_back();
			;;;;if (m_byState != LS_NAMESTR) return -1;
			;;;;if (m_pXmlNode->m_szKey != vecString.back()) return -1;

		m_pXmlNode = m_vecXmlNode.back();
		m_vecXmlNode.pop_back();
	}
	else if (vecState.front() == LS_SLASH)
	{
		//例<Library ID = "L_8"/>
		XmlNode *pXmlElem = new XmlNode(LN_ELEMENT);
			;;;;if (!pXmlElem) return -1;

		//第一个纯字符串是关键字或元素
		m_byState = vecState.back();
		vecState.pop_back();
		if (m_byState == LS_NAMESTR)
		{
			pXmlElem->m_szKey = vecString.back();
			vecString.pop_back();
		}

		//处理循环的键值对
		while (!vecState.empty() && vecState.back() == LS_NAMESTR)
		{
			vecState.pop_back();
			XmlAttr *pXmlAttr = new XmlAttr(LN_UNKNOWN);
				;;;;if (!pXmlAttr) return -1;

			//键
			pXmlAttr->m_szKey = vecString.back();
			vecString.pop_back();

			//=
				;;;;if (vecState.back() != LS_DEFINE) return -1;
			vecState.pop_back();

			//值
			m_byState = vecState.back();
			vecState.pop_back();
				;;;;if (m_byState != LS_STRING && m_byState != LS_NUMBER) return -1;

			if (m_byState == LS_STRING)
			{
				pXmlAttr->m_byType = LN_STRING;
				pXmlAttr->m_szString = vecString.back();
				vecString.pop_back();
			}
			if (m_byState == LS_NUMBER)
			{
				pXmlAttr->m_byType = LN_NUMBER;
				pXmlAttr->m_nNumber = vecNumber.back();
				vecNumber.pop_back();
			}

			//键值对添加到元素
			pXmlElem->PushAttr(pXmlAttr);
		}

			;;;;if (vecState.size() != 1) return -1;
		m_pXmlNode->PushBaby(pXmlElem);
	}
	else
	{
		//例<Library ID = "L_8">
		XmlNode *pXmlElem = new XmlNode(LN_ELEMENT);
			;;;;if (!pXmlElem) return -1;

		//第一个纯字符串是关键字或元素
		m_byState = vecState.back();
		vecState.pop_back();
		if (m_byState == LS_NAMESTR)
		{
			pXmlElem->m_szKey = vecString.back();
			vecString.pop_back();
		}

		//处理循环的键值对
		while (!vecState.empty() && vecState.back() == LS_NAMESTR)
		{
			vecState.pop_back();
			XmlAttr *pXmlAttr = new XmlAttr(LN_UNKNOWN);
				;;;;if (!pXmlAttr) return -1;

			//键
			pXmlAttr->m_szKey = vecString.back();
			vecString.pop_back();

			//=
				;;;;if (vecState.back() != LS_DEFINE) return -1;
			vecState.pop_back();

			//值
			m_byState = vecState.back();
			vecState.pop_back();
				;;;;if (m_byState != LS_STRING && m_byState != LS_NUMBER) return -1;

			if (m_byState == LS_STRING)
			{
				pXmlAttr->m_byType = LN_STRING;
				pXmlAttr->m_szString = vecString.back();
				vecString.pop_back();
			}
			if (m_byState == LS_NUMBER)
			{
				pXmlAttr->m_byType = LN_NUMBER;
				pXmlAttr->m_nNumber = vecNumber.back();
				vecNumber.pop_back();
			}

			//键值对添加到元素
			pXmlElem->PushAttr(pXmlAttr);
		}

			;;;;if (!vecState.empty()) return -1;
		SwitchXmlNode(pXmlElem);
	}

	return 0;
}
