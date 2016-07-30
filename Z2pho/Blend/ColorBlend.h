#ifndef __ColorBlend_h__
#define __ColorBlend_h__

class CColorBlend {
  public: //Pseudo Blend Modes
	static COLORREF	 Normal(COLORREF bg, COLORREF fg); //正常模式
	static COLORREF	 Behind(COLORREF bg, COLORREF fg); //背后模式
	static COLORREF	 Dissolve(COLORREF bg, COLORREF fg); //溶解模式
	static COLORREF	 Opacity(COLORREF bg, COLORREF fg); //不透明模式

  public: //Commutative Blend Modes
	static COLORREF	 Darken(COLORREF bg, COLORREF fg); //变暗模式
	static COLORREF	 Lighten(COLORREF bg, COLORREF fg); //变亮模式
	static COLORREF	 Multiply(COLORREF bg, COLORREF fg); //正片叠底模式
	static COLORREF	 LinearBurn(COLORREF bg, COLORREF fg); //线性加深模式
	static COLORREF	 LinearDodge(COLORREF bg, COLORREF fg); //线性减淡模式
	static COLORREF	 Screen(COLORREF bg, COLORREF fg); //滤色模式
	static COLORREF	 Difference(COLORREF bg, COLORREF fg); //差分模式
	static COLORREF	 Exclusion(COLORREF bg, COLORREF fg); //排除模式
	static COLORREF	 Average(COLORREF bg, COLORREF fg); //平均模式
	static COLORREF	 Negation(COLORREF bg, COLORREF fg); //负片模式
	static COLORREF	 Phoenix(COLORREF bg, COLORREF fg); //复兴模式
	static COLORREF	 Interpolation(COLORREF bg, COLORREF fg); //插值模式

  public: //Non-commutative Blend Modes
	static COLORREF	 ColorBurn(COLORREF bg, COLORREF fg); //颜色加深模式
	static COLORREF	 ColorDodge(COLORREF bg, COLORREF fg); //颜色减淡模式
	static COLORREF	 ColorDark(COLORREF bg, COLORREF fg); //颜色加重模式
	static COLORREF	 Overlay(COLORREF bg, COLORREF fg); //叠加模式
	static COLORREF	 SoftLight(COLORREF bg, COLORREF fg); //柔光模式
	static COLORREF	 HardLight(COLORREF bg, COLORREF fg); //强光模式
	static COLORREF	 VividLight(COLORREF bg, COLORREF fg); //亮光模式
	static COLORREF	 LinearLight(COLORREF bg, COLORREF fg); //线性光模式
	static COLORREF	 PinLight(COLORREF bg, COLORREF fg); //点光模式
	static COLORREF	 HardMix(COLORREF bg, COLORREF fg); //实色混合模式
	static COLORREF	 Reflect(COLORREF bg, COLORREF fg); //反射模式
	static COLORREF	 Glow(COLORREF bg, COLORREF fg); //发光模式
	static COLORREF	 Freeze(COLORREF bg, COLORREF fg); //冷冻模式
	static COLORREF	 Heat(COLORREF bg, COLORREF fg); //加热模式

  public: //Other Blend Modes
	static COLORREF	 Hueration(COLORREF bg, COLORREF fg); //色相模式
	static COLORREF	 Saturation(COLORREF bg, COLORREF fg); //饱和度模式
	static COLORREF	 Coloration(COLORREF bg, COLORREF fg); //上色模式
	static COLORREF	 Brightness(COLORREF bg, COLORREF fg); //明度模式
	static COLORREF	 BitXor(COLORREF bg, COLORREF fg); //按位异或模式
	static COLORREF	 BitAnd(COLORREF bg, COLORREF fg); //按位并模式
	static COLORREF	 BitOr(COLORREF bg, COLORREF fg); //按位或模式
};

#endif
