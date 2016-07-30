#ifndef __BezierCurve_h__
#define __BezierCurve_h__

class CBezierCurve {
  public:
	CBezierCurve(int nDegree, int nCount, LPPOINT lpPoints, bool *lpFlags, COLORREF *lpColors, bool bClose = false);
	~CBezierCurve();

  public:
	void			 Draw(CCanvas *pCanvas);

  private:
	void			 _Draw2D(CCanvas *pCanvas);
	void			 _Draw3D(CCanvas *pCanvas);
	void			 _DrawND(CCanvas *pCanvas);
	void			 _UpdateCQ(int i);

  private:
	int				 m_nDegree;
	int				 m_nCount;
	LPPOINT			 m_lpPoints;
	bool			*m_lpFlags;
	COLORREF		*m_lpColors;
	bool			 m_bClose;

  private:
	CPoint			 split_points[4];
	CPoint			 tCenter;
	CPoint			 tQuarter;
};

#endif
