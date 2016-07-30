#ifndef __RPlayer_h__
#define __RPlayer_h__

#define CHECK_RETURN(byRtnType) \
{ \
	if ((byRtnType) != TYPE_GVP) \
		return byRtnType; \
}


class CRPlayer {
	typedef std::vector<word>		VECWORD;
  public:
	CRPlayer();
	virtual ~CRPlayer();

  public:
	bool			 IsAlive() const;
	void			 SetBitmap(word bitmap[4]);

  public:
	void			 SetFriend(byte byFriend);
	void			 SetChair1(byte byChair1);
	void			 SetOriginVA(int nOriginVA);
	void			 RestoreBitmap(); //破解前调用
	bool			 HasCards(byte byGrade, byte byCount);
	bool			 HasLianDui();
	int				 GetCardsCount() const;
	bool			 CheckBeat(byte byType, byte byGrade, byte byCount, word wAttach);

  public: //卡牌进出
	void			 OutCards(byte byType, byte byGrade, byte byCount, word wAttach = 0);
	void			 InCards(byte byType, byte byGrade, byte byCount, word wAttach = 0);
  private:
	void			 _OutSingle(word mask);
	void			 _InSingle(word mask);
	void			 _OutPair(word mask);
	void			 _InPair(word mask);
	void			 _OutTrip(word mask);
	void			 _InTrip(word mask);
	void			 _OutTripS(word mask, word wAttach);
	void			 _InTripS(word mask, word wAttach);
	void			 _OutTripP(word mask, word wAttach);
	void			 _InTripP(word mask, word wAttach);
	void			 _OutPlane(word mask);
	void			 _InPlane(word mask);
	void			 _OutPlaneS(word mask, word wAttach);
	void			 _InPlaneS(word mask, word wAttach);
	void			 _OutPlaneP(word mask, word wAttach);
	void			 _InPlaneP(word mask, word wAttach);
	void			 _OutQuadS(word mask, word wAttach);
	void			 _InQuadS(word mask, word wAttach);
	void			 _OutQuadP(word mask, word wAttach);
	void			 _InQuadP(word mask, word wAttach);
	void			 _OutBomb(word mask);
	void			 _InBomb(word mask);
	void			 _OutRocket();
	void			 _InRocket();
	void			 _OutShunZi(word mask);
	void			 _InShunZi(word mask);
	void			 _OutLianDui(word mask);
	void			 _InLianDui(word mask);

  public: //速胜应答
	byte			 OneHand(byte byType, byte &byRspType, byte &byGrade, byte &byCount, word &wAttach);
	byte			 OneHand_GetType();
	byte			 OneHand_GetGrade(byte byRspType);
	byte			 OneHand_GetCount(byte byRspType);
	word			 OneHand_GetAttach(byte byRspType);

  public: //出牌应答
	byte			 RspCards(byte byType, byte byRspType, byte &byGrade, byte &byCount, word &wAttach, word &wChoice);
  private:
	byte			 _RspGvp(byte byRspType, byte &byGrade, byte &byCount, word &wAttach, word &wChoice);
	byte			 _RspSingle(byte byRspType, byte &byGrade, byte &byCount);
	byte			 _RspPair(byte byRspType, byte &byGrade, byte &byCount);
	byte			 _RspTrip(byte byRspType, byte &byGrade, byte &byCount);
	byte			 _RspTripS(byte byRspType, byte &byGrade, byte &byCount, word &wAttach, word &wChoice);
	byte			 _RspTripP(byte byRspType, byte &byGrade, byte &byCount, word &wAttach, word &wChoice);
	byte			 _RspPlane(byte byRspType, byte &byGrade, byte &byCount);
	byte			 _RspPlaneS(byte byRspType, byte &byGrade, byte &byCount, word &wAttach, word &wChoice);
	byte			 _RspPlaneP(byte byRspType, byte &byGrade, byte &byCount, word &wAttach, word &wChoice);
	byte			 _RspQuadS(byte byRspType, byte &byGrade, byte &byCount, word &wAttach, word &wChoice);
	byte			 _RspQuadP(byte byRspType, byte &byGrade, byte &byCount, word &wAttach, word &wChoice);
	byte			 _RspShunZi(byte byRspType, byte &byGrade, byte &byCount);
	byte			 _RspLianDui(byte byRspType, byte &byGrade, byte &byCount);
	byte			 _RspSingleBySingle(byte &byGrade, byte &byCount);
	byte			 _RspPairByPair(byte &byGrade, byte &byCount);
	byte			 _RspBomb(byte byRspType, byte &byGrade, byte &byCount);
	byte			 _RspRocket(byte byRspType, byte &byGrade, byte &byCount);

