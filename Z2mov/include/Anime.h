#ifndef __Anime_h__
#define __Anime_h__

#include <boost/function.hpp>

//动画播放完毕回调
class CAnime;
typedef boost::function<void(CAnime *)>	 FN_AnimeEnd;

class CObject;
class CAnime {
	friend class		 CZ2mov; //访问更新函数和后向指针

  public:
	CAnime();
	CAnime(int nFrame, int nPeriod, FN_AnimeEnd AnimeEnd = NULL, int *pParam = NULL);
	virtual ~CAnime();

  public:
	void			 SetFrame(int nFrame);
	void			 SetPeriod(int nPeriod);
	void			 Attach(CObject* pObject);
	CObject			*GetAttach() const;
	void			 SetRemove(bool bRemove = true);
	bool			 GetRemove() const;
	void			 SetEndProc(FN_AnimeEnd AnimeEnd);
	FN_AnimeEnd		 GetEndProc() const;
	void			 SetEndParam(int *pParam);
	int				*GetEndParam() const;

  protected:
	bool			 AllocImage(int nWidth, int nHeight);

  public:
	void			 Play();
	void			 Stop();
	void			 Pause();
	void			 Resume();
	bool			 IsActive() const;
	bool			 IsPause() const;

  protected:
	virtual void	 SetUp();
	virtual void	 Update(int nDelta);

  protected:
	int				 m_nFrame; //帧数
	int				 m_nStep; //帧号
	int				 m_nPeriod; //更新周期(ms)
	int				 m_nElapse; //流逝时间
	CObject			*m_pAttach;
	bool			 m_bRemove;
	CImage			*m_pImage;

  protected:
	FN_AnimeEnd		 OnAnimeEnd;
	int				*m_pParam;

  private:
	bool			 m_bActive;
	bool			 m_bPause;

  private:
	CAnime			*m_pNext;
};

inline void CAnime::SetFrame(int nFrame)
{
	if (m_bActive)
		return;
	m_nFrame = nFrame;
}

inline void CAnime::SetPeriod(int nPeriod)
{
	if (m_bActive)
		return;
	m_nPeriod = nPeriod;
}

inline void CAnime::Attach(CObject* pObject)
{
	if (m_bActive)
		return;
	m_pAttach = pObject;
}

inline CObject *CAnime::GetAttach() const
{
	return m_pAttach;
}

inline void CAnime::SetRemove(bool bRemove)
{
	if (m_bActive)
		return;
	m_bRemove = bRemove;
}

inline bool CAnime::GetRemove() const
{
	return m_bRemove;
}

inline void CAnime::SetEndProc(FN_AnimeEnd AnimeEnd)
{
	if (m_bActive)
		return;
	OnAnimeEnd = AnimeEnd;
}

inline FN_AnimeEnd CAnime::GetEndProc() const
{
	return OnAnimeEnd;
}

inline void CAnime::SetEndParam(int *pParam)
{
	if (m_bActive)
		return;
	m_pParam = pParam;
}

inline int *CAnime::GetEndParam() const
{
	return m_pParam;
}

inline bool CAnime::IsActive() const
{
	return m_bActive;
}

inline bool CAnime::IsPause() const
{
	return m_bPause;
}

#endif
