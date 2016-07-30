#ifndef __ZProt_h__
#define __ZProt_h__

#pragma warning(disable: 4018)
#pragma warning(disable: 4996)

#include <stdlib.h>
#include <time.h>
#include <string>
#include <iostream>

#include <CPUID.h>
#include <HDSerial.h>
#include <Registry.h>
#include <BigNum.h>
#include <RSACrypt.h>

#ifdef	_MSC_VER
	#ifdef	_DEBUG
		#pragma comment(lib, "ZProtD.lib")
	#else
		#pragma comment(lib, "ZProt.lib")
	#endif
#endif

#endif
