#ifndef __Canvas_h__
#define __Canvas_h__

#define CNV_ALIGN_CENTER		1
#define CNV_ALIGN_TOP			2
#define CNV_ALIGN_BOTTOM		3
#define CNV_ALIGN_LEFT			4
#define CNV_ALIGN_RIGHT			5
#define CNV_ALIGN_TOPLEFT		6
#define CNV_ALIGN_TOPRIGHT		7
#define CNV_ALIGN_BOTTOMLEFT	8
#define CNV_ALIGN_BOTTOMRIGHT	9


class CCanvas : public CSprite {
  public:
	CCanvas(CImage *pImage, CPoint xDrawPos, CSize xSize, int nDepth);
	virtual ~CCanvas();

  public:
	void			 Set(CImage *pImage, CPoint xDrawPos, CSize xSize, int nDepth);
	void			 ClearImage();
	void			 ResetSize(int nAlignMode, CSize xSize);
	void			 ResetSize(int nAlignMode, int nWidth, int nHeight);

  protected:
	bool			 AllocImage(int nWidth, int nHeight);
};

inline void CCanvas::ClearImage()
{
	m_pImage->Clear();
	g_pZ2coo->Redraw(this);
}

#endif
