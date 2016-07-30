#ifndef __PreClick_h__
#define __PreClick_h__

struct CLDblClick {
	int			 m_nRelFlags;
	LPARAM		 m_pRelPoint;
	int			 m_nButtonUp; //0:没有 1:左单击 2:左双击
};

struct CRDblClick {
	int			 m_nRelFlags;
	LPARAM		 m_pRelPoint;
	int			 m_nButtonUp; //0:没有 1:右单击 2:右双击
};

extern CLDblClick	 g_tLDblClick;
extern CRDblClick	 g_tRDblClick;

extern void			 BlockLDblClick();
extern void			 BlockRDblClick();
extern void			 BlockKeepLDown(CObject *pActived, UINT nFlags, CPoint point);
extern void			 BlockKeepRDown(CObject *pActived, UINT nFlags, CPoint point);

#endif
