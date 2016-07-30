#include "StdAfx.h"

static short		 g_shLeft;
static short		 g_shAdvance;
static short		 g_shTop;
static short		 g_shHeight;

static CLineTag		 g_tLineTag;

static int GetChar(const TCHAR *pIn, char *chOut, int nPos)
{
	*chOut = pIn[nPos];
	return nPos + 1;
}

static int GetShort(const TCHAR *pIn, short *shOut, int nPos)
{
	*shOut = (pIn[nPos] << 8) + pIn[nPos + 1];
	return nPos + 2;
}

static int GetColor(const TCHAR *pIn, COLORREF *color, int nPos)
{
	*color = BGRA(pIn[nPos], pIn[nPos + 1], pIn[nPos + 2], pIn[nPos + 3]);
	return nPos + 4;
}

static int GetString(const TCHAR *pIn, TCHAR *pOut, int nPos, int L)
{
	wcscpy_s(pOut, L, pIn + nPos);
	return nPos + L;
}

static int GetFamily(const TCHAR *pIn, char *str, int nPos, int L)
{
	TCHAR pOut[LF_FACESIZE];

	wcscpy_s(pOut, L + 1, pIn + nPos);
	WideCharToMultiByte(CP_ACP, 0, pOut, -1, str, LF_FACESIZE, NULL, NULL);

	return nPos + L;
}

CTextBar::CTextBar(bool bMultiLine, int nHeight, CPoint xDrawPos, CSize xSize, int nDepth)
			: CPanel(NULL, xDrawPos, xSize, nDepth)
{
	m_bMultiLine = bMultiLine;
	m_nHeight = nHeight;
	m_nBaseLine = (m_nHeight + 9) / 10;

	m_pImage = NULL;
	m_xPencil.x = 0;
	m_xPencil.y = 0;
}

CTextBar::~CTextBar()
{
	delete m_pImage;
	m_pImage = NULL;
}

void CTextBar::Set(bool bMultiLine, int nHeight, CPoint xDrawPos, CSize xSize, int nDepth)
{
	m_bMultiLine = bMultiLine;
	m_nHeight = nHeight;
	m_nBaseLine = (m_nHeight + 9) / 10;

	m_xPencil.x = 0;
	m_xPencil.y = 0;

	m_xDrawPos = xDrawPos;
	m_xSize = xSize;
	m_nDepth = nDepth;

	AllocImage(m_xSize.cx, m_xSize.cy);
}

bool CTextBar::Initial()
{
	m_pImage = new CImage(m_xSize.cx, m_xSize.cy);
	return m_pImage != NULL;
}

bool CTextBar::Release()
{
	CPanel::Release();

	delete m_pImage;
	m_pImage = NULL;

	return true;
}

void CTextBar::SetRotate(FT_Face &face)
{
	float fAngle = 10.0;
	float fRadian = (float)(fAngle / 180.0 * 3.14159);

	FT_Matrix matrix;
	matrix.xx = (FT_Fixed)( cos(fRadian) * 0x10000L);
	matrix.xy = (FT_Fixed)(-sin(fRadian) * 0x10000L);
	matrix.yx = (FT_Fixed)( sin(fRadian) * 0x10000L);
	matrix.yy = (FT_Fixed)( cos(fRadian) * 0x10000L);

	FT_Set_Transform(face, &matrix, NULL);
}

FTC_SBit CTextBar::GetBitmap(TCHAR tch, FTC_ScalerRec &tScalerRec)
{
	FTC_Scaler		 pSelectedIC;
	FTC_SBit		 bitmap;

	int glyph_index = FTC_CMapCache_Lookup(g_pZText->GetCMapCache(), tScalerRec.face_id, -1, tch);
	//�Ҳ���ʱ�Կո����
	if (glyph_index == 0)
		glyph_index = FTC_CMapCache_Lookup(g_pZText->GetCMapCache(), tScalerRec.face_id, -1, _T(' '));
	if (glyph_index)
		pSelectedIC = &tScalerRec;

	FTC_SBitCache_LookupScaler(g_pZText->GetSBitCache(), pSelectedIC, FT_LOAD_DEFAULT,
							   glyph_index, &bitmap, (FTC_Node *)NULL);

	return bitmap;
}

