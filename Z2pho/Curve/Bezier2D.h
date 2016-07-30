#ifndef __Bezier2D_h__
#define __Bezier2D_h__

#include "BezierBase.h"

class CBezier2D : public CBezierBase {
  public:
	CBezier2D(int nPoint, LPPOINT lpPoints);
	virtual ~CBezier2D();

  private:
	void			 DeCasteljau(int ptx[], int pty[]);
	bool			 CheckTermination(int ptx[], int pty[]);
};

#endif
