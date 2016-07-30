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
	void			 GetChannel(const CImage *pImage); //���ͼ���ͨ��ֵ

  protected:
	void			 BGRA_BChan(const CImage *pImage); //BGRA��ͨ��
	void			 BGRA_GChan(const CImage *pImage);
	void			 BGRA_RChan(const CImage *pImage);
	void			 BGRA_AChan(const CImage *pImage);
	void			 BGRA_BGChan(const CImage *pImage); //BGRA˫ͨ��
	void			 BGRA_BRChan(const CImage *pImage);
	void			 BGRA_GRChan(const CImage *pImage);
	void			 BGRA_BGRChan(const CImage *pImage); //BGRA��ͨ��

  protected:
	void			 HSB_HChan(const CImage *pImage); //HSB��ͨ��
	void			 HSB_SChan(const CImage *pImage);
	void			 HSB_BChan(const CImage *pImage);
	void			 HSB_HSChan(const CImage *pImage); //HSB˫ͨ��
	void			 HSB_HBChan(const CImage *pImage);
	void			 HSB_SBChan(const CImage *pImage);
	void			 HSB_HSBChan(const CImage *pImage); //HSB��ͨ��

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