void CTextBar::AdjustBitmap(FTC_SBit bitmap)
{
	g_shLeft = bitmap->left;
	g_shAdvance = bitmap->xadvance;

	//�հ׷��ŵ���ΪĬ��ֵ
	if (bitmap->width == 0xFF)
		g_shAdvance = g_pZText->GetSpaceWidth();

	//���������
	if (g_shLeft < 0)
		g_shLeft = 0;

	//��ת����
	if (bitmap->width != 0xFF)
	{
		if (g_shLeft + bitmap->width > g_shAdvance)
		{
			g_shAdvance = g_shLeft + bitmap->width;
			g_shLeft >>= 1;
		}
	}

	g_shTop = bitmap->top;
	g_shHeight = bitmap->height;

	if (m_nHeight != 0)
	{
		//�����ϳ���
		if (bitmap->height - m_nBaseLine > g_shTop)
			g_shTop = bitmap->height - m_nBaseLine;
		if (g_shTop > m_nHeight - m_nBaseLine)
			g_shTop = m_nHeight - m_nBaseLine;

		//�����߶�
		if (m_nHeight < g_shHeight)
			g_shHeight = m_nHeight;
	}
}

bool CTextBar::PenAdvance(CRect &rect, int nXSpace, int nYSpace, FTC_SBit bitmap)
{
	if (m_xPencil.x + g_shAdvance + nXSpace > rect.Width())
	{
		if (!m_bMultiLine)
			return false;
		m_xPencil.x = 0;
		m_xPencil.y += m_nHeight + nYSpace;
		if (m_xPencil.y + m_nBaseLine > rect.Height())
			return false;
	}
	return true;
}

void CTextBar::FillGlyph(CRect &rect, int nWidth, int nHeight, int nYBearing, COLORREF bkcolor)
{
	for (int y = m_xPencil.y - nYBearing; y < m_xPencil.y - nYBearing + nHeight && y < rect.bottom; y++)
	{
		int *p = (int *)m_pImage->GetBits(m_xPencil.x, y);
		for (int x = m_xPencil.x; x < m_xPencil.x + nWidth && x < rect.right; x++)
			*p++ = bkcolor;
	}
}

void CTextBar::_DrawMonoGlyph(FTC_SBit bitmap, bool transparent, COLORREF color)
{
	for (int y = 0; y < g_shHeight; y++)
	{
		int *p = (int *)m_pImage->GetBits(m_xPencil.x + g_shLeft, m_xPencil.y - g_shTop + y);
		const byte *q = bitmap->buffer + y * bitmap->pitch;

		if (transparent)
		{
			for (int x = 0; x < bitmap->width; x++)
			{
				if (q[x >> 3] & (0x80 >> (x & 7)))
					*p++ = color; //ǰ��ɫ
				else 
					*p++ = 0; //ͨ͸
			}
		}
		else
		{
			for (int x = 0; x < bitmap->width; x++)
			{
				if (q[x >> 3] & (0x80 >> (x & 7)))
					*p++ = color; //ǰ��ɫ
				else 
					*p++; //����ɫ
			}
		}
	}
}

void CTextBar::_DrawGrayGlyph(FTC_SBit bitmap, bool transparent, COLORREF color)
{
	byte b = GetBValue(color);
	byte g = GetGValue(color);
	byte r = GetRValue(color);

	for (int y = 0; y < g_shHeight; y++)
	{
		byte *p = (byte *)m_pImage->GetBits(m_xPencil.x + g_shLeft, m_xPencil.y - g_shTop + y);
		const byte *q = bitmap->buffer + y * bitmap->pitch;

		if (transparent)
		{
			for (int x = 0; x < bitmap->width; x++)
			{
				byte gray = *q++;
				*p++ = b;
				*p++ = g;
				*p++ = r;
				*p++ = gray;
			}
		}
		else
		{
			for (int x = 0; x < bitmap->width; x++)
			{
				//��͸��ʱ��ָ������ɫ�ϳ�
				byte gray = *q++;
				alpha_composite(*p++, b, gray, *p);
				alpha_composite(*p++, g, gray, *p);
				alpha_composite(*p++, r, gray, *p);
				*p++;
			}
		}
	}
}

void CTextBar::ClearText()
{
	m_xPencil.x = 0;
	m_xPencil.y = 0;

	ClearImage();
	g_pZ2coo->Redraw(this);
}

void CTextBar::DrawGlyph(FTC_SBit bitmap, bool transparent, COLORREF color)
{
	if (bitmap->format == FT_PIXEL_MODE_MONO)
	{
		_DrawMonoGlyph(bitmap, transparent, color);
	}
	else
	{
		_DrawGrayGlyph(bitmap, transparent, color);
	}
}

