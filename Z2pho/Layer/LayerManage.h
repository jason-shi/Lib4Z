#ifndef __LayerManage_h__
#define __LayerManage_h__

#include "Layer.h"

class CLayerManage {
  public:
	CLayerManage();
	~CLayerManage();
	SINGLETON(CLayerManage);

  public:
	void			 InsertLayer(CLayer *pCursor, CLayer *pLayer);
	void			 DeleteLayer(CLayer *pLayer);

  public:
	void			 BlendLayer(CCanvas *pCanvas, CLayer *pLayer);
	void			 BlendAllLayer(CCanvas *pCanvas);

  protected:
	CLayer			*m_pHead;
};

#define g_pLayerManage		CLayerManage::GetInstancePtr()

#endif
