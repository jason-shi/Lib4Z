#include "StdAfx.h"
#include "Dib.h"
#include "File.h"
#include "Png.h"

CDib::CDib(int width, int height, int depth)
	:Bits(0), Info(0), hGlobal(0)
{
	Create(width, height, depth);
}

CDib::~CDib()
{
	if (hGlobal)
	{
		::GlobalUnlock(hGlobal);
		::GlobalFree(hGlobal);
	}
}

BOOL CDib::Create(int width, int height, int depth)
{
	W = width;
	H = height;
	D = depth;

	bytes_per_line = ScanBytes(width, depth);
	bytes_per_pixel = PixelBytes(depth);

	long bitsAlloc = bytes_per_line * height;
	long headerSize = sizeof(BITMAPINFOHEADER);

	if ((hGlobal = ::GlobalAlloc(GMEM_MOVEABLE, bitsAlloc + headerSize)) == 0)
		return FALSE;

	Info = (BITMAPINFO *)::GlobalLock(hGlobal);
	Bits = (void *)((char *)Info + headerSize);

	Info->bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
	Info->bmiHeader.biWidth			= width;
	Info->bmiHeader.biHeight		= height;
	Info->bmiHeader.biBitCount		= (unsigned short)depth;
	Info->bmiHeader.biPlanes		= 1;
	Info->bmiHeader.biXPelsPerMeter	= 0;
	Info->bmiHeader.biYPelsPerMeter	= 0;
	Info->bmiHeader.biClrUsed		= 0;
	Info->bmiHeader.biClrImportant	= 0;
	Info->bmiHeader.biCompression	= BI_RGB;
	Info->bmiHeader.biSizeImage		= bitsAlloc;

	return TRUE;
}

void CDib::Release()
{
	if (hGlobal)
	{
		::GlobalUnlock(hGlobal);
		::GlobalFree(hGlobal);
	}
	Info = NULL;
	Bits = NULL;
}

void CDib::Clear()
{
	memset(GetBits(), 0, bytes_per_line * H);
}

BOOL CDib::LoadBMP(const TCHAR* filename)
{
	FILE *fp = _wfopen(filename, L"rb");
	if (fp == NULL)
		return FALSE;

	if (!ReadBMP(fp))
	{
		fclose(fp);
		return FALSE;
	}

	fclose(fp);
	return TRUE;
}

BOOL CDib::LoadPNG(const TCHAR* filename)
{
	FILE *fp = _wfopen(filename, L"rb");
	if (fp == NULL)
		return FALSE;

	if (!ReadPNG(fp))
	{
		fclose(fp);
		return FALSE;
	}

	fclose(fp);
	return TRUE;
}

BOOL CDib::ReadBMP(FILE *fp)
{
	BITMAPFILEHEADER header;
	BITMAPINFOHEADER infoHeader;

	if (fread(&header, 1, sizeof(header), fp) != sizeof(header)
		|| header.bfType != ((WORD)('M' << 8) | 'B')
		|| fread(&infoHeader, 1, sizeof(infoHeader), fp) != sizeof(infoHeader)
		|| infoHeader.biSize < sizeof(BITMAPINFOHEADER)
		|| infoHeader.biCompression != BI_RGB)
		return FALSE;

	int width  = infoHeader.biWidth;
	int height = infoHeader.biHeight;
	if (!IsOK())
	{
		if (!Create(width, height, 32))
			return FALSE;
	}
	if (width > Width() || height > Height())
		return FALSE;

	int bits_offset = sizeof(BITMAPFILEHEADER) + infoHeader.biSize;
	fseek(fp, bits_offset, SEEK_SET);

	int length = width * 3;
	byte *pBuff = (byte *)malloc(length);

	for (int y = height - 1; y >= 0; y--)
	{
		if (fread(pBuff, 1, length, fp) != length)
			return FALSE;
		byte *p = (byte *)GetBits(0, y);
		byte *q = pBuff;
		for (int x = 0; x < width; x++)
		{
			*p++ = *q++;
			*p++ = *q++;
			*p++ = *q++;
			if (*(q - 1) == 0 && *(q - 2) == 255 && *(q - 3) == 0)
				*p++ = 0;
			else
				*p++ = 255;
		}
	}
	free(pBuff);
	pBuff = NULL;

	return TRUE;
}