int CTextBar::ExtOutText(const TCHAR *lpText, int nLength, CLineTag *pLineTag)
{
	m_pImage->Clear();

	//��ȡ��������
	CRect rect;
	GetRect(&rect);

	//��ʼ�����и�ʮ��֮�ŵ�λ��
	m_xPencil.x = 0;
	m_xPencil.y = m_nHeight - m_nBaseLine;

	//�ּ�࣬�м��
	int nXSpace = 0;
	int nYSpace = 0;

	int nPos = pLineTag->nTagPos;

	short T, L;
	while (nPos < nLength)
	{
		nPos = GetShort(lpText, &T, nPos);
		nPos = GetShort(lpText, &L, nPos);
		switch (T)
		{
		case TF_TRANSPARENT:
			nPos = GetChar(lpText, (char *)&pLineTag->transparent, nPos);
			break;
		case TF_COLOR:
			nPos = GetColor(lpText, &pLineTag->color, nPos);
			break;
		case TF_BKCOLOR:
			nPos = GetColor(lpText, &pLineTag->bkcolor, nPos);
			break;
		case TF_X_SPACE:
			nPos = GetShort(lpText, (short *)&nXSpace, nPos);
			break;
		case TF_Y_SPACE:
			nPos = GetShort(lpText, (short *)&nYSpace, nPos);
			break;
		case TF_TEXT:
			FTC_SBit bitmap;
			for (int i = max(nPos, pLineTag->nTagSkip); i < nPos + L; i++)
			{
				if (lpText[i] == _T('\n'))
				{
					//�������з�
					pLineTag->nTagSkip = i + 1;

					//�ֱ�λ��������һ��
					if (!m_bMultiLine)
						goto Exit;
					m_xPencil.x = 0;
					m_xPencil.y += m_nHeight + nYSpace;
					if (m_xPencil.y + m_nBaseLine > rect.Height())
						goto Exit;
					continue;
				}
				bitmap = GetBitmap(lpText[i], pLineTag->tScalerRec);
				AdjustBitmap(bitmap);

				//�ж��Ƿ񳬹�һ��
				if (!PenAdvance(rect, nXSpace, nYSpace, bitmap))
				{
					//��������ǰ�ַ�
					pLineTag->nTagSkip = i;
					goto Exit;
				}

				if (!pLineTag->transparent)
				{
					//��͸��ʱ����䱳��ɫ
					FillGlyph(rect, g_shAdvance + nXSpace, m_nHeight, m_nHeight - m_nBaseLine, pLineTag->bkcolor);
				}
				DrawGlyph(bitmap, pLineTag->transparent, pLineTag->color);
				m_xPencil.x += g_shAdvance + nXSpace;
			}
			nPos += L; //L���ַ�
			pLineTag->nTagPos = nPos; //���½���λ��
			break;
		default:
			nPos += L;
			break;
		}
	}
	pLineTag->nTagSkip = nLength; //�������

Exit:
	g_pZ2coo->Redraw(this);

	m_xPencil.y += m_nBaseLine;
	return m_xPencil.y;
}

