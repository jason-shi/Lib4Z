#ifndef __WinLib_h__
#define __WinLib_h__

#pragma warning(disable: 4996)

#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#if defined(_MSC_VER) && defined(_DEBUG)
#include <crtdbg.h>
#define	DEBUG_NEW	new(_NORMAL_BLOCK, __FILE__, __LINE__)
#else
#define	DEBUG_NEW	new
#endif
#include <Application.h>
#include <Window.h>
#include <dc.h>
#include <Mci.h>
#include <Algebra.h>
#include <Geometry.h>
#include <File.h>
#include <Misc.h>
#include <SyncObj.h>

#ifdef	_MSC_VER
	#ifdef	_DEBUG
		#pragma comment(lib, "WinLibD.lib")
	#else
		#pragma comment(lib, "WinLib.lib")
	#endif
#endif

#endif
