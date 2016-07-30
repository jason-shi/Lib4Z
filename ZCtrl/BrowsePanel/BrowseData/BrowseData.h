#ifndef __BrowseData_h__
#define __BrowseData_h__

typedef void (*FN_FlushData)(void *pBrowseView);

class CBrowseData {
  public:
	CBrowseData(int nCycleLine);
	virtual ~CBrowseData();

  public:
	void			 SetBrowseView(void *pBrowseView);
	void			 SetFN_FlushData(FN_FlushData FlushData);

  public:
	bool			 IsDataLost(int nDataIndex) const;
	void			 ClearDataLost(int nDataIndex);

  public:
	int				 GetTotalLine() const;
	int				 GetCycleLine() const;
	int				 GetDataCursor() const;

  public:
	int				 GetBarHeight(int nDataIndex) const;
	int				 GetRealHeight(int nBrowseLine) const;
	int				 GetFrontHeight(int nBrowseLine) const;
	int				 GetFieldHeight(int nBrowseLine, int nViewHeight, bool bAutoScroll) const;
	virtual int		 GetTotalHeight(int nViewHeight, bool bAutoScroll) const;
	virtual int		 GetProximalLine(int nFrontHeight) const;
	virtual int		 GetBrowseLine(int nViewHeight) const;

  private:
	int				 _GetLastHeight() const;

  protected:
	void			 OnRecvData(int nHeight); //向后添加（循环）
	void			 OnChangeData(int nDataIndex, int nHeight);

  public:
	void			 FlushData(); //默认视图更新显示
	void			 FlushData(void *pBrowseView); //指定视图更新显示

  protected:
	void			*m_pBrowseView;
	int				 m_nCycleLine; //数据区循环行数，0代表不循环
	int				 m_nTotalLine; //数据区当前行数，循环时保持在循环行数
	int				 m_nDataCursor; //当前存储位置，循环时[0, m_nCycleLine)

  protected:
	std::vector<bool>		 m_vecDataLost;
	std::vector<word>		 m_vecHeight;
	int						 m_nTotalHeight;

  protected:
	FN_FlushData	 OnFlushData;
};

inline void CBrowseData::SetBrowseView(void *pBrowseView)
{
	m_pBrowseView = pBrowseView;
}

inline void CBrowseData::SetFN_FlushData(FN_FlushData FlushData)
{
	OnFlushData = FlushData;
}

inline bool CBrowseData::IsDataLost(int nDataIndex) const
{
	return m_vecDataLost[nDataIndex];
}

inline void CBrowseData::ClearDataLost(int nDataIndex)
{
	m_vecDataLost[nDataIndex] = false;
}

inline int CBrowseData::GetCycleLine() const
{
	return m_nCycleLine;
}

inline int CBrowseData::GetTotalLine() const
{
	return m_nTotalLine;
}

inline int CBrowseData::GetDataCursor() const
{
	return m_nDataCursor;
}

#endif
