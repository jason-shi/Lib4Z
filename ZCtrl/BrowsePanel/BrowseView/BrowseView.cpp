#include "StdAfx.h"
#include "BrowseView.h"

CBrowseView::CBrowseView(CImage *pImage, CPoint xDrawPos, CPoint xSrcPos, CSize xSize, int nDepth)
				: CPanel(pImage, xDrawPos, xSrcPos, xSize, nDepth)
{
	m_pBrowseData = NULL;
	m_bAutoScroll = false;
}

CBrowseView::~CBrowseView()
{
}

static void _OnFlushData(void *pObject)
{
	CBrowseView *pBrowseView = (CBrowseView *)pObject;
	if (!pBrowseView)
		return;

	pBrowseView->OnUpdateView(pBrowseView);
}

void CBrowseView::BindBrowseData(CBrowseData *pBrowseData)
{
	m_pBrowseData = pBrowseData;

	//反向指定BrowseView和视图更新回调
	m_pBrowseData->SetBrowseView(this);
	m_pBrowseData->SetFN_FlushData(_OnFlushData);
}
