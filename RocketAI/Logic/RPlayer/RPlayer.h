#ifndef __RPlayer_h__
#define __RPlayer_h__

#define PI_SINGLE_PHOBIA		0
#define PI_PAIR_PHOBIA			1
#define PI_SHUNZI_PHOBIA		2

#define CHECK_RETURN(byRtnType) \
{ \
	if ((byRtnType) != TYPE_GVP) \
		return byRtnType; \
}


class CRPlayer {
	typedef std::vector<word>		VECWORD;
  public:
	CRPlayer();
	~CRPlayer();

  public:
	bool			 IsAlive() const;
	void			 SetBitmap(word bitmap[4]);

  public:
	void			 RestoreBitmap(); //破解前调用
	bool			 HasCards(byte byGrade, byte byCount);
	bool			 HasRocket();
	bool			 HasLianDui();

  public: //卡牌进出
	void			 OutCards(byte byType, byte byGrade, byte byCount);
	void			 InCards(byte byType, byte byGrade, byte byCount);
  private:
	void			 _OutSingle(word mask);
	void			 _InSingle(word mask);
	void			 _OutPair(word mask);
	void			 _InPair(word mask);
	void			 _OutBomb(word mask);
	void			 _InBomb(word mask);
	void			 _OutCannon(word mask);
	void			 _InCannon(word mask);
	void			 _OutRedFlag();
	void			 _InRedFlag();
	void			 _OutRocket();
	void			 _InRocket();
	void			 _OutShunZi(word mask);
	void			 _InShunZi(word mask);
	void			 _OutLianDui(word mask);
	void			 _InLianDui(word mask);

  public: //出牌应答
	byte			 RspCards(byte byType, byte byRspType, byte &byGrade, byte &byCount);
  private:
	byte			 _RspGvp(byte byRspType, byte &byGrade, byte &byCount);
	byte			 _RspSingle(byte byRspType, byte &byGrade, byte &byCount);
	byte			 _RspPair(byte byRspType, byte &byGrade, byte &byCount);
	byte			 _RspBomb(byte byRspType, byte &byGrade, byte &byCount);
	byte			 _RspCannon(byte byRspType, byte &byGrade, byte &byCount);
	byte			 _RspRedFlag(byte byRspType, byte &byGrade, byte &byCount);
	byte			 _RspRocket(byte byRspType, byte &byGrade, byte &byCount);
	byte			 _RspShunZi(byte byRspType, byte &byGrade, byte &byCount);
	byte			 _RspLianDui(byte byRspType, byte &byGrade, byte &byCount);
	byte			 _RspCha(byte byRspType, byte &byGrade, byte &byCount);
	byte			 _RspGou(byte byRspType, byte &byGrade, byte &byCount);
	byte			 _RspSingleBySingle(byte &byGrade, byte &byCount);
	byte			 _RspPairByPair(byte &byGrade, byte &byCount);

  private: //尝试应答
	byte			 _TrySingle(byte &byGrade, byte &byCount);
	byte			 _TryPair(byte &byGrade, byte &byCount);
	byte			 _TryBomb(byte &byGrade, byte &byCount);
	byte			 _TryCannon(byte &byGrade, byte &byCount);
	byte			 _TryRedFlag(byte &byGrade, byte &byCount);
	byte			 _TryRocket(byte &byGrade, byte &byCount);
	byte			 _TryShunZi(byte &byGrade, byte &byCount);
	byte			 _TryLianDui(byte &byGrade, byte &byCount);

  private: //手牌分析
	void			 _CalcSinglePhobia(byte& byShunZiPhobia);
	void			 _CalcPairPhobia(byte& byShunZiPhobia);
	void			 _CalcShunZiPhobia(byte& byShunZiPhobia);

  public:
	void			 GetTypePhobia(byte byTypePhobia[3]);
	void			 GetWinRate(byte& byWinRate);
	void			 SetThkParam(byte byPhobiaAttack, byte byWinRhythm);

