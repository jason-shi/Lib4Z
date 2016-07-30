#ifndef __XmlNode_h__
#define __XmlNode_h__

#define LN_UNKNOWN				0
#define LN_ROOT					1
#define LN_ARRAY				2
#define LN_ELEMENT				3
#define LN_NUMBER				4
#define LN_STRING				5

class XmlNode {
  public:
	XmlNode(byte byType);
	virtual ~XmlNode();

  public:
	XmlNode			*FindBaby(const std::string& szKey);
	void			 PushBaby(XmlNode *pBaby);
	XmlNode			*FindAttr(const std::string& szKey);
	void			 PushAttr(XmlNode *pBaby);

  public:
	bool			 IsDataType();

  public:
	std::string		 m_szKey;
	byte			 m_byType;

  public:
	int				 m_nNumber;
	std::string		 m_szString;

  public:
	XmlNode			*m_pNext;
	XmlNode			*m_pBaby;
	XmlNode			*m_pAttr;
};

inline bool XmlNode::IsDataType()
{
	return m_byType == LN_ELEMENT || m_byType == LN_STRING || m_byType == LN_NUMBER;
}

#endif
