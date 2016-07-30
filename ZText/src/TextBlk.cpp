#include "StdAfx.h"

static short		 g_shLeft;
static short		 g_shAdvance;
static short		 g_shTop;
static short		 g_shHeight;

static TFontStyle	 g_tScanFS;
static TFontStyle	 g_tDrawFS;

inline void UpdateFaceID(TFontStyle& tFontStyle)
{
	//��FaceName��Style������FaceID
	tFontStyle.tScalerRec.face_id = g_pZText->GetFaceID(tFontStyle.szFaceName, tFontStyle.byStyle);
}

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

CTextBlk::CTextBlk(bool bMultiLine, int nHeight, CPoint xDrawPos, CSize xSize, int nDepth)
			: CPanel(NULL, xDrawPos, xSize, nDepth)
{
	m_bMultiLine = bMultiLine;
	m_nHeight = nHeight;
	m_nBaseLine = (m_nHeight + 9) / 10;

	m_pImage = NULL;
	m_xPencil.x = 0;
	m_xPencil.y = 0;
}

CTextBlk::~CTextBlk()
{
	delete m_pImage;
	m_pImage = NULL;
}

void CTextBlk::Set(bool bMultiLine, int nHeight, CPoint xDrawPos, CSize xSize, int nDepth)
{
	m_bMultiLine = bMultiLine;
	m_nHeight = nHeight;
	m_nBaseLine = (m_nHeight + 9) / 10;

	m_xPencil.x = 0;
	m_xPencil.y = 0;

	m_xDrawPos = xDrawPos;
	m_xSize = xSize;
	m_nDepth = nDepth;

	AllocImage(xSize.cx, xSize.cy);
}

bool CTextBlk::Initial()
{
	if (!CPanel::Initial())
		return false;

	m_pImage = new CImage(m_xSize.cx, m_xSize.cy);
	return m_pImage != NULL;
}

bool CTextBlk::Release()
{
	CPanel::Release();

	delete m_pImage;
	m_pImage = NULL;

	return true;
}

void CTextBlk::SetRotate(FT_Face &face)
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

void CTextBlk::InitialFontStyle(TFontStyle &tFontStyle, LOGFONT &logFont)
{
	tFontStyle.byStyle = TS_NORMAL;

	WideCharToMultiByte(CP_ACP, 0, logFont.lfFaceName, -1, tFontStyle.szFaceName, LF_FACESIZE, NULL, NULL);

	UpdateFaceID(tFontStyle);
	tFontStyle.tScalerRec.width = logFont.lfWidth;
	tFontStyle.tScalerRec.height = logFont.lfHeight;
	tFontStyle.tScalerRec.pixel = 1;

	g_pZText->SetSpaceWidth(logFont.lfWidth);
}

