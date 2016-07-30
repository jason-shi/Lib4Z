#include "StdAfx.h"
#include "RCrack.h"
#include "..\RPlayer\RPlayerRole\RPlayerBK.h"
#include "..\RPlayer\RPlayerRole\RPlayerSJ.h"
#include "..\RPlayer\RPlayerRole\RPlayerDJ.h"
#include "..\RPlayer\RPlayerRole\RPlayerXJ.h"

using namespace std;

struct TMaxGrade {
	byte		 byType;
	byte		 byGrade;
};

struct TMaxScore {
	byte		 byType;
	byte		 byGrade;
	byte		 byScore;
};

//ׯ��ʹ�õ�����
static byte			 g_byTrumpCount[4];
static bool			 g_bCanTrumpKill;
static bool			 g_bEscapeCard[4];
static bool			 g_bBankerSweep[4];

//�м�ʹ�õ�����
static word			 g_wScoreBitmap[4];

static TMaxGrade	 g_tMaxGrade[4];
static byte			 g_byChairM0123;
static byte			 g_byTypeM0123;
static byte			 g_byGradeM0123;

static TMaxScore	 g_tMaxScore[4];
static byte			 g_byFriendScore;

static bool			 g_bICanOverBanker;
static bool			 g_bHadOverBanker;
static bool			 g_bOverBankerWithScore;

byte CRCrack::GetMiddleGrade(word _bitmap)
{
	word temp = _bitmap;

	//���ȳ����Ƿֵ���
	_bitmap &= ~MASK_5TK;
	if (_bitmap != 0)
		return TblBitIndex[(_bitmap & -_bitmap) % 37];

	//����С����
	return TblBitIndex[(temp & -temp) % 37];
}

void CRCrack::InitTrumpCount()
{
	for (int i = 0; i < 4; i++)
	{
		word _bitmap = m_pRPlayer[i]->bitmap[0];

		g_byTrumpCount[i] = WORD_BIT_COUNT(_bitmap);
		if (m_pRPlayer[i]->m_byFuTwo != 0)
			g_byTrumpCount[i] += m_pRPlayer[i]->m_byFuTwo - 1;
	}

	g_bCanTrumpKill = true;

	//ׯ�Ҳ�����ɱ������
	for (int i = 1; i < 4; i++)
	{
		if (g_byTrumpCount[0] <= g_byTrumpCount[i]) //���Ƹ���������
		{
			g_bCanTrumpKill = false;
			break;
		}
		if (g_byTrumpCount[0] <= g_byTrumpCount[i] + 1) //��һ��
		{
			if (m_pRPlayer[0]->bitmap[0] <= m_pRPlayer[i]->bitmap[0]) //���Ʋ�����
			{
				g_bCanTrumpKill = false;
				break;
			}
		}
	}

	//ׯ�Ҳ�����ɱ������
	if (g_byTrumpCount[0] == 1 //���һ����
		&& g_byTrumpCount[1] == 0
		&& g_byTrumpCount[2] == 0
		&& g_byTrumpCount[3] == 0)
	{
		for (int i = 1; i < 4; i++)
		{
			for (int j = 1; j < 4; j++)
			{
				if (m_pRPlayer[0]->bitmap[j] == 0)
					continue;
				if (m_pRPlayer[0]->bitmap[j] < m_pRPlayer[i]->bitmap[j]) //���Ʋ�����
				{
					g_bCanTrumpKill = false;
					break;
				}
			}
		}
	}
}

void CRCrack::InitEscapeCard()
{
	memset(g_bEscapeCard, 0, sizeof(g_bEscapeCard));
	for (byte bySuit = TYPE_HEART; bySuit != TYPE_ERROR; bySuit++)
	{
		word _bitmap = m_pRPlayer[0]->bitmap[bySuit - 1];
		if (_bitmap == 0)
			continue;

		if (m_pRPlayer[1]->bitmap[bySuit - 1] == 0 //ֻ��ׯ���е���
			&& m_pRPlayer[2]->bitmap[bySuit - 1] == 0
			&& m_pRPlayer[3]->bitmap[bySuit - 1] == 0)
		{
			g_bEscapeCard[bySuit - 1] = false;
			continue;
		}
		if (WORD_BIT_COUNT(_bitmap) >= 5) //���ֻ�ɫ���ƽ϶�
		{
			g_bEscapeCard[bySuit - 1] = false;
			continue;
		}
		if (_bitmap > m_pRPlayer[1]->bitmap[bySuit - 1] //��������
			&& _bitmap > m_pRPlayer[2]->bitmap[bySuit - 1]
			&& _bitmap > m_pRPlayer[3]->bitmap[bySuit - 1])
		{
			g_bEscapeCard[bySuit - 1] = false;
			continue;
		}
		if ((_bitmap & 0x03FF) == 0) //����A,K
		{
			g_bEscapeCard[bySuit - 1] = false;
			continue;
		}
		g_bEscapeCard[bySuit - 1] = true;
	}

	for (byte bySuit = TYPE_HEART; bySuit != TYPE_ERROR; bySuit++)
	{
		word _bitmap = m_pRPlayer[0]->bitmap[bySuit - 1];
		if (_bitmap == 0)
			continue;
		if (_bitmap & _bitmap - 1) //��Ψһ
			continue;

		//Ψһ�Ҳ�����󣬱ص�
		if (m_pRPlayer[1]->bitmap[bySuit - 1] > _bitmap
			|| m_pRPlayer[2]->bitmap[bySuit - 1] > _bitmap
			|| m_pRPlayer[3]->bitmap[bySuit - 1] > _bitmap)
		{
			memset(g_bEscapeCard, 0, sizeof(g_bEscapeCard));
			g_bEscapeCard[bySuit - 1] = true;
			break;
		}
	}
}

