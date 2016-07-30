#ifndef	__StateManage_h__
#define	__StateManage_h__

#include <SingleTon.h>
#include <boost\function.hpp>

class IState {
  public:
	IState();
	virtual ~IState();
};

class IStateContext {
  public:
	IStateContext();
	virtual ~IStateContext();
};

#endif
