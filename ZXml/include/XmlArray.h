#ifndef __XmlArray_h__
#define __XmlArray_h__

#include "XmlNode.h"

class XmlArray : public XmlNode {
  public:
	XmlArray(byte byType);
	virtual ~XmlArray();

  public:
	std::vector<int>		 m_vecSize;
};

#endif
