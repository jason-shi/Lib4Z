#include "StdAfx.h"
#include "File.h"

CImage::CImage(int nWidth, int nHeight)
{
	Create(nWidth, nHeight);
}

BOOL CImage::Create(int nWidth, int nHeight)
{
	return CDib::Create(nWidth, nHeight, 32);
}

BOOL CImage::LoadImage(const TCHAR* filename)
{
	const TCHAR *suffix = wcsrchr(filename, L'.');
	if (!suffix)
		return FALSE;

	if (wmemcmp(suffix, L".BMP", wcslen(L".BMP")) == 0
		|| wmemcmp(suffix, L".bmp", wcslen(L".bmp")) == 0)
		return LoadBMP(filename);

	if (wmemcmp(suffix, L".PNG", wcslen(L".PNG")) == 0
		|| wmemcmp(suffix, L".png", wcslen(L".png")) == 0)
		return LoadPNG(filename);

	return FALSE;
}

void CImage::Copy(const CImage* pImage, const CRect& rect, CPoint src_pos)
{
	int len	= rect.Width() * 4;
	for	(int y = rect.top; y < rect.bottom;	y++) {
		memcpy(GetBits(rect.left, y), pImage->GetBits(src_pos.x, src_pos.y++), len);
	}
}

void CImage::MixImage(const CImage* pImage, const CRect& rect, CPoint src_pos)
{
	byte r, g, b, a;
	for (int y = rect.top; y < rect.bottom; y++) {
		byte *p = (byte *)GetBits(rect.left, y);
		const byte *q = (byte *)pImage->GetBits(src_pos.x, src_pos.y++);
		for (int x = rect.left; x < rect.right; x++) {
			b = *q++;
			g = *q++;
			r = *q++;
			a = *q++;
			if (a == 255) {
				*p++ = b;
				*p++ = g;
				*p++ = r;
				*p++ = a;
			} else if (a == 0) {
				p += 4;
			} else {
				alpha_composite(*p++, b, a, *p);
				alpha_composite(*p++, g, a, *p);
				alpha_composite(*p++, r, a, *p);
				p++;
			}
		}
	}
}

void CImage::FastLineH(int x1, int x2, int y, COLORREF color)
{
	int *p = (int *)GetBits(min(x1, x2), y);
	int *limit = (int *)GetBits(max(x1, x2), y);
	while (p <= limit)
	{
		//�����˵�
		*p++ = color;
	}
}

void CImage::FastLineL(int x1, int y1, int x2, int y2, COLORREF color)
{
	int *p = (int *)GetBits(min(x1, x2), max(y1, y2));
	int *limit = (int *)GetBits(max(x1, x2), min(y1, y2));
	while (p <= limit)
	{
		*p++ = color;
		p += W;
	}
}

void CImage::FastLineV(int x, int y1, int y2, COLORREF color)
{
	int *p = (int *)GetBits(x, max(y1, y2));
	int *limit = (int *)GetBits(x, min(y1, y2));
	while (p <= limit)
	{
		*p = color;
		p += W;
	}
}

void CImage::FastLineB(int x1, int y1, int x2, int y2, COLORREF color)
{
	int *p = (int *)GetBits(max(x1, x2), max(y1, y2));
	int *limit = (int *)GetBits(min(x1, x2), min(y1, y2));
	while (p <= limit)
	{
		*p-- = color;
		p += W;
	}
}

void CImage::FillLine(int x1, int x2, int y, COLORREF color)
{
	int left = max(min(x1, x2), 0);
	int right = min(max(x1, x2), W);

	int *p = (int *)GetBits(left, y);
	for (int x = left; x < right; x++)
	{
		//������ˮƽ��
		*p++ = color;
	}
}

void CImage::FillRect(const CRect& rect, COLORREF color)
{
	const unsigned char b = GetBValue(color);
	const unsigned char g = GetGValue(color);
	const unsigned char r = GetRValue(color);
	const unsigned char a = GetAValue(color);

	for (int y = rect.top; y < rect.bottom; y++) {
		byte *p = (byte *)GetBits(rect.left, y);
		for (int x = rect.left; x < rect.right; x++) {
			*p++ = b;
			*p++ = g;
			*p++ = r;
			*p++ = a;
		}
	}
}

void CImage::GrayedRect(const CRect& rect)
{
	for (int y = rect.top; y < rect.bottom; y++) {
		byte *p = (byte *)GetBits(rect.left, y);
		for (int x = rect.left; x < rect.right; x++) {
			byte average = *p * 0.114 + *(p + 1) * 0.587 + *(p + 2) * 0.299;
			*p++ = average;
			*p++ = average;
			*p++ = average;
			*p++ = 255;
		}
	}
}

