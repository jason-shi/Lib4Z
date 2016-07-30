#include "StdAfx.h"
#include "BrowseSequence.h"

CBrowseSequence::CBrowseSequence(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize, int nDepth)
				: CPanel(pImage, xDrawPos, xSrcPos, xSize, nDepth)
{
}

CBrowseSequence::~CBrowseSequence()
{
}