int CTextBar::EnhOutText(const TCHAR *lpText, int nLength, CLineTag *pLineTag)
{
	m_pImage->Clear();

	//��ȡ��������
	CRect rect;
	GetRect(&rect);

	m_xPencil.x = 0;
	m_xPencil.y = 0;

	//���Ƶ�ǰ���б��
	memcpy(&g_tLineTag, pLineTag, sizeof(CLineTag));

	//��nTagPos����ʼ����������nTagSkip���ַ�
	int nTagNext;
	int nTagCurr;

	//Ԥ�����е����߶ȣ���׼λ�ã�������о��룬�ַ����
	int nMaxHeight;
	int nYBearing;
	int nMaxBottom;
	int nTotalWidth;

	//�ּ�࣬�м��
	int nXSpace = 0;
	int nYSpace = 0;

	short T, L;
	while (pLineTag->nTagPos < nLength)
	{
		nMaxHeight = 0;
		nYBearing = 0;
		nMaxBottom = 0;
		nTotalWidth = 0;

		nTagCurr = pLineTag->nTagSkip; //��¼��ǰ����λ��

		int nPos = pLineTag->nTagPos;
		while (nPos < nLength)
		{
			nTagNext = nPos; //��һ�д�nTagNext����ʼ����

			nPos = GetShort(lpText, &T, nPos);
			nPos = GetShort(lpText, &L, nPos);
			switch (T)
			{
			case TF_TRANSPARENT:
			case TF_COLOR:
			case TF_BKCOLOR:
				nPos += L; //��������
				break;
			case TF_WIDTH:
				nPos = GetShort(lpText, (short *)&pLineTag->tScalerRec.width, nPos);
				break;
			case TF_HEIGHT:
				nPos = GetShort(lpText, (short *)&pLineTag->tScalerRec.height, nPos);
				break;
			case TF_FAMILY:
				nPos = GetFamily(lpText, pLineTag->szFaceName, nPos, L);
				pLineTag->UpdateFaceID();
				break;
			case TF_STYLE:
				nPos = GetChar(lpText, (char *)&pLineTag->byStyle, nPos);
				pLineTag->UpdateFaceID();
				break;
			case TF_X_SPACE:
				nPos = GetShort(lpText, (short *)&nXSpace, nPos);
				break;
			case TF_Y_SPACE:
				nPos = GetShort(lpText, (short *)&nYSpace, nPos);
				break;
			case TF_TEXT:
				for (int i = max(nPos, pLineTag->nTagSkip); i < nPos + L; i++)
				{
					if (lpText[i] == _T('\n'))
					{
						//�������з�
						pLineTag->nTagSkip = i + 1;

						//����ʹ�õ�ǰ�и�
						if (nMaxHeight == 0)
						{
							nMaxHeight = pLineTag->tScalerRec.height;
							nMaxBottom = 0;
							nYBearing = pLineTag->tScalerRec.height;
						}
						goto Draw;
					}
					FTC_SBit bitmap = GetBitmap(lpText[i], pLineTag->tScalerRec);
					AdjustBitmap(bitmap);

					//�ж��Ƿ񳬹�һ��
					if (nTotalWidth + g_shAdvance + nXSpace > rect.Width())
					{
						//��������ǰ�ַ�
						pLineTag->nTagSkip = i;
						goto Draw;
					}

					nTotalWidth += g_shAdvance + nXSpace; //����ۼ�
					if (nMaxHeight < bitmap->height) //���߶�
						nMaxHeight = bitmap->height;
					if (nYBearing < bitmap->top) //��׼λ��
						nYBearing = bitmap->top;
					if (nMaxBottom < bitmap->height - bitmap->top) //������о���
						nMaxBottom = bitmap->height - bitmap->top;
				}
				nPos += L; //ǰ��L���ַ�
				break;
			}
		}
		pLineTag->nTagSkip = nLength; //�������
		nTagNext = nLength;

Draw:
		m_xSize.cy = m_xPencil.y + nYBearing + nMaxBottom;

		//ͼ�񲻹���ʱ�½�һ��ͼ��
		if (m_pImage->Height() < m_xSize.cy)
		{
			AllocImage(m_xSize.cx, m_xSize.cy);
			ClearImage();
		}
		m_xPencil.y += nYBearing; //FreeType�ĸֱ�λ��

		nPos = pLineTag->nTagPos;
		while (nPos < nLength)
		{
			nPos = GetShort(lpText, &T, nPos);
			nPos = GetShort(lpText, &L, nPos);
			switch (T)
			{
			case TF_TRANSPARENT:
				nPos = GetChar(lpText, (char *)&g_tLineTag.transparent, nPos);
				break;
			case TF_COLOR:
				nPos = GetColor(lpText, &g_tLineTag.color, nPos);
				break;
			case TF_BKCOLOR:
				nPos = GetColor(lpText, &g_tLineTag.bkcolor, nPos);
				break;
			case TF_WIDTH:
				nPos = GetShort(lpText, (short *)&g_tLineTag.tScalerRec.width, nPos);
				break;
			case TF_HEIGHT:
				nPos = GetShort(lpText, (short *)&g_tLineTag.tScalerRec.height, nPos);
				break;
			case TF_FAMILY:
				nPos = GetFamily(lpText, g_tLineTag.szFaceName, nPos, L);
				g_tLineTag.UpdateFaceID();
				break;
			case TF_STYLE:
				nPos = GetChar(lpText, (char *)&g_tLineTag.byStyle, nPos);
				g_tLineTag.UpdateFaceID();
				break;
			case TF_X_SPACE:
				nPos = GetShort(lpText, (short *)&nXSpace, nPos);
				break;
			case TF_Y_SPACE:
				nPos = GetShort(lpText, (short *)&nYSpace, nPos);
				break;
			case TF_TEXT:
				//����һ���������ɵ��ַ�
				FTC_SBit bitmap;
				for (int i = max(nPos, nTagCurr); i < nPos + L; i++)
				{
					if (i == pLineTag->nTagSkip)
						goto Next;
					if (lpText[i] == _T('\n'))
						goto Next;

					bitmap = GetBitmap(lpText[i], g_tLineTag.tScalerRec);
					AdjustBitmap(bitmap);

					m_xPencil.x += nXSpace; //ƫ���ּ��
					if (!g_tLineTag.transparent)
					{
						//��͸��ʱ����䱳��ɫ
						FillGlyph(rect, g_shAdvance + nXSpace, nYBearing + nMaxBottom, nYBearing, g_tLineTag.bkcolor);
					}
					DrawGlyph(bitmap, g_tLineTag.transparent, g_tLineTag.color);
					m_xPencil.x += g_shAdvance;
				}
				nPos += L; //ǰ��L���ַ�
				break;
			}
		}
Next:
		m_xPencil.x = 0;
		m_xPencil.y += nMaxBottom + nYSpace; //��һ�еĶ���

		pLineTag->nTagPos = nTagNext;

		if (!m_bMultiLine)
			goto Exit;
	}

Exit:
	g_pZ2coo->Redraw(this);

	return m_xPencil.y;
}

