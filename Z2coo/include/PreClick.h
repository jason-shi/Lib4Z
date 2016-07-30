#ifndef __PreClick_h__
#define __PreClick_h__

struct CLDblClick {
	int			 m_nRelFlags;
	LPARAM		 m_pRelPoint;
	int			 m_nButtonUp; //0:û�� 1:�󵥻� 2:��˫��
};

struct CRDblClick {
	int			 m_nRelFlags;
	LPARAM		 m_pRelPoint;
	int			 m_nButtonUp; //0:û�� 1:�ҵ��� 2:��˫��
};

extern CLDblClick	 g_tLDblClick;
extern CRDblClick	 g_tRDblClick;

extern void			 BlockLDblClick();
extern void			 BlockRDblClick();
extern void			 BlockKeepLDown(CObject *pActived, UINT nFlags, CPoint point);
extern void			 BlockKeepRDown(CObject *pActived, UINT nFlags, CPoint point);

#endif
