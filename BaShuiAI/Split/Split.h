/*****************************************************************************
 *                                  ����˵��
 *
 * SplitPile.cpp�����Ƴɶѣ��ֳ���ͨ���ƣ���������ã�����˳�Ӳ��ƣ�������ã�
 * Split.cpp��    ���ƹ��̣��ֳ���ͨ���ƹ��̺���˳�Ӳ��ƹ���
 * SplitSpec.cpp��������ƣ��򵥷����Ƿ񹹳���������
 *                �ڷ����Ƿ�Ϊ��˳��ʱ����Ҫ������˳�Ӳ��ƹ���
 *
 * �����ƶ����ͣ����λΪ1����ʾ�������ͣ���7λ��ʾ��������
 *               ���λΪ0����ʾ��ͨ���ͣ���7λ��ʾ�����ƶѵ����ͣ�λͼ��
 *
 * ��˳�ӣ���AI�ڲ����ַ�Ϊ��˳�Ӳ���ͬ��˳����һ��ͬ��˳��������ͬ��˳
 *         ��AI�ⲿ��ͨ��SpecType��������˳�ӡ���ͨ��Type[3]������ͬ��˳��
 *
 *****************************************************************************/
#ifndef __Split_h__
#define __Split_h__

#define CHECK_RETURN(byRtnType) \
{ \
	if ((byRtnType) != INNER_TYPE_GVP) \
		return byRtnType; \
}

#define SHIFT_1(n)				((1 << (n)) - 1)

#define V_SINGLE				1
#define V_SISTER				0
#define V_A2					0
#define V_KA					0
#define V_PAIR					100
#define V_SHUNZI				1000
#define V_A23					1000
#define V_QKA					1000
#define V_BOMB					10000
#define V_S_SHUNZI				100000
#define V_S_A23					100000
#define V_S_QKA					100000
#define V_ATTACH_1ST_PAIR		(6 * 100000)
#define V_ATTACH_2ND_BOMB		(8 * 100000)
#define V_ATTACH_2ND_S_SHUNZI	(10 * 100000)

struct TPile {
	int				 nVA;
	int				 nAttachVA;
	byte			 byType;
	byte			 byGrade;

	TPile(int _nVA, int _nAttachVA, byte _byType, byte _byGrade)
	{
		nVA = _nVA;
		nAttachVA = _nAttachVA;
		byType = _byType;
		byGrade = _byGrade;
	}
};

typedef std::vector<TPile>		 VECPILE;

class CSplit {
  public:
	CSplit();
	virtual ~CSplit();

  public:
	dword			 ForceSplit(byte byCard[8], word wFlushBitmap);

  protected:
	void			 OutCards(byte byType, byte byGrade);
	void			 InCards(byte byType, byte byGrade);
  private:
	void			 _OutSingle(word mask);
	void			 _InSingle(word mask);
	void			 _OutSister(word mask);
	void			 _InSister(word mask);
	void			 _OutPair(word mask);
	void			 _InPair(word mask);
	void			 _OutBomb(word mask);
	void			 _InBomb(word mask);
	void			 _OutShunZi(word mask);
	void			 _InShunZi(word mask);
	void			 _OutSameShunZi(word mask);
	void			 _InSameShunZi(word mask);

  protected:
	byte			 SplitlPile(byte byType, byte &byGrade);
	byte			 SplitlPile_Sister(byte byType, byte &byGrade);
	byte			 SplitlPile_ShunZi(byte byType, byte &byGrade);
  private:
	byte			 _SplitSister(byte &byGrade);
	byte			 _SplitA2(byte &byGrade);
	byte			 _SplitKA(byte &byGrade);
	byte			 _SplitPair(byte &byGrade);
	byte			 _SplitShunZi(byte &byGrade);
	byte			 _SplitA23(byte &byGrade);
	byte			 _SplitQKA(byte &byGrade);
	byte			 _SplitBomb(byte &byGrade);
	byte			 _SplitSameShunZi(byte &byGrade);
	byte			 _SplitSameA23(byte &byGrade);
	byte			 _SplitSameQKA(byte &byGrade);

  protected:
	void			 PushPile(VECPILE &vecPile, byte byType, byte byGrade);
	void			 PopPile(VECPILE &vecPile, byte &byType, byte &byGrade);
	byte			 GetMaxSingle();
	void			 PlaceCards_1(const VECPILE &vecPile, byte byPlacedCard[8]);
	void			 PlaceCards_2(const VECPILE &vecPile, byte byPlacedCard[8]);
	void			 TransToBitmap(byte byCard[8]);
	dword			 GetCardBitmap(byte byCard[8], byte byPlacedCard[8]);
	dword			 GetTypeBitmap(VECPILE &vecPile);
	dword			 SplitCards(byte byCard[8], word wFlushBitmap);
	byte			 GetSpecType_TriShunZi(VECPILE &vecPile);
	dword			 SplitCards_TriShunZi(byte byCard[8], word wFlushBitmap, VECPILE &vecPile);

  protected:
	dword			 SplitSpecType(byte byCard[8], word wFlushBitmap);
  private:
	dword			 _SplitDblBomb(byte byCard[8], word wFlushBitmap);
	dword			 _SplitOneBomb(byte byCard[8], word wFlushBitmap);
	dword			 _SplitFourPair(byte byCard[8], word wFlushBitmap);
	dword			 _SplitOneShunZi(byte byCard[8], word wFlushBitmap);
	dword			 _SplitTriShunZi(byte byCard[8], word wFlushBitmap);

  protected:
	byte			 m_byCard[8];
	word			 bitmap[4];
	word			 m_wFlushBitmap;

  protected:
	VECPILE			 m_vecPile;
	dword			 VSum;
	dword			 VSumMax;
};

#endif