int CTextBar::ExtScanText(const TCHAR *lpText, int nLength, CLineTag *pLineTag)
{
	return m_xPencil.y;
}

int CTextBar::EnhScanText(const TCHAR *lpText, int nLength, CLineTag *pLineTag)
{
	//��ȡ��������
	CRect rect;
	GetRect(&rect);

	m_xPencil.x = 0;
	m_xPencil.y = 0;

	//���Ƶ�ǰ���б��
	memcpy(&g_tLineTag, pLineTag, sizeof(CLineTag));

	//��nTagPos����ʼ����������nTagSkip���ַ�
	int nTagNext;

	//Ԥ�����е����߶ȣ���׼λ�ã�������о��룬�ַ����
	int nYBearing;
	int nMaxBottom;
	int nTotalWidth;

	short T, L;
	while (pLineTag->nTagPos < nLength)
	{
		nYBearing = 0;
		nMaxBottom = 0;
		nTotalWidth = 0;

		int nPos = pLineTag->nTagPos;
		while (nPos < nLength)
		{
			nTagNext = nPos; //��һ�д�nTagNext����ʼ����

			nPos = GetShort(lpText, &T, nPos);
			nPos = GetShort(lpText, &L, nPos);
			switch (T)
			{
			case TF_TRANSPARENT:
			case TF_COLOR:
			case TF_BKCOLOR:
				nPos += L; //��������
				break;
			case TF_WIDTH:
				nPos = GetShort(lpText, (short *)&pLineTag->tScalerRec.width, nPos);
				break;
			case TF_HEIGHT:
				nPos = GetShort(lpText, (short *)&pLineTag->tScalerRec.height, nPos);
				break;
			case TF_FAMILY:
				nPos = GetFamily(lpText, pLineTag->szFaceName, nPos, L);
				pLineTag->UpdateFaceID();
				break;
			case TF_STYLE:
				nPos = GetChar(lpText, (char *)&pLineTag->byStyle, nPos);
				pLineTag->UpdateFaceID();
				break;
			case TF_TEXT:
				for (int i = max(nPos, pLineTag->nTagSkip); i < nPos + L; i++)
				{
					if (lpText[i] == _T('\n'))
					{
						//�������з�
						pLineTag->nTagSkip = i + 1;

						//����ʹ�õ�ǰ�и�
						if (nYBearing + nMaxBottom == 0)
						{
							nMaxBottom = 0;
							nYBearing = pLineTag->tScalerRec.height;
						}
						goto Next;
					}
					FTC_SBit bitmap = GetBitmap(lpText[i], pLineTag->tScalerRec);
					AdjustBitmap(bitmap);

					//�ж��Ƿ񳬹�һ��
					if (nTotalWidth + g_shAdvance > rect.Width())
					{
						//��������ǰ�ַ�
						pLineTag->nTagSkip = i;
						goto Next;
					}

					nTotalWidth += g_shAdvance; //����ۼ�
					if (nYBearing < bitmap->top) //��׼λ��
						nYBearing = bitmap->top;
					if (nMaxBottom < bitmap->height - bitmap->top) //������о���
						nMaxBottom = bitmap->height - bitmap->top;
				}
				nPos += L; //ǰ��L���ַ�
				break;
			}
		}
		pLineTag->nTagSkip = nLength; //�������
		nTagNext = nLength;

Next:
		m_xPencil.x = 0;
		m_xPencil.y += nYBearing + nMaxBottom; //��һ�еĶ���

		pLineTag->nTagPos = nTagNext;

		if (!m_bMultiLine)
			goto Exit;
	}

Exit:
	return m_xPencil.y;
}
