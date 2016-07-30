#ifndef __Xml_h__
#define __Xml_h__

#include "XmlNode.h"
#include "XmlArray.h"
#include "XmlAttr.h"
#include "XmlForest.h"

#define LS_BEGIN				1
#define LS_ELEMENT				2
#define LS_ATTR					3
#define LS_VALUE				4
#define LS_DEFINE				5
#define LS_SLASH				6
#define LS_STRING				7
#define LS_NUMBER				8
#define LS_NAMESTR				9
#define LS_BRACE				10
#define LS_SQUARE				11
#define LS_BRACKET				12
#define LS_ANGLE				13
#define LS_QUOTATION			14
#define LS_ARRAY				15

#ifndef  SINGLETON
#define  SINGLETON(type) \
	static inline type* GetInstancePtr() \
	{ \
		static type _instance; \
		return &_instance; \
	}
#endif

#define XML_ADVANCE(pBuff, nLength, nOffset) \
{ \
	if (nOffset < 0) \
		return -1; \
	pBuff += nOffset; \
	nLength -= nOffset; \
}

#define HEX_2_DEC(hex) \
		((hex) >= '0' && (hex) <= '9' ? (hex) - '0' : \
		(hex) >= 'A' && (hex) <= 'F' ? (hex) - 'A' + 10 : (hex) - 'a' + 10)

#define DEC_2_HEX(dec) \
		(dec) < 10 ? (dec) + '0' : (dec) - 10 + 'A'

//XML结点访问控制
#define XML_VISIT_BABY(vecVisit, pNode) \
	{ \
		pNode = XMLVisitBaby(vecVisit, pNode); \
		continue; \
	}

#define XML_VISIT_NEXT(vecVisit, pNode) \
	{ \
		pNode = XMLVisitNext(vecVisit, pNode); \
		continue; \
	}

XmlNode				*XMLVisitBaby(std::vector<XmlNode *> &vecVisit, XmlNode *pNode);
XmlNode				*XMLVisitNext(std::vector<XmlNode *> &vecVisit, XmlNode *pNode);


class XmlForest;
class XmlNode;
class CZXml {
  public:
	SINGLETON(CZXml);
	virtual ~CZXml();

  private:
	CZXml();
	void			 _InitialMember();

  public:
	int				 SaveTree(byte *pBuff, std::string& szRootKey); //解析成Json树，并存入森林
	bool			 LoadTree(std::vector<XmlNode *> &vecResult, std::vector<std::string>& vecKey); //从森林加载

  public:
	int				 Read(byte *pBuff, XmlNode *pXmlNode); //挂载到指定的结点下
	int				 Write(byte *pBuff, XmlNode *pXmlNode); //将Json树写成XML

  protected:
	int				 ReadString(std::string &szString, byte *pBuff);
	int				 ReadNumber(int &nNumber, byte *pBuff);

  protected:
	int				 ReadBString(std::string &szString, byte *pBuff);
	int				 ReadXString(std::string &szString, byte *pBuff);
	int				 ReadQString(std::string &szString, byte *pBuff);

  protected:
	int				 ReadBitValue(int &nNumber, byte *pBuff);
	int				 ReadHexValue(int &nNumber, byte *pBuff);
	int				 ReadOctValue(int &nNumber, byte *pBuff);
	int				 ReadDecValue(int &nNumber, byte *pBuff);

  protected:
	int				 ProcessLBrace(byte *pBuff ,int nLength);
	int				 ProcessLSquare(byte *pBuff ,int nLength);
	int				 ProcessLBracket(byte *pBuff ,int nLength);
	int				 ProcessLAngle(byte *pBuff ,int nLength);
	int				 ProcessRBrace(byte *pBuff ,int nLength);
	int				 ProcessRSquare(byte *pBuff ,int nLength);
	int				 ProcessRBracket(byte *pBuff ,int nLength);
	int				 ProcessRAngle(byte *pBuff ,int nLength);

  protected:
	int				 SettleUpBrace();
	int				 SettleUpSquare();
	int				 SettleUpBracket();
	int				 SettleUpAngle();

  protected:
	int				 ProcessAlpha(byte *pBuff ,int nLength);
	int				 ProcessDigit(byte *pBuff ,int nLength);

  protected:
	bool			 PushXmlData();
	int				 FinallyProcess();

  protected:
	void			 SwitchXmlNode(XmlNode *pXmlNode);

  protected:
	XmlNode					*m_pXmlNode;
	std::vector<XmlNode *>	 m_vecXmlNode;

  protected:
	byte			 m_byState;
	byte			 m_byStatus;

  protected:
	bool			 m_bNegative;

  protected:
	int				 m_nNumber;
	std::string		 m_szString;

  protected:
	std::vector<int>		 m_vecNumber;
	std::vector<std::string> m_vecString;
	std::vector<byte>		 m_vecState;
	std::vector<byte>		 m_vecParen;

  protected:
	byte			*m_pBuff;
	int				 m_nLength;

  protected:
	XmlForest		*m_pXmlForest;
	int				 m_nError;
};

inline void CZXml::SwitchXmlNode(XmlNode *pXmlNode)
{
	m_pXmlNode->PushBaby(pXmlNode);

	m_vecXmlNode.push_back(m_pXmlNode);
	m_pXmlNode = pXmlNode;
}

#define g_pZXml			CZXml::GetInstancePtr()

#endif