void CRCrack::SimSweepCard(word bitmap[4], byte byFuTwo[4])
{
	//ׯ�ҳ�����
	byte byGrade = bitmap[0] & 0xFF00 ? TblMaxGrade[bitmap[0] >> 8] + 8 : TblMaxGrade[bitmap[0]];
	if (byGrade != 12)
		bitmap[0] ^= 1 << byGrade;
	else
	{
		if (--byFuTwo[0] == 0)
			bitmap[0] ^= 1 << byGrade;
	}

	//�мҳ���С��
	for (int i = 1; i < 4; i++)
	{
		if (bitmap[i] != 0)
		{
			byGrade = TblBitIndex[(bitmap[i] & -bitmap[i]) % 37];
			if (byGrade != 12)
				bitmap[i] ^= 1 << byGrade;
			else
			{
				if (--byFuTwo[i] == 0)
					bitmap[i] ^= 1 << byGrade;
			}
		}
	}
}

void CRCrack::InitBankerSweep()
{
	for (byte bySuit = TYPE_SPADE; bySuit != TYPE_ERROR; bySuit++)
	{
		word bitmap[4] = { 0 };
		byte byFuTwo[4] = { 0 };
		for (int i = 0; i < 4; i++)
		{
			bitmap[i] = m_pRPlayer[i]->bitmap[bySuit - 1];
			if (bySuit == TYPE_SPADE)
				byFuTwo[i] = m_pRPlayer[i]->m_byFuTwo;
		}

		g_bBankerSweep[bySuit - 1] = bitmap[0] != 0;
		while (bitmap[0] != 0)
		{
			//�ж��м��Ƿ��ׯ�Ҵ�
			if (bitmap[1] > bitmap[0] || bitmap[2] > bitmap[0] || bitmap[3] > bitmap[0])
			{
				g_bBankerSweep[bySuit - 1] = false;
				break;
			}
			//�ж��м���������ɱ
			if (bySuit != TYPE_SPADE)
			{
				if (bitmap[1] == 0 && m_pRPlayer[1]->bitmap[0] != 0
					|| bitmap[2] == 0 && m_pRPlayer[2]->bitmap[0] != 0
					|| bitmap[3] == 0 && m_pRPlayer[3]->bitmap[0] != 0)
				{
					g_bBankerSweep[bySuit - 1] = false;
					break;
				}
			}
			//ģ�����ƶ���
			SimSweepCard(bitmap, byFuTwo);
		}
	}
}

void CRCrack::InitMaxGrade()
{
	//������ĳ��Ʒ�ʽ
	memset(g_tMaxGrade, 0, sizeof(g_tMaxGrade));
	for (int i = 0; i < Z.byChair1; i++)
		g_tMaxGrade[i].byType = m_pRPlayer[i]->GetMaxGrade(Z.byType, g_tMaxGrade[i].byGrade);

	//�������
	byte byChairM = Z.byChairM;
	byte byTypeM = Z.byTypeM;
	byte byGradeM = Z.byGradeM;
	for (int i = 0; i < Z.byChair1; i++)
	{
		if (g_tMaxGrade[i].byType == byTypeM) //�������ͬ��ɫ
		{
			if (g_tMaxGrade[i].byGrade > byGradeM || byGradeM == GRADE_INIT)
			{
				byChairM = i;
				byGradeM = g_tMaxGrade[i].byGrade;
			}
		}
		else if (g_tMaxGrade[i].byType == TYPE_SPADE) //��һ������ɱ
		{
			byChairM = i;
			byTypeM = g_tMaxGrade[i].byType;
			byGradeM = g_tMaxGrade[i].byGrade;
		}
	}
	g_byChairM0123 = byChairM;
	g_byTypeM0123 = byTypeM;
	g_byGradeM0123 = byGradeM;
}

void CRCrack::InitMaxScore()
{
	byte byRspType;
	byte byGrade;
	byte byScore;
	memset(g_tMaxScore, 0, sizeof(g_tMaxScore));
	for (int i = 0; i < Z.byChair1; i++)
	{
		byRspType = m_pRPlayer[i]->GetMaxScore(Z.byType, byGrade, byScore);
		g_tMaxScore[i].byType = byRspType;
		g_tMaxScore[i].byGrade = byGrade;
		g_tMaxScore[i].byScore = byScore;
	}

	//�������Ҷ��ѵ�������
	g_byFriendScore = m_tCrackParam.tBattleParam.byCurr5TK;
	for (int i = 1; i < Z.byChair1; i++)
	{
		if (i == m_byBanker)
			continue;
		g_byFriendScore += g_tMaxScore[i].byScore;
	}
}

