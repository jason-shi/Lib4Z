#ifndef __BrowseRuler_h__
#define __BrowseRuler_h__

class CBrowseRuler : public CPanel {
  public:
	CBrowseRuler(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize, int nDepth);
	virtual ~CBrowseRuler();
};

#endif
