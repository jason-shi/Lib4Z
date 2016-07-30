#ifdef	_MSC_VER

#pragma	warning(disable: 4201)
#pragma	warning(disable: 4100)
#pragma	warning(disable: 4786)
#pragma	warning(disable: 4800)
#pragma	warning(disable: 4996)
#endif

#define PRIVATE_NAME			Dhcpd

#include "PRIVATE_NAMESPACE_BEGIN"
#include "PRIVATE_NAMESPACE_END"

using namespace PRIVATE_NAME;

#include <vector>
#include <list>
#include <time.h>

#include <ZOmy.h>
#include <WinLib.h>
#include <ZDesign.h>
#include <ZLexer.h>
#include <JProc.h>
#include <ZTimer.h>

#include <Common.h>

#include <DLinkPub.h>
#include <DhcpPub.h>

#ifdef	_MSC_VER
	#ifdef	_DEBUG
		#pragma comment(lib, "ZOmyD.lib")
		#pragma comment(lib, "WinLibD.lib")
		#pragma comment(lib, "ZDesignD.lib")
		#pragma comment(lib, "ZLexerD.lib")
		#pragma comment(lib, "JProcD.lib")
		#pragma comment(lib, "ZTimerD.lib")
	#else
		#pragma comment(lib, "ZOmy.lib")
		#pragma comment(lib, "WinLib.lib")
		#pragma comment(lib, "ZDesign.lib")
		#pragma comment(lib, "ZLexer.lib")
		#pragma comment(lib, "JProc.lib")
		#pragma comment(lib, "ZTimer.lib")
	#endif
#endif