void CRCrack::InitOverBanker()
{
	//�Լ���ׯ�ұȽ�
	if (Z.byChairM == m_byBanker)
	{
		g_bICanOverBanker = false;
		if (g_tMaxGrade[0].byType == Z.byTypeM) //�������ͬ��ɫ
		{
			if (g_tMaxGrade[0].byGrade > Z.byGradeM || Z.byGradeM == GRADE_INIT)
				g_bICanOverBanker = true;
		}
		else if (g_tMaxGrade[0].byType == TYPE_SPADE) //��һ������ɱ
			g_bICanOverBanker = true;
	}
	else
	{
		if (Z.byChair1 <= m_byBanker)
		{
			g_bICanOverBanker = false;
		}
		else
		{
			g_bICanOverBanker = true;
			if (g_tMaxGrade[m_byBanker].byType != TYPE_GVP
				&& g_tMaxGrade[m_byBanker].byType == g_tMaxGrade[0].byType) //�������ͬ��ɫ
			{
				if (g_tMaxGrade[m_byBanker].byGrade > g_tMaxGrade[0].byGrade || g_tMaxGrade[0].byGrade == GRADE_INIT)
					g_bICanOverBanker = false;
			}
			else if (g_tMaxGrade[m_byBanker].byType == TYPE_SPADE) //��һ������ɱ
				g_bICanOverBanker = false;
		}
	}

	//���ѳ����Ƿ��ׯ�Ҵ�
	g_bOverBankerWithScore = false;
	if (Z.byChair1 > m_byBanker)
	{
		for (int i = 1; i < Z.byChair1; i++)
		{
			if (i == m_byBanker)
				continue;
			if (g_tMaxScore[i].byScore == 0)
				continue;
			if (g_tMaxScore[i].byType != TYPE_GVP
				&& g_tMaxScore[i].byType == g_tMaxGrade[m_byBanker].byType
				&& g_tMaxScore[i].byGrade > g_tMaxGrade[m_byBanker].byGrade) //������ͬ�ȼ���
			{
				g_bOverBankerWithScore = true;
				break;
			}
			if (g_tMaxScore[i].byType == TYPE_SPADE
				&& g_tMaxGrade[m_byBanker].byType != TYPE_SPADE) //����������ɱ
			{
				g_bOverBankerWithScore = true;
				break;
			}
		}
	}

	//�����Ƿ��Ѿ���ׯ�Ҵ�
	g_bHadOverBanker = false;
	if (Z.byChair1 > m_byBanker)
	{
		if (g_tMaxGrade[m_byBanker].byType == TYPE_GVP) //ׯ�ҵ渱��
		{
			g_bHadOverBanker = true;
		}
		if (Z.byTypeM == g_tMaxGrade[m_byBanker].byType
			&& Z.byGradeM >= g_tMaxGrade[m_byBanker].byGrade) //������ͬ�ȼ���
		{
			g_bHadOverBanker = true;
		}
		if (Z.byTypeM == TYPE_SPADE
			&& g_tMaxGrade[m_byBanker].byType != TYPE_SPADE) //��������ɱ
		{
			g_bHadOverBanker = true;
		}
	}
}

