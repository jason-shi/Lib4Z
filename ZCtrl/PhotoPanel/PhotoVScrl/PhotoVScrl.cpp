#include "StdAfx.h"
#include "PhotoVScrl.h"

CPhotoVScrl::CPhotoVScrl(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize)
				: CVScrollBar(pImage, xDrawPos, xSrcPos, xSize)
{
}

CPhotoVScrl::~CPhotoVScrl()
{
}
