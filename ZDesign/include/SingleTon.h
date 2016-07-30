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

//OO训练营，fyj自创该写法
#ifndef SLUGINMODE
#define SLUGINMODE(type, func, param) \
	static inline type& func() \
	{ \
		static type _instance(param); \
		return _instance; \
	}
#endif

#endif