byte CRCrack::BankerFirstCard(byte &byGrade)
{
	//��ʼ����������
	InitTrumpCount();
	InitBankerSweep();

	//�����Է����ٵ�
	if (m_pRPlayer[1]->bitmap[0] > m_pRPlayer[0]->bitmap[0]
		|| m_pRPlayer[2]->bitmap[0] > m_pRPlayer[0]->bitmap[0]
		|| m_pRPlayer[3]->bitmap[0] > m_pRPlayer[0]->bitmap[0])
	{
		word _bitmap = m_pRPlayer[0]->bitmap[0];
		if (_bitmap != 0)
		{
			if ((_bitmap & ~MASK_5TK) != 0)
			{
				_bitmap &= ~MASK_5TK;
				if ((_bitmap & _bitmap - 1) == 0) //�������ĵ���
				{
					byte byTrumpCount = 0;
					for (int i = 1; i < 4; i++)
						byTrumpCount += g_byTrumpCount[i];
					if (byTrumpCount >= 2)
						_bitmap = m_pRPlayer[0]->bitmap[0];
				}
			}
			byGrade = TblBitIndex[(_bitmap & -_bitmap) % 37];
			return TYPE_SPADE;
		}
	}

	//���ܵ����Է����ٵ�
	for (int i = 1; i < 4; i++)
	{
		if (g_byTrumpCount[i] > g_byTrumpCount[0])
		{
			//���ƴ��»��㹻�������Ʊ�ɱ����ȻҪ���������Ƿֲ�6�š�7�š�2�š�3��
			if (g_byTrumpCount[0] >= 4) //���������������Ƶ����
			{
				word _bitmap = m_pRPlayer[0]->bitmap[0];
				if ((_bitmap & ~MASK_5TK) != 0)
				{
					_bitmap &= ~MASK_5TK;
					if ((_bitmap & _bitmap - 1) == 0) //�������ĵ���
					{
						byte byTrumpCount = 0;
						for (int i = 1; i < 4; i++)
							byTrumpCount += g_byTrumpCount[i];
						if (byTrumpCount >= 2)
							_bitmap = m_pRPlayer[0]->bitmap[0];
					}
				}
				byGrade = TblBitIndex[(_bitmap & -_bitmap) % 37];
				return TYPE_SPADE;
			}
			else
			{
				//�����ƵĴ���
				for (byte byRspType = TYPE_HEART; byRspType != TYPE_ERROR; byRspType++)
				{
					word _bitmap = m_pRPlayer[0]->bitmap[byRspType - 1];
					if (_bitmap != 0)
					{
						byGrade = _bitmap & 0xFF00 ? TblMaxGrade[_bitmap >> 8] + 8 : TblMaxGrade[_bitmap];
						return byRspType;
					}
				}
			}
		}
	}

	//�м�û������ʱ�����ȳ�����
	byte byRspFrom = TYPE_SPADE;
	if (m_pRPlayer[1]->bitmap[0] == 0 && m_pRPlayer[2]->bitmap[0] == 0 && m_pRPlayer[3]->bitmap[0] == 0)
		byRspFrom = TYPE_HEART;

	//��������ƣ�����������
	for (byte byRspType = byRspFrom; byRspType != TYPE_ERROR; byRspType++)
	{
		if (g_bBankerSweep[byRspType - 1])
		{
			word _bitmap = m_pRPlayer[0]->bitmap[byRspType - 1];

			//�Ӹպó����мҵ��ƿ�ʼ��
			word _other = m_pRPlayer[1]->bitmap[byRspType - 1]
						| m_pRPlayer[2]->bitmap[byRspType - 1]
						| m_pRPlayer[3]->bitmap[byRspType - 1];
			if (_other != 0)
			{
				byte _grade = _other & 0xFF00 ? TblMaxGrade[_other >> 8] + 8 : TblMaxGrade[_other];
				if (_grade == 12)
					_grade = 11;
				_bitmap &= ~((1 << (_grade + 1)) - 1); //�պó����мҵ���
			}
			byGrade = TblBitIndex[(_bitmap & -_bitmap) % 37];
			return byRspType;
		}
	}

	//�������ƣ������ĸ���
	for (byte byRspType = byRspFrom; byRspType != TYPE_ERROR; byRspType++)
	{
		word _bitmap = m_pRPlayer[0]->bitmap[byRspType - 1];
		if (WORD_BIT_COUNT(_bitmap) >= 3)
		{
			byGrade = GetMiddleGrade(_bitmap);
			return byRspType;
		}
	}

	//�������ƣ�������
	for (byte byRspType = byRspFrom; byRspType != TYPE_ERROR; byRspType++)
	{
		word _bitmap = m_pRPlayer[0]->bitmap[byRspType - 1];
		if (_bitmap != 0)
		{
			byGrade = GetMiddleGrade(_bitmap);
			return byRspType;
		}
	}

	//����ѡ��
	return m_pRPlayer[0]->MakeCards(TYPE_GVP, byGrade);
}

byte CRCrack::BankerFollowCard(byte &byGrade)
{
	//��ʼ��˼������
	InitTrumpCount();
	InitEscapeCard();
	InitBankerSweep();
	InitMaxGrade();
	InitMaxScore();

	if (m_pRPlayer[0]->bitmap[Z.byType - 1] != 0)
	{
		word _bitmap = m_pRPlayer[0]->bitmap[Z.byType - 1];
		if (g_byFriendScore >= 15 || Z.byCurr5TK >= 10) //�мҳ���15�ֻ����Ѿ�����10��
		{
			if (Z.byChair1 == 1) //�����Ѿ�����
			{
				if (g_byChairM0123 == 0) //ׯ�Ҵ�˵���������ͬ��ɫ
				{
					_bitmap &= TblContraryMask[Z.byGradeM];
					if ((_bitmap & MASK_5TK) != 0) //�з��ȳ�����
						_bitmap &= MASK_5TK;
				}
				else //�мҴ�
				{
					if ((_bitmap & ~MASK_5TK) != 0) //���ȳ����������С��
						_bitmap &= ~MASK_5TK;
				}
			}
			else //�����м�û�г���
			{
				if (Z.byTypeM == Z.byType) //�ܹ������Ѿ���������
				{
					if (_bitmap & TblContraryMask[Z.byGradeM])
						_bitmap &= TblContraryMask[Z.byGradeM];
				}
				if ((_bitmap & ~MASK_5TK) != 0) //���ȳ����������С��
					_bitmap &= ~MASK_5TK;
			}
		}
		else //�ֲ���
		{
			if (Z.byChair1 == 1) //�����Ѿ�����
			{
				if (g_byChairM0123 == 0) //ׯ�Ҵ�˵���������ͬ��ɫ
				{
					_bitmap &= TblContraryMask[Z.byGradeM];
					if ((_bitmap & MASK_5TK) != 0) //�з��ȳ�����
						_bitmap &= MASK_5TK;
				}
				else //�мҴ�
				{
					if ((_bitmap & ~MASK_5TK) != 0) //���ȳ����������С��
						_bitmap &= ~MASK_5TK;
				}
			}
			else
			{
				if (_bitmap & TblContraryMask[Z.byGradeM])
					_bitmap &= TblContraryMask[Z.byGradeM];
				if ((_bitmap & ~MASK_5TK) != 0) //���ȳ����������С��
					_bitmap &= ~MASK_5TK;
			}
		}
		byGrade = TblBitIndex[(_bitmap & -_bitmap) % 37];
		return Z.byType;
	}
	else
	{
		if (g_bCanTrumpKill)
		{
			word _bitmap = m_pRPlayer[0]->bitmap[0];
			if (Z.byTypeM != TYPE_SPADE
				&& g_tMaxGrade[1].byType != TYPE_SPADE
				&& g_tMaxGrade[2].byType != TYPE_SPADE
				&& g_tMaxGrade[3].byType != TYPE_SPADE) //����ɱ
			{
				if (_bitmap & MASK_5TK)
					_bitmap &= MASK_5TK;
				byGrade = TblBitIndex[(_bitmap & -_bitmap) % 37];
				return TYPE_SPADE;
			}
			if (Z.byCurr5TK >= 15) //�Ѿ���15�����ϣ����Ҳ�������ɱ
			{
				if (Z.byTypeM == TYPE_SPADE)
				{
					if (_bitmap & TblContraryMask[Z.byGradeM])
						_bitmap &= TblContraryMask[Z.byGradeM];
				}
				byGrade = TblBitIndex[(_bitmap & -_bitmap) % 37];
				return TYPE_SPADE;
			}
		}

		//���ʺ�������ɱ
		for (byte byRspType = TYPE_HEART; byRspType != TYPE_ERROR; byRspType++)
		{
			if (!g_bEscapeCard[byRspType - 1])
				continue;

			word _bitmap = m_pRPlayer[0]->bitmap[byRspType - 1];

			//���ƣ������Ƿ�ҲҪ��
			byGrade = TblBitIndex[(_bitmap & -_bitmap) % 37];
			return byRspType;
		}
		for (byte byRspType = TYPE_HEART; byRspType != TYPE_ERROR; byRspType++)
		{
			word _bitmap = m_pRPlayer[0]->bitmap[byRspType - 1];
			if (_bitmap != 0)
			{
				byGrade = TblBitIndex[(_bitmap & -_bitmap) % 37];
				return byRspType;
			}
		}
	}

	//����ѡ��
	byte byRspType = m_pRPlayer[0]->MakeCards(Z.byType, byGrade);
	return byRspType;
}