BOOL CDib::ReadPNG(FILE *fp)
{
	png_structp png_ptr = NULL;
	png_infop info_ptr = NULL;
	double display_exponent = 0;
	png_uint_32 image_width, image_height, image_rowbytes;
	int bit_depth, color_type;
	int image_channels;

	//初始化，取得宽，高，位深，色类
	unsigned char sig[8];
	fread(sig, 1, 8, fp);
	if (png_sig_cmp(sig, 0, 8))
		return FALSE;
	if ((png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL)) == NULL)
		return FALSE;
	if ((info_ptr = png_create_info_struct(png_ptr)) == NULL) {
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		return FALSE;
	}
	if (setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		return FALSE;
	}
	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, 8);  /* we already read the 8 signature bytes */
	png_read_info(png_ptr, info_ptr);  /* read all PNG info up to image data */
	png_get_IHDR(png_ptr, info_ptr, &image_width, &image_height, &bit_depth, &color_type,
		NULL, NULL, NULL);

	//取得图像其他块数据
	double gamma;
	png_bytepp row_pointers = NULL;

	if (setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		return NULL;
	}

	if (color_type == PNG_COLOR_TYPE_PALETTE)
		png_set_expand(png_ptr);
	if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
		png_set_expand(png_ptr);
	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
		png_set_expand(png_ptr);
	if (bit_depth == 16)
		png_set_strip_16(png_ptr);
	if (color_type == PNG_COLOR_TYPE_GRAY ||
		color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
		png_set_gray_to_rgb(png_ptr);
	if (png_get_gAMA(png_ptr, info_ptr, &gamma))
		png_set_gamma(png_ptr, display_exponent, gamma);

	png_read_update_info(png_ptr, info_ptr);

	image_rowbytes = png_get_rowbytes(png_ptr, info_ptr);
	image_channels = (int)png_get_channels(png_ptr, info_ptr);

	//创建内存区域
	if (!IsOK()) {
		if (!Create(image_width, image_height, bit_depth * 4))
			return FALSE;
	}
	if ((row_pointers = (png_bytepp)malloc(image_height*sizeof(png_bytep))) == NULL) {
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		free(Info);
		Info = NULL;
		return FALSE;
	}
	for (unsigned int i = 0; i < image_height; ++i)
		row_pointers[image_height-i-1] = (png_bytep)Bits + i*bytes_per_line;

	png_read_image(png_ptr, row_pointers);
	if (image_channels == 3) //3通道时增加透明
	{
		for (unsigned int y=0; y<image_height; y++) {
			byte *p = (byte *)row_pointers[y] + bytes_per_line - 1;
			byte *q = (byte *)row_pointers[y] + image_rowbytes - 1;
			for (unsigned int x = 0; x < image_width; x++) {
				if (*(q-2) == 0 && *(q-1) == 255 && *q == 0)
					*p-- = 0;
				else
					*p-- = 255;
				*p-- = *q--;
				*p-- = *q--;
				*p-- = *q--;
			}
		}
	}
	for (unsigned int y=0; y<image_height; y++) { //r,b交换
		byte *p = (byte *)row_pointers[y];
		for (unsigned int x = 0; x < image_width; x++) {			
			int t = *p;
			*p = *(p+2);
			*(p+2) = t;
			p += 4;
		}
	}

	free(row_pointers);
	row_pointers = NULL;
	png_read_end(png_ptr, NULL);

	if (png_ptr && info_ptr) {
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		png_ptr = NULL;
		info_ptr = NULL;
	}
	return TRUE;
}