void CImage::ZoomRect(const CImage* pImage, const CRect& rect, CPoint src_pos, float fRatioX, float fRatioY)
{
	int nWidth = (int)(rect.right - rect.left) * fRatioX; //��ͼ��Ŀ��
	int nHeight = (int)(rect.bottom - rect.top) * fRatioY; //��ͼ��ĸ߶�
	byte *lpSrc = (byte *)pImage->GetBits(src_pos.x, src_pos.y); //Դͼ�����Ҫ����������׵�ַ
	int xSrc, ySrc; //��Ӧ����Դͼ�������
	for (int y = 0; y < nHeight; y++) //����ͼ���ÿ�н��в���
	{
		byte *p = (byte *)GetBits(rect.left, rect.top+y); //�õ���Ҫ���Ƶĵ�ĵ�ַ
		for (int x = 0; x < nWidth; x++) //����ͼ���ÿ���ϵ�ÿ�����ؽ��в���
		{
			xSrc = (int)(x / fRatioX + 0.5); //��Ӧ��Դͼ��ĺ�����
			ySrc = (int)(y / fRatioY + 0.5); //��Ӧ��Դͼ���������

			//��Ӧ����Դͼ�����Ҫ��������֮��
			if (xSrc >= 0 && xSrc < rect.right-rect.left && ySrc >= 0 && ySrc < rect.bottom-rect.top)
			{
				//�õ���Ӧ�ĵ���Դͼ��ĵ�ַ
				byte *q = lpSrc - ySrc * pImage->bytes_per_line + xSrc * pImage->bytes_per_pixel;
				for (int i = 0; i < 3; i++)
					*p++ = *q++;
				*p++ = *q++;
			}
			else
			{
				p += 4;
			}
		}
	}
}

void CImage::Convolve(const CRect& rect, float *lpKernel)
{
	//�����ϵ��֮��
	float kernels = 0;
	for (int i = 0; i < 9; i++)
		kernels += lpKernel[i];
	if (kernels == 0)
		kernels = 1.0f;

	//������ͼ�񲢶��⸴��������
	CImage *pTemp = new CImage(rect.Width() + 2, rect.Height() + 2);
	pTemp->_Convolve_ExtCopy(this, rect.TopLeft());

	//����ʱͼƬ��ƫ��
	int bytes_per_line = pTemp->BytesPerLine();

	for (int y = rect.top, src_y = 1; y < rect.bottom; y++) {
		byte *p = (byte *)GetBits(rect.left, y);
		//�������ƶ����еĿ�ʼ
		const byte *q = (byte *)pTemp->GetBits(1, src_y++);
		const byte *pNebour[9] = {
			q + bytes_per_line - 4, q + bytes_per_line, q + bytes_per_line + 4,
			q - 4, q, q + 4,
			q - bytes_per_line - 4, q - bytes_per_line, q - bytes_per_line + 4,
		};
		q += 4;

		for (int x = rect.left; x < rect.right; x++) {
			//�Ը��������ڵ�������
			int nSumB = 0, nSumG = 0, nSumR = 0;
			for (int i = 0; i < 9; i++)
			{
				nSumB += *(pNebour[i] + 0) * lpKernel[i];
				nSumG += *(pNebour[i] + 1) * lpKernel[i];
				nSumR += *(pNebour[i] + 2) * lpKernel[i];
			}
			nSumB /= kernels; nSumG /= kernels; nSumR /= kernels;
			*p++ = nSumB > 255 ? 255 : nSumB < 0 ? 0 : nSumB;
			*p++ = nSumG > 255 ? 255 : nSumG < 0 ? 0 : nSumG;
			*p++ = nSumR > 255 ? 255 : nSumR < 0 ? 0 : nSumR;
			*p++;
			//�����������ƶ�һ������
			for (int i = 0; i < 9; i++)
				pNebour[i] += 4;
		}
	}
	delete pTemp;
}

void CImage::_Convolve_ExtCopy(CImage *pImage, CPoint src_pos)
{
	Copy(pImage, CRect(1, 1, W - 1, H - 1), src_pos);

	//����������
	Copy(this, CRect(0, 0, W, 1), CPoint(0, 1));
	Copy(this, CRect(0, H - 1, W, H), CPoint(0, H - 2));
	//����������
	Copy(this, CRect(0, 0, 1, H), CPoint(1, 0));
	Copy(this, CRect(W - 1, 0, W, H), CPoint(W - 2, 0));
}
