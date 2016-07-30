#ifndef __JuliaCDS_h__
#define __JuliaCDS_h__

#include "MandelCDS.h"

class CJuliaCDS : public CMandelCDS {
  public:
	CJuliaCDS(int N, const CComplex& C0);
	virtual ~CJuliaCDS();

  public:
	void			 Draw(CCanvas *pCanvas);

  protected:
	CComplex		 C0;
};

#endif
