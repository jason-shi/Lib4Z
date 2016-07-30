#ifndef __ChanBlend_h__
#define __ChanBlend_h__

class CChanBlend {
  public:
	~CChanBlend();

  public: //Commutative Blend Modes
	static byte		 Darken(byte bg, byte fg); //�䰵ģʽ
	static byte		 Lighten(byte bg, byte fg); //����ģʽ
	static byte		 Multiply(byte bg, byte fg); //��Ƭ����ģʽ
	static byte		 LinearBurn(byte bg, byte fg); //���Լ���ģʽ
	static byte		 LinearDodge(byte bg, byte fg); //���Լ���ģʽ
	static byte		 Screen(byte bg, byte fg); //��ɫģʽ
	static byte		 Difference(byte bg, byte fg); //���ģʽ
	static byte		 Exclusion(byte bg, byte fg); //�ų�ģʽ
	static byte		 Average(byte bg, byte fg); //ƽ��ģʽ
	static byte		 Negation(byte bg, byte fg); //��Ƭģʽ
	static byte		 Phoenix(byte bg, byte fg); //����ģʽ
	static byte		 Interpolation(byte bg, byte fg); //��ֵģʽ

  public: //Non-commutative Blend Modes
	static byte		 ColorBurn(byte bg, byte fg); //��ɫ����ģʽ
	static byte		 ColorDodge(byte bg, byte fg); //��ɫ����ģʽ
	static byte		 ColorDark(byte bg, byte fg); //��ɫ����ģʽ
	static byte		 Overlay(byte bg, byte fg); //����ģʽ
	static byte		 SoftLight(byte bg, byte fg); //���ģʽ
	static byte		 HardLight(byte bg, byte fg); //ǿ��ģʽ
	static byte		 VividLight(byte bg, byte fg); //����ģʽ
	static byte		 LinearLight(byte bg, byte fg); //���Թ�ģʽ
	static byte		 PinLight(byte bg, byte fg); //���ģʽ
	static byte		 HardMix(byte bg, byte fg); //ʵɫ���ģʽ
	static byte		 Reflect(byte bg, byte fg); //����ģʽ
	static byte		 Glow(byte bg, byte fg); //����ģʽ
	static byte		 Freeze(byte bg, byte fg); //�䶳ģʽ
	static byte		 Heat(byte bg, byte fg); //����ģʽ

  private:
	static double	*_TblCosine;
};

inline CChanBlend::~CChanBlend()
{
	delete _TblCosine;
	_TblCosine = NULL;
}

#endif
