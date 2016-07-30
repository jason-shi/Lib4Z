#ifndef __LineTag_h__
#define __LineTag_h__

class CLineTag {
  public:
	CLineTag();
	CLineTag(const CLineTag& tLineTag);
	virtual ~CLineTag();

  public:
	void			 Initial();
	void			 UpdateFaceID();

  public:
	CLineTag&		 operator=(const CLineTag& tLineTag);

  public:
	char			 szFaceName[LF_FACESIZE * 2];
	byte			 byStyle;
	FTC_ScalerRec	 tScalerRec;

	int				 nTagPos;
	int				 nTagSkip;

	bool			 transparent;
	COLORREF		 color;
	COLORREF		 bkcolor;
};

#endif
