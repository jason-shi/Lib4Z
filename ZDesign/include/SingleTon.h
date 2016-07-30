#ifndef __SINGLETON_h__
#define __SINGLETON_h__

#ifndef NULL
#define NULL 0
#endif

#ifndef SINGLETON
#define SINGLETON(type) \
	static inline type* GetInstancePtr() \
	{ \
		static type _instance; \
		return &_instance; \
	}
#endif

//OOѵ��Ӫ��fyj�Դ���д��
#ifndef SLUGINMODE
#define SLUGINMODE(type, func, param) \
	static inline type& func() \
	{ \
		static type _instance(param); \
		return _instance; \
	}
#endif

#endif
