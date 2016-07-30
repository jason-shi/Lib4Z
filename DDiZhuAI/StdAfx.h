#ifdef	_MSC_VER

#pragma	warning(disable: 4201)
#pragma	warning(disable: 4100)
#pragma	warning(disable: 4786)
#pragma	warning(disable: 4800)
#pragma	warning(disable: 4996)
#endif

typedef	unsigned char		byte;
typedef unsigned short		word;
typedef unsigned int		dword;

#include <vector>
#include <time.h>

#include <ZDesign.h>

#include <Common.h>
#include <Table.h>
#include <Logic\RSpecial\RSpecial.h>
#include <Logic\RCrack\RCrack.h>

#include <DDiZhuAI.h>

#ifdef	_MSC_VER
	#ifdef	_DEBUG
		#pragma comment(lib, "ZDesignD.lib")
	#else
		#pragma comment(lib, "ZDesign.lib")
	#endif
#endif
