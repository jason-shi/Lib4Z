#ifndef __Layer_h__
#define __Layer_h__

#define LYR_IMAGE_SMALL			1
#define LYR_IMAGE_MIDDLE		2
#define LYR_IMAGE_BIG			3
#define LYR_IMAGE_LARGE			4

class CLayer {
  public:
	CLayer(CImage *pImage);
	CLayer(CImage *pImage, std::string szName);
	CLayer(CImage *pImage, std::string szName, CPoint xDrawPos);
	CLayer(CImage *pImage, std::string szName, CPoint xDrawPos, int nOpacity);
	virtual ~CLayer();

  public:
	void			 SetImage(CImage *pImage);
	CImage			*GetImage() const;
	void			 SetName(const std::string& szName);
	std::string		 GetName() const;
	void			 SetDrawPos(CPoint xDrawPos);
	CPoint			 GetDrawPos() const;
	void			 SetVisible(bool bVisible);
	bool			 GetVisible() const;
	void			 SetLock(bool bLock);
	bool			 GetLock() const;

  protected:
	CImage			*m_pImage;
	std::string		 m_szName;
	CPoint			 m_xDrawPos;
	int				 m_nOpacity;
	bool			 m_bVisible;
	bool			 m_bLock;

  public:
	CLayer			*m_pNext;
	ImageBlendDelegate		 ImageBlend;
};

inline void CLayer::SetImage(CImage *pImage)
{
	m_pImage = pImage;
}

inline CImage *CLayer::GetImage() const
{
	return m_pImage;
}

inline void CLayer::SetName(const std::string& szName)
{
	m_szName = szName;
}

inline std::string CLayer::GetName() const
{
	return m_szName;
}

inline void CLayer::SetDrawPos(CPoint xDrawPos)
{
	m_xDrawPos = xDrawPos;
}

inline CPoint CLayer::GetDrawPos() const
{
	return m_xDrawPos;
}

inline void CLayer::SetVisible(bool bVisible)
{
	m_bVisible = bVisible;
}

inline bool CLayer::GetVisible() const
{
	return m_bVisible;
}

inline void CLayer::SetLock(bool bLock)
{
	m_bLock = bLock;
}

inline bool CLayer::GetLock() const
{
	return m_bLock;
}

#endif
