#pragma once

/* 该头文件用于设置DLL宏(HAZEL_API) */

// 检查是否为windwos系统
#ifdef HZ_PLATFORM_WINDOWS
	// 检查是引擎dllexport还是客户端dllimport
	#ifdef HZ_BUILD_DLL
		#define	HAZEL_API __declspec(dllexport)
	#else
		#define	HAZEL_API __declspec(dllimport)
	#endif
#else
	#error Hazel only support Windows;
#endif

#define BIT(x) (1 << x)
#define HZ_BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

#ifdef HZ_ENABLE_ASSERTS
	#define HZ_ASSERT(x, ...) { if(!(x))  { HZ_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); }}
	#define HZ_CORE_ASSERT(x, ...) { if(!(x))  { HZ_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); }}
#else
	#define HZ_ASSERT(x, ...)
	#define HZ_CORE_ASSERT(x, ...)
#endif