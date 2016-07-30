#include "StdAfx.h"
#include "Algebra.h"

CComplex operator*(double value, const CComplex& complex)
{
	return CComplex(value * complex.a, value * complex.b);
}

CComplex cpower(const CComplex& complex, unsigned int N)
{
	CComplex result = complex;
	for (unsigned int i = 1; i < N; i++)
		result *= complex;

	return result;
}
