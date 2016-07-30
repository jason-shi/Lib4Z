#ifndef	__Channel_h__
#define	__Channel_h__

#define CHL_BLUE			0x0001
#define CHL_GREEN			0x0002
#define CHL_RED				0x0004
#define CHL_ALPHA			0x0008
#define CHL_HUE				0x0010
#define CHL_SATUR			0x0020
#define CHL_BRIGHT			0x0040

class CChannel : protected CImage {
  public:
	CChannel(int nChlType);
	virtual ~CChannel();

  public:
	void			 GetChannel(const CImage *pImage); //获得图象的通道值

  protected:
	void			 BGRA_BChan(const CImage *pImage); //BGRA单通道
	void			 BGRA_GChan(const CImage *pImage);
	void			 BGRA_RChan(const CImage *pImage);
	void			 BGRA_AChan(const CImage *pImage);
	void			 BGRA_BGChan(const CImage *pImage); //BGRA双通道
	void			 BGRA_BRChan(const CImage *pImage);
	void			 BGRA_GRChan(const CImage *pImage);
	void			 BGRA_BGRChan(const CImage *pImage); //BGRA三通道

  protected:
	void			 HSB_HChan(const CImage *pImage); //HSB单通道
	void			 HSB_SChan(const CImage *pImage);
	void			 HSB_BChan(const CImage *pImage);
	void			 HSB_HSChan(const CImage *pImage); //HSB双通道
	void			 HSB_HBChan(const CImage *pImage);
	void			 HSB_SBChan(const CImage *pImage);
	void			 HSB_HSBChan(const CImage *pImage); //HSB三通道

  protected:
	int				 m_nChlType;
};

inline CChannel::CChannel(int nChlType)
{
	m_nChlType = nChlType;
}

inline CChannel::~CChannel()
{
}

#endif