FTC_SBit CTextBlk::GetBitmap(TCHAR tch, FTC_ScalerRec &tScalerRec)
{
	FTC_Scaler pSelectedIC;
	FTC_SBit   bitmap;

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

void CTextBlk::AdjustBitmap(FTC_SBit bitmap)
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

bool CTextBlk::PenAdvance(CRect &rect, int nXSpace, int nYSpace, FTC_SBit bitmap)
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

void CTextBlk::FillGlyph(CRect &rect, int nWidth, int nHeight, int nYBearing, COLORREF bkcolor)
{
	for (int y = m_xPencil.y - nYBearing; y < m_xPencil.y - nYBearing + nHeight && y < rect.bottom; y++)
	{
		int *p = (int *)m_pImage->GetBits(m_xPencil.x, y);
		for (int x = m_xPencil.x; x < m_xPencil.x + nWidth && x < rect.right; x++)
			*p++ = bkcolor;
	}
}

void CTextBlk::_DrawMonoGlyph(FTC_SBit bitmap, bool transparent, COLORREF color)
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

void CTextBlk::_DrawGrayGlyph(FTC_SBit bitmap, bool transparent, COLORREF color)
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

void CTextBlk::DrawGlyph(FTC_SBit bitmap, bool transparent, COLORREF color)
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

void CTextBlk::ClearText()
{
	m_xPencil.x = 0;
	m_xPencil.y = 0;

	ClearImage();
	g_pZ2coo->Redraw(this);
}

int CTextBlk::OutText(const TCHAR *lpText, int nLength, HFONT hFont, bool transparent,
					  COLORREF color, COLORREF bkcolor, int nXSpace, int nYSpace)
{
	m_pImage->Clear();

	//��ȡ��������
	CRect rect;
	GetRect(&rect);

	m_xPencil.x = 0;
	m_xPencil.y = 0;

	//����ָ����FontStyle
	LOGFONT logFont;
	GetObject(hFont, sizeof(logFont), &logFont);

	InitialFontStyle(g_tDrawFS, logFont);

	//��ʼ�����и�ʮ��֮�ŵ�λ��
	m_xPencil.y = m_nHeight - m_nBaseLine;

	FTC_SBit bitmap;
	for (int i = 0; i < nLength; i++)
	{
		if (lpText[i] == _T('\n'))
			goto Exit;

		bitmap = GetBitmap(lpText[i], g_tDrawFS.tScalerRec);
		AdjustBitmap(bitmap);

		//�ж��Ƿ񳬹�һ��
		if (!PenAdvance(rect, nXSpace, nYSpace, bitmap))
			goto Exit;

		if (!transparent)
		{
			//��͸��ʱ����䱳��ɫ
			FillGlyph(rect, g_shAdvance + nXSpace, m_nHeight, m_nHeight - m_nBaseLine, bkcolor);
		}
		DrawGlyph(bitmap, transparent, color);
		m_xPencil.x += g_shAdvance + nXSpace;
	}

Exit:
	g_pZ2coo->Redraw(this);

	m_xPencil.y += m_nBaseLine;
	return m_xPencil.y;
}

int CTextBlk::ExtOutText(const TCHAR *lpText, int nLength)
{
	m_pImage->Clear();

	//��ȡ��������
	CRect rect;
	GetRect(&rect);

	m_xPencil.x = 0;
	m_xPencil.y = 0;

	//����Ĭ�ϵ�FontStyle
	LOGFONT logFont;
	GetObject(g_pZText->GetDefaultFont(), sizeof(logFont), &logFont);

	InitialFontStyle(g_tDrawFS, logFont);

	//͸������ɫ������ɫ
	bool transparent = true;
	COLORREF color = BGRA(0, 0, 0, 255);
	COLORREF bkcolor = BGRA(255, 255, 255, 255);

	//��ʼ�����и�ʮ��֮�ŵ�λ��
	m_xPencil.y = m_nHeight - m_nBaseLine;

	//�ּ�࣬�м��
	int nXSpace = 0;
	int nYSpace = 0;

	short T, L;
	int nPos = 0;
	while (nPos < nLength)
	{
		nPos = GetShort(lpText, &T, nPos);
		nPos = GetShort(lpText, &L, nPos);
		switch (T)
		{
		case TF_TRANSPARENT:
			nPos = GetChar(lpText, (char *)&transparent, nPos);
			break;
		case TF_COLOR:
			nPos = GetColor(lpText, &color, nPos);
			break;
		case TF_BKCOLOR:
			nPos = GetColor(lpText, &bkcolor, nPos);
			break;
		case TF_X_SPACE:
			nPos = GetShort(lpText, (short *)&nXSpace, nPos);
			break;
		case TF_Y_SPACE:
			nPos = GetShort(lpText, (short *)&nYSpace, nPos);
			break;
		case TF_TEXT:
			FTC_SBit bitmap;
			for (int i = nPos; i < nPos + L; i++)
			{
				if (lpText[i] == _T('\n'))
				{
					if (!m_bMultiLine)
						goto Exit;
					m_xPencil.x = 0;
					m_xPencil.y += m_nHeight + nYSpace;
					if (m_xPencil.y + m_nBaseLine > rect.Height())
						goto Exit;
					continue;
				}
				bitmap = GetBitmap(lpText[i], g_tDrawFS.tScalerRec);
				AdjustBitmap(bitmap);

				//�ж��Ƿ񳬹�һ��
				if (!PenAdvance(rect, nXSpace, nYSpace, bitmap))
					goto Exit;

				if (!transparent)
				{
					//��͸��ʱ����䱳��ɫ
					FillGlyph(rect, g_shAdvance + nXSpace, m_nHeight, m_nHeight - m_nBaseLine, bkcolor);
				}
				DrawGlyph(bitmap, transparent, color);
				m_xPencil.x += g_shAdvance;
			}
			nPos += L; //L���ַ�
			break;
		default:
			nPos += L;
			break;
		}
	}

Exit:
	g_pZ2coo->Redraw(this);

	m_xPencil.y += m_nBaseLine;
	return m_xPencil.y;
}

int CTextBlk::EnhOutText(const TCHAR *lpText, int nLength)
{
	m_pImage->Clear();

	//��ȡ��������
	CRect rect;
	GetRect(&rect);

	m_xPencil.x = 0;
	m_xPencil.y = 0;

	//����Ĭ�ϵ�FontStyle
	LOGFONT logFont;
	GetObject(g_pZText->GetDefaultFont(), sizeof(logFont), &logFont);

	InitialFontStyle(g_tScanFS, logFont);
	InitialFontStyle(g_tDrawFS, logFont);

	//͸������ɫ������ɫ
	bool transparent = true;
	COLORREF color = BGRA(0, 0, 0, 255);
	COLORREF bkcolor = BGRA(255, 255, 255, 255);

	//��nTagPos����ʼ����������nTagSkip���ַ�
	int nTagPos = 0, nTagNext;
	int nTagSkip = 0, nTagCurr;

	//Ԥ�����е����߶ȣ���׼λ�ã�������о��룬�ַ����
	int nMaxHeight;
	int nYBearing;
	int nMaxBottom;
	int nTotalWidth;

	//�ּ�࣬�м��
	int nXSpace = 0;
	int nYSpace = 0;

	short T, L;
	while (nTagPos < nLength)
	{
		nMaxHeight = 0;
		nYBearing = 0;
		nMaxBottom = 0;
		nTotalWidth = 0;

		nTagCurr = nTagSkip; //��¼��ǰ����λ��

		int nPos = nTagPos;
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
				nPos = GetShort(lpText, (short *)&g_tScanFS.tScalerRec.width, nPos);
				break;
			case TF_HEIGHT:
				nPos = GetShort(lpText, (short *)&g_tScanFS.tScalerRec.height, nPos);
				break;
			case TF_FAMILY:
				nPos = GetFamily(lpText, g_tScanFS.szFaceName, nPos, L);
				UpdateFaceID(g_tScanFS);
				break;
			case TF_STYLE:
				nPos = GetChar(lpText, (char *)&g_tScanFS.byStyle, nPos);
				UpdateFaceID(g_tScanFS);
				break;
			case TF_X_SPACE:
				nPos = GetShort(lpText, (short *)&nXSpace, nPos);
				break;
			case TF_Y_SPACE:
				nPos = GetShort(lpText, (short *)&nYSpace, nPos);
				break;
			case TF_TEXT:
				for (int i = max(nPos, nTagSkip); i < nPos + L; i++)
				{
					if (lpText[i] == _T('\n'))
					{
						//�������з�
						nTagSkip = i + 1;

						//����ʹ�õ�ǰ�и�
						if (nMaxHeight == 0)
						{
							nMaxHeight = g_tScanFS.tScalerRec.height;
							nMaxBottom = 0;
							nYBearing = g_tScanFS.tScalerRec.height;
						}
						goto Draw;
					}
					FTC_SBit bitmap = GetBitmap(lpText[i], g_tScanFS.tScalerRec);
					AdjustBitmap(bitmap);

					//�ж��Ƿ񳬹�һ��
					if (nTotalWidth + g_shAdvance + nXSpace > rect.Width())
					{
						//��������ǰ�ַ�
						nTagSkip = i;
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
		nTagSkip = nLength; //�������
		nTagNext = nLength;

Draw:
		if (m_xPencil.y + nYBearing + nMaxBottom > rect.Height())
			goto Exit;
		m_xPencil.y += nYBearing; //FreeType�ĸֱ�λ��

		nPos = nTagPos;
		while (nPos < nLength)
		{
			nPos = GetShort(lpText, &T, nPos);
			nPos = GetShort(lpText, &L, nPos);
			switch (T)
			{
			case TF_TRANSPARENT:
				nPos = GetChar(lpText, (char *)&transparent, nPos);
				break;
			case TF_COLOR:
				nPos = GetColor(lpText, &color, nPos);
				break;
			case TF_BKCOLOR:
				nPos = GetColor(lpText, &bkcolor, nPos);
				break;
			case TF_WIDTH:
				nPos = GetShort(lpText, (short *)&g_tDrawFS.tScalerRec.width, nPos);
				break;
			case TF_HEIGHT:
				nPos = GetShort(lpText, (short *)&g_tDrawFS.tScalerRec.height, nPos);
				break;
			case TF_FAMILY:
				nPos = GetFamily(lpText, g_tDrawFS.szFaceName, nPos, L);
				UpdateFaceID(g_tDrawFS);
				break;
			case TF_STYLE:
				nPos = GetChar(lpText, (char *)&g_tDrawFS.byStyle, nPos);
				UpdateFaceID(g_tDrawFS);
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
					if (i == nTagSkip)
						goto Next;
					if (lpText[i] == _T('\n'))
						goto Next;

					bitmap = GetBitmap(lpText[i], g_tDrawFS.tScalerRec);
					AdjustBitmap(bitmap);

					m_xPencil.x += nXSpace; //ƫ���ּ��
					if (!transparent)
					{
						//��͸��ʱ����䱳��ɫ
						FillGlyph(rect, g_shAdvance + nXSpace, nYBearing + nMaxBottom, nYBearing, bkcolor);
					}
					DrawGlyph(bitmap, transparent, color);
					m_xPencil.x += g_shAdvance;
				}
				nPos += L; //ǰ��L���ַ�
				break;
			}
		}
Next:
		m_xPencil.x = 0;
		m_xPencil.y += nMaxBottom + nYSpace; //��һ�еĶ���

		nTagPos = nTagNext;

		if (!m_bMultiLine)
			goto Exit;
	}

Exit:
	g_pZ2coo->Redraw(this);

	return m_xPencil.y;
}
