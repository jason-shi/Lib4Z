#ifndef __CPUID_h__
#define __CPUID_h__

class CCPUID {
  public:
	long			 GetCPUFreq();
	std::string		 GetManID();
	std::string		 GetCPUType();

	bool			 IsMMXSupport();
};

#endif
