#include "StdAfx.h"
#include "NovaCDS.h"

CNovaCDS::CNovaCDS(int N, const CComplex& C0, const CComplex& R, const CComplex& Z0)
			: CNewtonCDS(N, C0), R(R), Z0(Z0)
{
}

CNovaCDS::~CNovaCDS()
{
}
