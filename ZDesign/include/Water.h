#ifndef __Water_h__
#define __Water_h__

#include <boost/function.hpp>

class CWater;
typedef boost::function<bool(CWater *pWater, void *lpParam)>		 FN_WaterProc;

class CWater {
  public:
	CWater();
	virtual ~CWater();

  public:
	virtual bool	 Initial();
	virtual bool	 Release();
};

inline bool CWater::Initial()
{
	return true;
}

inline bool CWater::Release()
{
	return true;
}

#endif
