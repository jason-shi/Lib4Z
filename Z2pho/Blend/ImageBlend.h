#ifndef __ImageBlend_h__
#define __ImageBlend_h__

class CImageBlend {
  public: //Pseudo Blend Modes
	static void		 Normal(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //正常模式
	static void		 Behind(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //背后模式
	static void		 Dissolve(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //溶解模式
	static void		 Opacity(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //不透明模式

  public: //Commutative Blend Modes
	static void		 Darken(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //变暗模式
	static void		 Lighten(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //变亮模式
	static void		 Multiply(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //正片叠底模式
	static void		 LinearBurn(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //线性加深模式
	static void		 LinearDodge(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //线性减淡模式
	static void		 Screen(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //滤色模式
	static void		 Difference(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //差分模式
	static void		 Exclusion(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //排除模式
	static void		 Average(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //平均模式
	static void		 Negation(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //负片模式
	static void		 Phoenix(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //复兴模式
	static void		 Interpolation(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //插值模式

  public: //Non-commutative Blend Modes
	static void		 ColorBurn(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //颜色加深模式
	static void		 ColorDodge(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //颜色减淡模式
	static void		 ColorDark(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //颜色加重模式
	static void		 Overlay(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //叠加模式
	static void		 SoftLight(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //柔光模式
	static void		 HardLight(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //强光模式
	static void		 VividLight(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //亮光模式
	static void		 LinearLight(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //线性光模式
	static void		 PinLight(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //点光模式
	static void		 HardMix(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //实色混合模式
	static void		 Reflect(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //反射模式
	static void		 Glow(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //发光模式
	static void		 Freeze(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //冷冻模式
	static void		 Heat(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //加热模式

  public: //Other Blend Modes
	static void		 Hueration(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //色相模式
	static void		 Saturation(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //饱和度模式
	static void		 Coloration(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //上色模式
	static void		 Brightness(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //明度模式
	static void		 BitXor(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //按位异或模式
	static void		 BitAnd(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //按位并模式
	static void		 BitOr(CImage *pbg, const CImage *pfg, const CRect& rect, CPoint src_pos); //按位或模式
};

//图象混合代理
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
