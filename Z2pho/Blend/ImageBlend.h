#ifndef __ImageBlend_h__
#define __ImageBlend_h__

class CImageBlend {
  public: //Pseudo Blend Modes
	static void		 Normal(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //����ģʽ
	static void		 Behind(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //����ģʽ
	static void		 Dissolve(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //�ܽ�ģʽ
	static void		 Opacity(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //��͸��ģʽ

  public: //Commutative Blend Modes
	static void		 Darken(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //�䰵ģʽ
	static void		 Lighten(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //����ģʽ
	static void		 Multiply(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //��Ƭ����ģʽ
	static void		 LinearBurn(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //���Լ���ģʽ
	static void		 LinearDodge(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //���Լ���ģʽ
	static void		 Screen(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //��ɫģʽ
	static void		 Difference(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //���ģʽ
	static void		 Exclusion(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //�ų�ģʽ
	static void		 Average(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //ƽ��ģʽ
	static void		 Negation(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //��Ƭģʽ
	static void		 Phoenix(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //����ģʽ
	static void		 Interpolation(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //��ֵģʽ

  public: //Non-commutative Blend Modes
	static void		 ColorBurn(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //��ɫ����ģʽ
	static void		 ColorDodge(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //��ɫ����ģʽ
	static void		 ColorDark(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //��ɫ����ģʽ
	static void		 Overlay(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //����ģʽ
	static void		 SoftLight(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //���ģʽ
	static void		 HardLight(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //ǿ��ģʽ
	static void		 VividLight(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //����ģʽ
	static void		 LinearLight(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //���Թ�ģʽ
	static void		 PinLight(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //���ģʽ
	static void		 HardMix(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //ʵɫ���ģʽ
	static void		 Reflect(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //����ģʽ
	static void		 Glow(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //����ģʽ
	static void		 Freeze(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //�䶳ģʽ
	static void		 Heat(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //����ģʽ

  public: //Other Blend Modes
	static void		 Hueration(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //ɫ��ģʽ
	static void		 Saturation(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //���Ͷ�ģʽ
	static void		 Coloration(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //��ɫģʽ
	static void		 Brightness(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //����ģʽ
	static void		 BitXor(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //��λ���ģʽ
	static void		 BitAnd(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //��λ��ģʽ
	static void		 BitOr(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //��λ��ģʽ
};

//ͼ���ϴ���
typedef void (*ImageBlendDelegate)(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos);
static ImageBlendDelegate ImageBlend[] = {
	&CImageBlend::Normal,
	&CImageBlend::Behind,
	&CImageBlend::Dissolve,
	&CImageBlend::Opacity,
	&CImageBlend::Darken,
	&CImageBlend::Lighten,
	&CImageBlend::Multiply,
	&CImageBlend::LinearBurn,
	&CImageBlend::LinearDodge,
	&CImageBlend::Screen,
	&CImageBlend::Difference,
	&CImageBlend::Exclusion,
	&CImageBlend::Average,
	&CImageBlend::Negation,
	&CImageBlend::Phoenix,
	&CImageBlend::Interpolation,
	&CImageBlend::ColorBurn,
	&CImageBlend::ColorDodge,
	&CImageBlend::ColorDark,
	&CImageBlend::Overlay,
	&CImageBlend::SoftLight,
	&CImageBlend::HardLight,
	&CImageBlend::VividLight,
	&CImageBlend::LinearLight,
	&CImageBlend::PinLight,
	&CImageBlend::HardMix,
	&CImageBlend::Reflect,
	&CImageBlend::Glow,
	&CImageBlend::Freeze,
	&CImageBlend::Heat,
	&CImageBlend::Hueration,
	&CImageBlend::Saturation,
	&CImageBlend::Coloration,
	&CImageBlend::Brightness,
	&CImageBlend::BitXor,
	&CImageBlend::BitAnd,
	&CImageBlend::BitOr,
};

#endif
