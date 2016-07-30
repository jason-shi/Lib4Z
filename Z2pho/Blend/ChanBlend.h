#ifndef __ChanBlend_h__
#define __ChanBlend_h__

class CChanBlend {
  public:
	~CChanBlend();

  public: //Commutative Blend Modes
	static byte		 Darken(byte bg, byte fg); //变暗模式
	static byte		 Lighten(byte bg, byte fg); //变亮模式
	static byte		 Multiply(byte bg, byte fg); //正片叠底模式
	static byte		 LinearBurn(byte bg, byte fg); //线性加深模式
	static byte		 LinearDodge(byte bg, byte fg); //线性减淡模式
	static byte		 Screen(byte bg, byte fg); //滤色模式
	static byte		 Difference(byte bg, byte fg); //差分模式
	static byte		 Exclusion(byte bg, byte fg); //排除模式
	static byte		 Average(byte bg, byte fg); //平均模式
	static byte		 Negation(byte bg, byte fg); //负片模式
	static byte		 Phoenix(byte bg, byte fg); //复兴模式
	static byte		 Interpolation(byte bg, byte fg); //插值模式

  public: //Non-commutative Blend Modes
	static byte		 ColorBurn(byte bg, byte fg); //颜色加深模式
	static byte		 ColorDodge(byte bg, byte fg); //颜色减淡模式
	static byte		 ColorDark(byte bg, byte fg); //颜色加重模式
	static byte		 Overlay(byte bg, byte fg); //叠加模式
	static byte		 SoftLight(byte bg, byte fg); //柔光模式
	static byte		 HardLight(byte bg, byte fg); //强光模式
	static byte		 VividLight(byte bg, byte fg); //亮光模式
	static byte		 LinearLight(byte bg, byte fg); //线性光模式
	static byte		 PinLight(byte bg, byte fg); //点光模式
	static byte		 HardMix(byte bg, byte fg); //实色混合模式
	static byte		 Reflect(byte bg, byte fg); //反射模式
	static byte		 Glow(byte bg, byte fg); //发光模式
	static byte		 Freeze(byte bg, byte fg); //冷冻模式
	static byte		 Heat(byte bg, byte fg); //加热模式

  private:
	static double	*_TblCosine;
};

inline CChanBlend::~CChanBlend()
{
	delete _TblCosine;
	_TblCosine = NULL;
}

#endif
