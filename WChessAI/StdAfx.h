#ifndef __StdAfx_h__
#define __StdAfx_h__

#ifdef	_MSC_VER
#pragma	warning(disable: 4100)
#pragma	warning(disable: 4146)
#pragma	warning(disable: 4201)
#pragma	warning(disable: 4786)
#pragma	warning(disable: 4800)
#pragma	warning(disable: 4996)
#endif

#define PRIVATE_NAME			WChessAI

namespace PRIVATE_NAME
{
}
using namespace PRIVATE_NAME;

#include <vector>
#include <time.h>

#include <ZOmy.h>
#include <WinLib.h>
#include <ZDesign.h>

#include <WChessAI.h>
#include <Table.h>
#include <WChess.h>

#ifdef	_MSC_VER
	#ifdef	_DEBUG
		#pragma comment(lib, "ZOmyD.lib")
		#pragma comment(lib, "WinLibD.lib")
		#pragma comment(lib, "ZDesignD.lib")
	#else
		#pragma comment(lib, "ZOmy.lib")
		#pragma comment(lib, "WinLib.lib")
		#pragma comment(lib, "ZDesign.lib")
	#endif
#endif

#endif
