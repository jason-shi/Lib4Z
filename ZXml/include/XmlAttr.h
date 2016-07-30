#ifndef __XmlAttr_h__
#define __XmlAttr_h__

#include "XmlNode.h"

class XmlAttr : public XmlNode {
  public:
	XmlAttr(byte byType);
	virtual ~XmlAttr();

  public:
	std::vector<XmlNode *>	 m_vecNode;
};

#endif
