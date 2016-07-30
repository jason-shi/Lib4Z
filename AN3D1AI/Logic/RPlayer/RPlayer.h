#ifndef __RPlayer_h__
#define __RPlayer_h__

#define CHECK_RETURN(byRtnType) \
{ \
	if ((byRtnType) != TYPE_GVP) \
		return byRtnType; \
}


class CRPlayer {
	friend class	 CRCrack;
	typedef std::vector<word>		VECWORD;
  public:
	CRPlayer();
	virtual ~CRPlayer();

  public:
	void			 RestoreWith(byte byIndex, const TCrackParam& tCrackParam);

  public:
	void			 SetOriginVA(int nOriginVA);
	bool			 HasCard(byte byType);
	byte			 GetMaxGrade(byte byType, byte &byGrade);
	byte			 GetMaxScore(byte byType, byte &byGrade, byte &byScore);

  public: //卡牌进出
	void			 OutCards(byte byType, byte byGrade);
	void			 InCards(byte byType, byte byGrade);

  public: //出牌应答
	byte			 RspCards(byte byType, byte byRspType, byte &byGrade);
	byte			 RspGvp(byte byRspType, byte &byGrade);
	byte			 RspPut(byte byType, byte byRspType, byte &byGrade);
	virtual byte	 RspPutSame(byte byType, byte byRspType, byte &byGrade);
	virtual byte	 RspPutDiff(byte byType, byte byRspType, byte &byGrade);

  public: //武断应答
	byte			 MakeCards(byte byType, byte &byGrade);
	byte			 MakeGvp(byte &byGrade);
	byte			 MakePut(byte byType, byte &byGrade);
	byte			 MakePutSame(byte byType, byte &byGrade);
	byte			 MakePutDiff(byte byType, byte &byGrade);

  public:
	byte			 MakeWithScore(byte byType, byte &byGrade);
	byte			 MakeWithoutScore(byte byType, byte &byGrade);
	byte			 MakeByLittle(byte byType, byte &byGrade);
	byte			 MakeJustOver(byte byType, byte &byGrade, byte byTypeM, byte byGradeM);

  protected:
	word			 bitmap[4]; //0 means trump card, the low 16 bits are used to indicate from 3 to big joker
                                //1,2,3 means minor card, the low 12 bits are used to indicate from 3 to ace
	byte			 m_byFuTwo;
	byte			 m_byFriend;
	byte			 m_byChair1;
};

inline bool CRPlayer::HasCard(byte byType)
{
	return bitmap[byType - 1] != 0;
}

inline byte CRPlayer::RspCards(byte byType, byte byRspType, byte &byGrade)
{
	return byType == TYPE_GVP ? RspGvp(byRspType, byGrade)
							  : RspPut(byType, byRspType, byGrade);
}

#endif
