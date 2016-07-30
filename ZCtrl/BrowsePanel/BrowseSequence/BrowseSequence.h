#ifndef __BrowseSeq_h__
#define __BrowseSeq_h__

class CBrowseSequence : public CPanel {
  public:
	CBrowseSequence(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize, int nDepth);
	virtual ~CBrowseSequence();
};

#endif
