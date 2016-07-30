#ifndef __Bezier3D_h__
#define __Bezier3D_h__

#include "BezierBase.h"

class CBezier3D : public CBezierBase {
  public:
	CBezier3D(int nPoint, LPPOINT lpPoints);
	virtual ~CBezier3D();

  private:
	void			 DeCasteljau(int ptx[], int pty[]);
	bool			 CheckTermination(int ptx[], int pty[]);
};

#endif
