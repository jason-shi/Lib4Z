#include "StdAfx.h"
#include "BrowseRuler.h"

CBrowseRuler::CBrowseRuler(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize, int nDepth)
				: CPanel(pImage, xDrawPos, xSrcPos, xSize, nDepth)
{
}

CBrowseRuler::~CBrowseRuler()
{
}
