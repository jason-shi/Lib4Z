#ifndef __ZDesign_h__
#define __ZDesign_h__

#include <SingleTon.h>
#include <Water.h>
#include <ZPool.h>
#include <ZDeposit.h>
#include <RspbManage.h>
#include <CmdManage.h>
#include <StateManage.h>


#ifdef	_MSC_VER
	#ifdef	_DEBUG
		#pragma comment(lib, "ZDesignD.lib")
	#else
		#pragma comment(lib, "ZDesign.lib")
	#endif
#endif

#endif
