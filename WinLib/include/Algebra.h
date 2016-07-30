#ifndef __Algebra_h__
#define __Algebra_h__

#include <math.h>

class CComplex {
  public:
	CComplex();
	CComplex(double _a, double _b);
	CComplex(const CComplex& complex);

  public:
	CComplex		 operator+(const CComplex& complex) const;
	CComplex		 operator-(const CComplex& complex) const;
	CComplex		 operator*(const CComplex& complex) const;
	CComplex		 operator/(const CComplex& complex) const;

	void			 operator+=(const CComplex& complex);
	void			 operator-=(const CComplex& complex);
	void			 operator*=(const CComplex& complex);
	void			 operator/=(const CComplex& complex);

	void			 operator=(const CComplex& complex);

  public:
	CComplex		 operator*(double value) const;
	CComplex		 operator/(double value) const;
	void			 operator*=(double value);
	void			 operator/=(double value);

  public:
	double			 GetModule();
	double			 GetProduct();

  public:
	double			 a;
	double			 b;
};

CComplex operator*(double value, const CComplex& complex);
CComplex cpower(const CComplex& complex, unsigned int N);

inline CComplex::CComplex()
{
}

inline CComplex::CComplex(double _a, double _b)
{
	a = _a;
	b = _b;
}

inline CComplex::CComplex(const CComplex& complex)
{
	a = complex.a;
	b = complex.b;
}

inline CComplex CComplex::operator+(const CComplex& complex) const
{
	return CComplex(a + complex.a, b + complex.b);
}

inline CComplex CComplex::operator-(const CComplex& complex) const
{
	return CComplex(a - complex.a, b - complex.b);
}

inline CComplex CComplex::operator*(const CComplex& complex) const
{
	return CComplex(a * complex.a - b * complex.b,
					a * complex.b + b * complex.a);
}

inline CComplex CComplex::operator/(const CComplex& complex) const
{
	double norm = complex.a * complex.a + complex.b * complex.b;

	return CComplex((a * complex.a + b * complex.b) / norm,
					(b * complex.a - a * complex.b) / norm);
}

inline void CComplex::operator+=(const CComplex& complex)
{
	a += complex.a;
	b += complex.b;
}

inline void CComplex::operator-=(const CComplex& complex)
{
	a -= complex.a;
	b -= complex.b;
}

inline void CComplex::operator*=(const CComplex& complex)
{
	double _a = a * complex.a - b * complex.b;
	double _b = a * complex.b + b * complex.a;
	a = _a; b = _b;
}

inline void CComplex::operator/=(const CComplex& complex)
{
	double norm = complex.a * complex.a + complex.b * complex.b;

	double _a = (a * complex.a + b * complex.b) / norm;
	double _b = (b * complex.a - a * complex.b) / norm;
	a = _a; b = _b;
}

inline void CComplex::operator=(const CComplex& complex)
{
	a = complex.a;
	b = complex.b;
}

inline CComplex CComplex::operator*(double value) const
{
	return CComplex(a * value, b * value);
}

inline CComplex CComplex::operator/(double value) const
{
	return CComplex(a / value, b / value);
}

inline void CComplex::operator*=(double value)
{
	a *= value;
	b *= value;
}

inline void CComplex::operator/=(double value)
{
	a /= value;
	b /= value;
}

inline double CComplex::GetProduct()
{
	return a * a + b * b;
}

inline double CComplex::GetModule()
{
	return sqrt(a * a + b * b);
}

#endif