  private: //尝试应答
	byte			 _RspAnySingle(byte &byGrade, byte &byCount);
	byte			 _RspAnyPair(byte &byGrade, byte &byCount);
	byte			 _RspAnyTrip(byte &byGrade, byte &byCount);
	byte			 _RspAnyTripS(byte &byGrade, byte &byCount, word &wAttach, word &wChoice);
	byte			 _RspAnyTripP(byte &byGrade, byte &byCount, word &wAttach, word &wChoice);
	byte			 _RspAnyPlane(byte &byGrade, byte &byCount);
	byte			 _RspAnyPlaneS(byte &byGrade, byte &byCount, word &wAttach, word &wChoice);
	byte			 _RspAnyPlaneP(byte &byGrade, byte &byCount, word &wAttach, word &wChoice);
	byte			 _RspAnyQuadS(byte &byGrade, byte &byCount, word &wAttach, word &wChoice);
	byte			 _RspAnyQuadP(byte &byGrade, byte &byCount, word &wAttach, word &wChoice);
	byte			 _RspAnyBomb(byte &byGrade, byte &byCount);
	byte			 _RspAnyRocket(byte &byGrade, byte &byCount);
	byte			 _RspAnyShunZi(byte &byGrade, byte &byCount);
	byte			 _RspAnyLianDui(byte &byGrade, byte &byCount);

  private:
	void			 _InitChoice1S(word &wChoice);
	void			 _InitChoice1P(word &wChoice);
	void			 _InitChoice2S(word &wChoice);
	void			 _InitChoice2P(word &wChoice);
	void			 _InitChoiceNS(byte byCount, word &wChoice);
	void			 _InitChoiceNP(byte byCount, word &wChoice);
	bool			 _ChangeAttach1S(word &wAttach, word &wChoice); //改变附加牌
	bool			 _ChangeAttach1P(word &wAttach, word &wChoice); //改变附加牌
	bool			 _ChangeAttach2S(word &wAttach, word &wChoice); //改变附加牌
	bool			 _ChangeAttach2P(word &wAttach, word &wChoice); //改变附加牌
	bool			 _ChangeAttachNS(word &wAttach, word &wChoice); //改变附加牌
	bool			 _ChangeAttachNP(word &wAttach, word &wChoice); //改变附加牌

  public: //手牌分析
	bool			 IsSinglePhobia();
	bool			 IsPairPhobia();
	void			 SetDZPhobia(CRPlayer *pDiZhu);
  public:
	bool			 HasWinRhythm();
	void			 SetWinRhythm(CRPlayer *pRPlayer[3]);

  public:
	bool			 IsBestPut(byte byType, byte byGrade, byte byCount, word wAttach = 0);
	bool			 IsBetterPut(byte byType, byte byGrade, byte byCount, word wAttach = 0);
	bool			 IsStupidPut(byte byType, byte byGrade, byte byCount, word wAttach = 0);
  private:
	bool			 _CheckStupid_OnesOwn(byte byType, byte byGrade, byte byCount);
	bool			 _CheckStupid_Single(byte byGrade, byte byCount);
	bool			 _CheckStupid_Pair(byte byGrade, byte byCount);
	bool			 _CheckStupid_Trip(byte byGrade, byte byCount);
	bool			 _CheckStupid_ShunZi(byte byGrade, byte byCount);
	bool			 _CheckStupid_LianDui(byte byGrade, byte byCount);
	bool			 _CheckStupid_Bomb(byte byGrade, byte byCount);
	bool			 _CheckStupid_Rocket(byte byGrade, byte byCount);

  public: //拆牌，价值分析
	int				 SplitCards();
  private:
	byte			 _SplitCards(byte byType, byte &byGrade, byte &byCount);
	byte			 _SplitTrip(byte &byGrade, byte &byCount);
	byte			 _SplitShunZi(byte &byGrade, byte &byCount);
	byte			 _SplitLianDui(byte &byGrade, byte &byCount);
	byte			 _SplitBomb(byte &byGrade, byte &byCount);
	byte			 _SplitRocket(byte &byGrade, byte &byCount);
  private:
	void			 _MergeTrip(byte &byGrade, byte &byCount);
	void			 _MergeBomb(byte &byGrade, byte &byCount);
	void			 _MergeRocket(byte &byGrade, byte &byCount);
	void			 _MergeShunZi(byte &byGrade, byte &byCount);
	void			 _MergeLianDui(byte &byGrade, byte &byCount);