void CRCrack::InitScoreBitmap()
{
	memset(g_wScoreBitmap, 0, sizeof(g_wScoreBitmap));
	for (byte byRspType = TYPE_SPADE; byRspType != TYPE_ERROR; byRspType++)
	{
		word _bitmap = 0;
		for (int i = 0; i < 4; i++)
		{
			if (i == m_byBanker)
				continue;
			_bitmap |= m_pRPlayer[i]->bitmap[byRspType - 1];
		}
		word v = m_pRPlayer[m_byBanker]->bitmap[byRspType - 1];
		if (v & v - 1)
			_bitmap |= v;
		_bitmap &= MASK_5TK;
		if (_bitmap == 0) //���Ż�ɫû�з�
			continue;
		g_wScoreBitmap[byRspType - 1] = _bitmap;
	}
}

byte CRCrack::PlayerFirstCard(byte &byGrade)
{
	//��ʼ������λͼ
	InitScoreBitmap();

	//ׯ��û������ѡ��ׯ��û�еĻ�ɫ
	if (m_pRPlayer[m_byBanker]->bitmap[0] == 0)
	{
		for (byte byRspType = TYPE_HEART; byRspType != TYPE_ERROR; byRspType++)
		{
			if (m_pRPlayer[0]->bitmap[byRspType - 1] == 0)
				continue;
			if (m_pRPlayer[m_byBanker]->bitmap[byRspType - 1] != 0) //ׯ���������ɫ
				continue;

			word _bitmap = m_pRPlayer[0]->bitmap[byRspType - 1];

			//�����ĸ��ƿ�ʼ
			byGrade = _bitmap & 0xFF00 ? TblMaxGrade[_bitmap >> 8] + 8 : TblMaxGrade[_bitmap];
			return byRspType;
		}
	}

	//����ѡ������û�еĻ�ɫ
	for (byte byRspType = TYPE_SPADE; byRspType != TYPE_ERROR; byRspType++)
	{
		if (m_pRPlayer[0]->bitmap[byRspType - 1] == 0)
			continue;
		if (g_wScoreBitmap[byRspType - 1] == 0) //û�зֿɼ�
			continue;

		word _bitmap = m_pRPlayer[0]->bitmap[byRspType - 1];
		for (int i = 1; i < 4; i++)
		{
			if (i == m_byBanker)
				continue;
			if (m_pRPlayer[i]->bitmap[byRspType - 1] == 0)
			{
				word v2 = g_wScoreBitmap[byRspType - 1];
				v2 ^= m_pRPlayer[m_byBanker]->bitmap[byRspType - 1] & MASK_5TK; //ȥ��ׯ�ҵķ���
				if ((_bitmap & MASK_5TK) == v2) //��ȫ���Լ���
				{
					if (v2 == 0)
						break;
					byGrade = TblBitIndex[(v2 & -v2) % 37];
				}
				else
				{
					//����չ�����ϼҡ��¼Ҳ�ͬ����
					byGrade = TblBitIndex[(_bitmap & -_bitmap) % 37];
				}
				return byRspType;
			}
		}
	}

	//�ϼ����⴦��ѡ��ׯ��û�еĻ�ɫ�����Ҷ���û�еĻ�ɫ
	if (m_byBanker == 1)
	{
		for (byte byRspType = TYPE_SPADE; byRspType != TYPE_ERROR; byRspType++)
		{
			if (m_pRPlayer[0]->bitmap[byRspType - 1] == 0)
				continue;
			if (m_pRPlayer[m_byBanker]->bitmap[byRspType - 1] != 0) //ׯ���������ɫ
				continue;

			//ѡ�����û�еĻ�ɫ
			word _bitmap = m_pRPlayer[0]->bitmap[byRspType - 1];
			for (int i = 1; i < 4; i++)
			{
				if (i == m_byBanker)
					continue;
				if (m_pRPlayer[i]->bitmap[byRspType - 1] == 0)
				{
					word v2 = g_wScoreBitmap[byRspType - 1];
					v2 ^= m_pRPlayer[m_byBanker]->bitmap[byRspType - 1] & MASK_5TK; //ȥ��ׯ�ҵķ���
					if ((_bitmap & MASK_5TK) == v2) //��ȫ���Լ���
					{
						if (v2 == 0)
							byGrade = TblBitIndex[(_bitmap & -_bitmap) % 37]; //��ʹ���Ƿ�ҲҪ������
						else
							byGrade = TblBitIndex[(v2 & -v2) % 37];
					}
					else
					{
						//����չ�����ϼҡ��¼Ҳ�ͬ����
						byGrade = TblBitIndex[(_bitmap & -_bitmap) % 37];
					}
					return byRspType;
				}
			}
		}
	}

	//�ϼ����⴦��ѡ��ׯ��û�еĻ�ɫ������ȫ�����д˻�ɫ
	if (m_byBanker == 1)
	{
		for (byte byRspType = TYPE_SPADE; byRspType != TYPE_ERROR; byRspType++)
		{
			if (m_pRPlayer[0]->bitmap[byRspType - 1] == 0)
				continue;
			if (g_wScoreBitmap[byRspType - 1] == 0) //û�зֿɼ�
				continue;
			if (m_pRPlayer[m_byBanker]->bitmap[byRspType - 1] != 0) //ׯ���������ɫ
				continue;

			//����ȫ�����д˻�ɫ
			word _bitmap = m_pRPlayer[0]->bitmap[byRspType - 1];
			word v2 = g_wScoreBitmap[byRspType - 1];
			v2 ^= m_pRPlayer[m_byBanker]->bitmap[byRspType - 1] & MASK_5TK; //ȥ��ׯ�ҵķ���
			if ((_bitmap & MASK_5TK) == v2) //��ȫ���Լ���
			{
				if (v2 == 0)
					break;
				byGrade = TblBitIndex[(v2 & -v2) % 37];
			}
			else
			{
				//����չ�����ϼҡ��¼Ҳ�ͬ����
				byGrade = TblBitIndex[(_bitmap & -_bitmap) % 37];
			}
			return byRspType;
		}
	}

	//�Լ����⴦��ѡ��ׯ��û�еĻ�ɫ �����Ҷ���û�еĻ�ɫ
	if (m_byBanker == 2)
	{
		for (byte byRspType = TYPE_SPADE; byRspType != TYPE_ERROR; byRspType++)
		{
			if (m_pRPlayer[0]->bitmap[byRspType - 1] == 0)
				continue;
			if (m_pRPlayer[m_byBanker]->bitmap[byRspType - 1] != 0) //ׯ���������ɫ
				continue;

			//ѡ�����û�еĻ�ɫ
			word _bitmap = m_pRPlayer[0]->bitmap[byRspType - 1];
			for (int i = 1; i < 4; i++)
			{
				if (i == m_byBanker)
					continue;
				if (m_pRPlayer[i]->bitmap[byRspType - 1] == 0)
				{
					word v2 = g_wScoreBitmap[byRspType - 1];
					v2 ^= m_pRPlayer[m_byBanker]->bitmap[byRspType - 1] & MASK_5TK; //ȥ��ׯ�ҵķ���
					if ((_bitmap & MASK_5TK) == v2) //��ȫ���Լ���
					{
						if (v2 == 0)
							byGrade = TblBitIndex[(_bitmap & -_bitmap) % 37]; //��ʹ���Ƿ�ҲҪ������
						else
							byGrade = TblBitIndex[(v2 & -v2) % 37];
					}
					else
					{
						//����չ�����ϼҡ��¼Ҳ�ͬ����
						byGrade = TblBitIndex[(_bitmap & -_bitmap) % 37];
					}
					return byRspType;
				}
			}
		}
	}

	//ѡ��ׯ���еĻ�ɫ��ׯ��Ψһ��
	for (byte byRspType = TYPE_SPADE; byRspType != TYPE_ERROR; byRspType++)
	{
		if (m_pRPlayer[0]->bitmap[byRspType - 1] == 0)
			continue;
		if (g_wScoreBitmap[byRspType - 1] == 0) //û�зֿɼ�
			continue;
		word v1 = m_pRPlayer[m_byBanker]->bitmap[byRspType - 1];
		if (v1 == 0) //ׯ��û�������ɫ
			continue;
		if ((v1 & v1 - 1) != 0) //ׯ�Ҳ���Ψһ��
			continue;

		word _bitmap = m_pRPlayer[0]->bitmap[byRspType - 1];
		if (_bitmap > v1) //�Լ���ׯ�Ҵ�
		{
			byGrade = _bitmap & 0xFF00 ? TblMaxGrade[_bitmap >> 8] + 8 : TblMaxGrade[_bitmap];
			return byRspType;
		}
		else
		{
			for (int i = 1; i < 4; i++)
			{
				if (i == m_byBanker)
					continue;
				if (m_pRPlayer[i]->bitmap[byRspType - 1] > v1) //���ѱ�ׯ�Ҵ�
				{
					byGrade = _bitmap & 0xFF00 ? TblMaxGrade[_bitmap >> 8] + 8 : TblMaxGrade[_bitmap];
					return byRspType;
				}
			}
		}
	}

	//�Լҡ��¼����⴦��ѡ��ׯ���еĻ�ɫ
	if (m_byBanker == 2 || m_byBanker == 3)
	{
		for (byte byRspType = TYPE_SPADE; byRspType != TYPE_ERROR; byRspType++)
		{
			if (m_pRPlayer[0]->bitmap[byRspType - 1] == 0)
				continue;
			if (g_wScoreBitmap[byRspType - 1] == 0) //û�зֿɼ�
				continue;
			if (m_pRPlayer[m_byBanker]->bitmap[byRspType - 1] == 0) //ׯ��û�������ɫ
				continue;

			word _bitmap = m_pRPlayer[0]->bitmap[byRspType - 1];
			for (int i = 1; i < 4; i++)
			{
				if (i == m_byBanker)
					continue;
				if (m_pRPlayer[i]->bitmap[byRspType - 1] == 0)
				{
					word v2 = g_wScoreBitmap[byRspType - 1];
					v2 ^= m_pRPlayer[m_byBanker]->bitmap[byRspType - 1] & MASK_5TK; //ȥ��ׯ�ҵķ���
					if ((_bitmap & MASK_5TK) == v2) //��ȫ���Լ���
					{
						if (v2 == 0)
							break;
						byGrade = TblBitIndex[(v2 & -v2) % 37];
					}
					else
					{
						//����չ�����ϼҡ��¼Ҳ�ͬ����
						byGrade = TblBitIndex[(_bitmap & -_bitmap) % 37];
					}
					return byRspType;
				}
			}
		}
	}

	//ѡ���зֵĻ�ɫ
	for (byte byRspType = TYPE_SPADE; byRspType != TYPE_ERROR; byRspType++)
	{
		if (m_pRPlayer[0]->bitmap[byRspType - 1] == 0)
			continue;
		if (g_wScoreBitmap[byRspType - 1] == 0) //û�зֿɼ�
			continue;

		word _bitmap = m_pRPlayer[0]->bitmap[byRspType - 1];
		for (int i = 1; i < 4; i++)
		{
			if (i == m_byBanker)
				continue;
			if (m_pRPlayer[i]->bitmap[byRspType - 1] == 0)
			{
				word v2 = g_wScoreBitmap[byRspType - 1];
				v2 ^= m_pRPlayer[m_byBanker]->bitmap[byRspType - 1] & MASK_5TK; //ȥ��ׯ�ҵķ���
				if ((_bitmap & MASK_5TK) == v2) //��ȫ���Լ���
				{
					if (v2 == 0)
						break;
					byGrade = TblBitIndex[(v2 & -v2) % 37];
				}
				else
				{
					//����չ�����ϼҡ��¼Ҳ�ͬ����
					byGrade = TblBitIndex[(_bitmap & -_bitmap) % 37];
				}
				return byRspType;
			}
		}
	}

	//����ѡ��
	for (byte byRspType = TYPE_SPADE; byRspType != TYPE_ERROR; byRspType++)
	{
		if (m_pRPlayer[0]->bitmap[byRspType - 1] == 0)
			continue;

		word _bitmap = m_pRPlayer[0]->bitmap[byRspType - 1];
		byGrade = TblBitIndex[(_bitmap & -_bitmap) % 37];
		return byRspType;
	}
	return TYPE_GVP;
}

