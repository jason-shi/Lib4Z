#ifndef __BezierND_h__
#define __BezierND_h__

#include "BezierBase.h"

class CBezierND : public CBezierBase {
  public:
	CBezierND(int nCount, LPPOINT lpPoints);
	virtual ~CBezierND();

  private:
	void			 DeCasteljau(int ptx[], int pty[]);
	bool			 CheckTermination(int ptx[], int pty[]);
};

#endif
