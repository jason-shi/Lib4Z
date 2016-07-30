#include "StdAfx.h"
#include "LineTag.h"

CLineTag::CLineTag()
{
}

CLineTag::~CLineTag()
{
}

CLineTag::CLineTag(const CLineTag& tLineTag)
{
	*this = tLineTag;
}

void CLineTag::Initial()
{
	//设置指定的FontStyle
	LOGFONT logFont;
	GetObject(g_pZText->GetDefaultFont(), sizeof(logFont), &logFont);

	strcpy(szFaceName, "SimSun.ttc");
	byStyle = TS_NORMAL;

	tScalerRec.face_id = g_pZText->GetFaceID(szFaceName, byStyle);
	tScalerRec.width = logFont.lfWidth;
	tScalerRec.height = logFont.lfHeight;
	tScalerRec.pixel = 1;

	nTagPos = 0;
	nTagSkip = 0;

	transparent = true;
	color = BGRA(0, 0, 0, 255);
	bkcolor = BGRA(255, 255, 255, 255);
}

void CLineTag::UpdateFaceID()
{
	tScalerRec.face_id = g_pZText->GetFaceID(szFaceName, byStyle);
}

CLineTag& CLineTag::operator=(const CLineTag& tLineTag)
{
	strcpy(szFaceName, tLineTag.szFaceName);
	memcpy(&tScalerRec, &tLineTag.tScalerRec, sizeof(FTC_ScalerRec));

	byStyle     = tLineTag.byStyle;
	nTagPos     = tLineTag.nTagPos;
	nTagSkip    = tLineTag.nTagSkip;

	transparent = tLineTag.transparent;
	color       = tLineTag.color;
	bkcolor     = tLineTag.bkcolor;

	return *this;
}