  private:
	void			 _RevSplitCards(byte &byType, byte &byGrade, byte &byCount);
	int				 _CalcRemainVA();

  public: //武断应答
	byte			 MakeCards(byte byType, byte &byGrade, byte &byCount, word &wAttach);
  protected:
	virtual byte	 _MakeGvp(byte &byGrade, byte &byCount, word &wAttach);
	virtual byte	 _MakeSingle(byte &byGrade, byte &byCount);
	virtual byte	 _MakePair(byte &byGrade, byte &byCount);
	byte			 _MakeTrip(byte &byGrade, byte &byCount);
	byte			 _MakeTripS(byte &byGrade, byte &byCount, word &wAttach);
	byte			 _MakeTripP(byte &byGrade, byte &byCount, word &wAttach);
	byte			 _MakePlane(byte &byGrade, byte &byCount);
	byte			 _MakePlaneS(byte &byGrade, byte &byCount, word &wAttach);
	byte			 _MakePlaneP(byte &byGrade, byte &byCount, word &wAttach);
	byte			 _MakeQuadS(byte &byGrade, byte &byCount, word &wAttach);
	byte			 _MakeQuadP(byte &byGrade, byte &byCount, word &wAttach);
	byte			 _MakeShunZi(byte &byGrade, byte &byCount);
	byte			 _MakeLianDui(byte &byGrade, byte &byCount);
	byte			 _MakeBomb(byte &byGrade, byte &byCount);
	byte			 _MakeRocket(byte &byGrade, byte &byCount);
  protected:
	byte			 _MakeAnySingle(byte &byGrade, byte &byCount);
	byte			 _MakeAnyPair(byte &byGrade, byte &byCount);
	byte			 _MakeAnyShunZi(byte &byGrade, byte &byCount);
	byte			 _MakeAnyLianDui(byte &byGrade, byte &byCount);
  private:
	void			 _MakeAttach1S(word &wAttach);
	void			 _MakeAttach1P(word &wAttach);
	void			 _MakeAttach2S(word &wAttach);
	void			 _MakeAttach2P(word &wAttach);
	void			 _MakeAttachNS(byte byCount, word &wAttach);
	void			 _MakeAttachNP(byte byCount, word &wAttach);

  protected:
	bool			 SuppressPlayer(byte byChair1) const;
	bool			 ReleasePlayer(byte byChair1) const;

  protected:
	word			 bitmap[4];        //手牌位图
	word			 bitmap_bak[4];    //手牌位图

  protected:
	byte			 m_byFriend;
	byte			 m_byChair1;
	byte			 m_bSinglePhobia;  //单张恐惧
	byte			 m_bPairPhobia;    //对子恐惧
	byte			 m_byWinRhythm;    //炸弹抑制
	int				 m_nOriginVA;      //原始价值
};

inline bool CRPlayer::IsAlive() const
{
	return bitmap[0] || bitmap[1] || bitmap[2] || bitmap[3];
}

inline void CRPlayer::SetFriend(byte byFriend)
{
	m_byFriend = byFriend & 0x06;
}

inline void CRPlayer::SetChair1(byte byChair1)
{
	m_byChair1 = byChair1;
}

inline void CRPlayer::SetOriginVA(int nOriginVA)
{
	m_nOriginVA = nOriginVA;
}

inline bool CRPlayer::_ChangeAttach1S(word &wAttach, word &wChoice)
{
	if (wChoice == 0)
		return false;

	wAttach = wChoice & -wChoice;
	wChoice ^= wAttach;
	return true;
}

inline bool CRPlayer::_ChangeAttach1P(word &wAttach, word &wChoice)
{
	return _ChangeAttach1S(wAttach, wChoice);
}

inline bool CRPlayer::_ChangeAttach2S(word &wAttach, word &wChoice)
{
	if (wChoice == 0)
		return false;

	wAttach = wChoice;
	wChoice = 0;
	return true;
}

inline bool CRPlayer::_ChangeAttach2P(word &wAttach, word &wChoice)
{
	return _ChangeAttach2S(wAttach, wChoice);
}

inline bool CRPlayer::_ChangeAttachNS(word &wAttach, word &wChoice)
{
	return _ChangeAttach2S(wAttach, wChoice);
}

inline bool CRPlayer::_ChangeAttachNP(word &wAttach, word &wChoice)
{
	return _ChangeAttachNS(wAttach, wChoice);
}

#endif
