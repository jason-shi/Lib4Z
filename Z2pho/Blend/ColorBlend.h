#ifndef __ColorBlend_h__
#define __ColorBlend_h__

class CColorBlend {
  public: //Pseudo Blend Modes
	static COLORREF	 Normal(COLORREF bg, COLORREF fg); //����ģʽ
	static COLORREF	 Behind(COLORREF bg, COLORREF fg); //����ģʽ
	static COLORREF	 Dissolve(COLORREF bg, COLORREF fg); //�ܽ�ģʽ
	static COLORREF	 Opacity(COLORREF bg, COLORREF fg); //��͸��ģʽ

  public: //Commutative Blend Modes
	static COLORREF	 Darken(COLORREF bg, COLORREF fg); //�䰵ģʽ
	static COLORREF	 Lighten(COLORREF bg, COLORREF fg); //����ģʽ
	static COLORREF	 Multiply(COLORREF bg, COLORREF fg); //��Ƭ����ģʽ
	static COLORREF	 LinearBurn(COLORREF bg, COLORREF fg); //���Լ���ģʽ
	static COLORREF	 LinearDodge(COLORREF bg, COLORREF fg); //���Լ���ģʽ
	static COLORREF	 Screen(COLORREF bg, COLORREF fg); //��ɫģʽ
	static COLORREF	 Difference(COLORREF bg, COLORREF fg); //���ģʽ
	static COLORREF	 Exclusion(COLORREF bg, COLORREF fg); //�ų�ģʽ
	static COLORREF	 Average(COLORREF bg, COLORREF fg); //ƽ��ģʽ
	static COLORREF	 Negation(COLORREF bg, COLORREF fg); //��Ƭģʽ
	static COLORREF	 Phoenix(COLORREF bg, COLORREF fg); //����ģʽ
	static COLORREF	 Interpolation(COLORREF bg, COLORREF fg); //��ֵģʽ

  public: //Non-commutative Blend Modes
	static COLORREF	 ColorBurn(COLORREF bg, COLORREF fg); //��ɫ����ģʽ
	static COLORREF	 ColorDodge(COLORREF bg, COLORREF fg); //��ɫ����ģʽ
	static COLORREF	 ColorDark(COLORREF bg, COLORREF fg); //��ɫ����ģʽ
	static COLORREF	 Overlay(COLORREF bg, COLORREF fg); //����ģʽ
	static COLORREF	 SoftLight(COLORREF bg, COLORREF fg); //���ģʽ
	static COLORREF	 HardLight(COLORREF bg, COLORREF fg); //ǿ��ģʽ
	static COLORREF	 VividLight(COLORREF bg, COLORREF fg); //����ģʽ
	static COLORREF	 LinearLight(COLORREF bg, COLORREF fg); //���Թ�ģʽ
	static COLORREF	 PinLight(COLORREF bg, COLORREF fg); //���ģʽ
	static COLORREF	 HardMix(COLORREF bg, COLORREF fg); //ʵɫ���ģʽ
	static COLORREF	 Reflect(COLORREF bg, COLORREF fg); //����ģʽ
	static COLORREF	 Glow(COLORREF bg, COLORREF fg); //����ģʽ
	static COLORREF	 Freeze(COLORREF bg, COLORREF fg); //�䶳ģʽ
	static COLORREF	 Heat(COLORREF bg, COLORREF fg); //����ģʽ

  public: //Other Blend Modes
	static COLORREF	 Hueration(COLORREF bg, COLORREF fg); //ɫ��ģʽ
	static COLORREF	 Saturation(COLORREF bg, COLORREF fg); //���Ͷ�ģʽ
	static COLORREF	 Coloration(COLORREF bg, COLORREF fg); //��ɫģʽ
	static COLORREF	 Brightness(COLORREF bg, COLORREF fg); //����ģʽ
	static COLORREF	 BitXor(COLORREF bg, COLORREF fg); //��λ���ģʽ
	static COLORREF	 BitAnd(COLORREF bg, COLORREF fg); //��λ��ģʽ
	static COLORREF	 BitOr(COLORREF bg, COLORREF fg); //��λ��ģʽ
};

#endif
