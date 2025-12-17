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