byte CRCrack::PlayerFollowCard(byte &byGrade)
{
	byte byRspType;
	if (g_byChairM0123 == m_byBanker) //�ļ����ׯ�Ҵ�
	{
		if (Z.byChair1 <= m_byBanker) //ׯ���Ѿ�����
		{
			word wBitmapBak[4];
			memcpy(wBitmapBak, m_pRPlayer[0]->bitmap, sizeof(word [4]));

			//ֻ���ׯ��С�ĸ���
			for (byte bySuit = TYPE_HEART; bySuit != TYPE_ERROR; bySuit++)
			{
				word _banker = m_pRPlayer[m_byBanker]->bitmap[bySuit - 1];
				byte _bankergrade = _banker & 0xFF00 ? TblMaxGrade[_banker >> 8] + 8 : TblMaxGrade[_banker];
				m_pRPlayer[0]->bitmap[bySuit - 1] &= (1 << _bankergrade) - 1;
			}

			//���ӷ֣�����
			byRspType = m_pRPlayer[0]->MakeWithoutScore(Z.byType, byGrade);
			if (byRspType == TYPE_GVP)
				byRspType = m_pRPlayer[0]->MakeByLittle(Z.byType, byGrade);

			memcpy(m_pRPlayer[0]->bitmap, wBitmapBak, sizeof(word [4])); //��ԭ

			//�������ߴ���ĵ���ѡ��
			if (byRspType == TYPE_GVP
				|| m_pRPlayer[0]->bitmap[Z.byType - 1] != 0 && byRspType != Z.byType)
			{
				//���ӷ֣�����
				byRspType = m_pRPlayer[0]->MakeWithoutScore(Z.byType, byGrade);
				if (byRspType == TYPE_GVP)
					byRspType = m_pRPlayer[0]->MakeByLittle(Z.byType, byGrade);
			}
		}
		else
		{
			if (!m_pRPlayer[m_byBanker]->HasCard(Z.byType) //ׯ����Ҫ��ɱ
				&& g_byFriendScore == 0) //�����޷�
			{
				//�ӷֱ�ɱ
				byRspType = m_pRPlayer[0]->MakeWithScore(Z.byType, byGrade);
				if (byRspType == TYPE_GVP)
					byRspType = m_pRPlayer[0]->MakeByLittle(Z.byType, byGrade);
			}
			else
			{
				word wBitmapBak[4];
				memcpy(wBitmapBak, m_pRPlayer[0]->bitmap, sizeof(word [4]));

				//ֻ���ׯ��С�ĸ���
				for (byte bySuit = TYPE_HEART; bySuit != TYPE_ERROR; bySuit++)
				{
					word _banker = m_pRPlayer[m_byBanker]->bitmap[bySuit - 1];
					byte _bankergrade = _banker & 0xFF00 ? TblMaxGrade[_banker >> 8] + 8 : TblMaxGrade[_banker];
					m_pRPlayer[0]->bitmap[bySuit - 1] &= (1 << _bankergrade) - 1;
				}

				//���ӷ֣�����
				byRspType = m_pRPlayer[0]->MakeWithoutScore(Z.byType, byGrade);
				if (byRspType == TYPE_GVP)
					byRspType = m_pRPlayer[0]->MakeByLittle(Z.byType, byGrade);

				memcpy(m_pRPlayer[0]->bitmap, wBitmapBak, sizeof(word [4])); //��ԭ

				//�������ߴ���ĵ���ѡ��
				if (byRspType == TYPE_GVP
					|| m_pRPlayer[0]->bitmap[Z.byType - 1] != 0 && byRspType != Z.byType)
				{
					//���ӷ֣�����
					byRspType = m_pRPlayer[0]->MakeWithoutScore(Z.byType, byGrade);
					if (byRspType == TYPE_GVP)
						byRspType = m_pRPlayer[0]->MakeByLittle(Z.byType, byGrade);
				}
			}
		}
	}
	else
	{
		if (Z.byChair1 > m_byBanker //ׯ�һ�û�г���
			|| Z.byChairM == m_byBanker && g_bICanOverBanker && g_byFriendScore != 0) //�Լ��ܱ�ׯ�Ҵ󣬶����ܼӷ�
		{
			if (g_bHadOverBanker || g_bOverBankerWithScore) //���Ѽӷֵ��Ʊ�ׯ�Ҵ�
			{
				//�ӷ�
				byRspType = m_pRPlayer[0]->MakeWithScore(Z.byType, byGrade);
				if (byRspType == TYPE_GVP)
					byRspType = m_pRPlayer[0]->MakeByLittle(Z.byType, byGrade);
			}
			else
			{
				//����ׯ�Ҵ����
				byte byTypeM = Z.byChair1 > m_byBanker ? g_tMaxGrade[m_byBanker].byType : Z.byTypeM;
				byte byGradeM = Z.byChair1 > m_byBanker ? g_tMaxGrade[m_byBanker].byGrade : Z.byGradeM;

				byRspType = m_pRPlayer[0]->MakeJustOver(Z.byType, byGrade, byTypeM, byGradeM);

				//�󲻹�����С��
				if (byRspType == TYPE_GVP)
					byRspType = m_pRPlayer[0]->MakeByLittle(Z.byType, byGrade);
			}
		}
		else
		{
			//�ӷ�
			byRspType = m_pRPlayer[0]->MakeWithScore(Z.byType, byGrade);
			if (byRspType == TYPE_GVP)
				byRspType = m_pRPlayer[0]->MakeByLittle(Z.byType, byGrade);
		}
	}
	return byRspType;
}

void CRCrack::Think()
{
	//�ӱ���Ĳ����ָ�
	RestoreWith(m_tCrackParam);

	//��ʼ��˼������
	InitMaxGrade();
	InitMaxScore();
	InitOverBanker();

	//����˼�������
	byte byRspType;
	byte byGrade;
	if (m_byBanker == 0)
	{
		byRspType = Z.byChair1 == 0 ? BankerFirstCard(byGrade)
									: BankerFollowCard(byGrade);
	}
	else
	{
		byRspType = Z.byChair1 == 0 ? PlayerFirstCard(byGrade)
									: PlayerFollowCard(byGrade);
	}

	//�����ڽ����
	m_tAction[1].byRspType = byRspType;
	m_tAction[1].byGrade = byGrade;
}