  public:
	word			 GetOddmentMask();
	bool			 IsStupidPut(byte byType, byte byGrade, byte byCount);
  private:
	bool			 _CheckStupid(byte byType, byte byGrade, byte byCount);
	bool			 _CheckStupid_Oddment(byte byType, byte byGrade, byte byCount, word old, word new1);
	bool			 _CheckStupid_OnesOwn(byte byType, byte byGrade, byte byCount, word old, word new1);
	bool			 _CheckStupid_Together(byte byType, byte byGrade, byte byCount, word old, word new1);
  private:
	bool			 _CheckStupid_Single(byte byGrade, byte byCount, word old, word new1);
	bool			 _CheckStupid_Pair(byte byGrade, byte byCount, word old, word new1);
	bool			 _CheckStupid_ShunZi(byte byGrade, byte byCount, word old, word new1);
	bool			 _CheckStupid_Bomb(byte byGrade, byte byCount, word old, word new1);

  public: //拆牌，价值分析
	int				 SplitCards();
	void			 ValueAnalysis();
  private:
	byte			 _SplitCards(byte byType, byte &byGrade, byte &byCount);
	byte			 _SplitBomb(byte &byGrade, byte &byCount);
	byte			 _SplitCannon(byte &byGrade, byte &byCount);
	byte			 _SplitRedFlag(byte &byGrade, byte &byCount);
	byte			 _SplitRocket(byte &byGrade, byte &byCount);
	byte			 _SplitShunZi(byte &byGrade, byte &byCount);
	byte			 _SplitLianDui(byte &byGrade, byte &byCount);
  private:
	void			 _MergeBomb(byte &byGrade, byte &byCount);
	void			 _MergeCannon(byte &byGrade, byte &byCount);
	void			 _MergeRedFlag(byte &byGrade, byte &byCount);
	void			 _MergeRocket(byte &byGrade, byte &byCount);
	void			 _MergeShunZi(byte &byGrade, byte &byCount);
	void			 _MergeLianDui(byte &byGrade, byte &byCount);

  private:
	void			 _RevSplitCards(byte &byType, byte &byGrade, byte &byCount);
	int				 _CalcRemainVA();

  public: //武断应答
	byte			 MakeCards(byte byChair1, byte byType, byte &byGrade, byte &byCount);
  private:
	byte			 _MakeCards(byte byChair1, byte byType, byte &byGrade, byte &byCount);
	byte			 _MakeGvp(byte byChair1, byte &byGrade, byte &byCount);
	byte			 _MakeSingle(byte byChair1, byte &byGrade, byte &byCount);
	byte			 _MakePair(byte byChair1, byte &byGrade, byte &byCount);
	byte			 _MakeBomb(byte byChair1, byte &byGrade, byte &byCount);
	byte			 _MakeCannon(byte byChair1, byte &byGrade, byte &byCount);
	byte			 _MakeRedFlag(byte byChair1, byte &byGrade, byte &byCount);
	byte			 _MakeRocket(byte byChair1, byte &byGrade, byte &byCount);
	byte			 _MakeShunZi(byte byChair1, byte &byGrade, byte &byCount);
	byte			 _MakeLianDui(byte byChair1, byte &byGrade, byte &byCount);
	byte			 _MakeBCRR(byte byChair1, byte &byGrade, byte &byCount);
	byte			 _MakeCha(byte byChair1, byte &byGrade, byte &byCount);
	byte			 _MakeGou(byte byChair1, byte &byGrade, byte &byCount);
  private:
	byte			 _TryMakeShunZi(byte &byGrade, byte &byCount);
	byte			 _TryMakeLianDui(byte &byGrade, byte &byCount);

  protected:
	bool			 SuppressPlayer(byte byChair1) const;
	bool			 ReleasePlayer(byte byChair) const;

  protected:
	word			 bitmap[4]; //手牌位图
	word			 bitmap_bak[4]; //手牌位图

  protected:
	byte			 m_byPhobiaAttack; //恐惧攻击
	byte			 m_byWinRhythm; //炸弹抑制
	bool			 m_bShieldRocket; //屏蔽火箭
};

inline bool CRPlayer::IsAlive() const
{
	return bitmap[0] || bitmap[1] || bitmap[2] || bitmap[3];
}

inline bool CRPlayer::SuppressPlayer(byte byChair1) const
{
	return m_byWinRhythm & (1 << byChair1) || m_byWinRhythm & 0x01;
}

inline bool CRPlayer::ReleasePlayer(byte byChair) const
{
	if (byChair != 2)
		return false;
	return (m_byWinRhythm & 0x01) == 0 || (m_byWinRhythm & 0x04) != 0;
}

#endif
