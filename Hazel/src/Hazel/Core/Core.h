#pragma once

#include <memory>

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

/* --------------------------------------- 辅助函数宏 --------------------------------------- */
#define BIT(x) (1 << x)
#define HZ_BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)


/* --------------------------------------- 断言检查宏 --------------------------------------- */
#ifdef HZ_DEBUG
	#define HZ_ENABLE_ASSERTS
#endif

#ifdef HZ_ENABLE_ASSERTS
	#define HZ_ASSERT(x, ...) { if(!(x))  { HZ_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); }}
	#define HZ_CORE_ASSERT(x, ...) { if(!(x))  { HZ_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); }}
#else
	#define HZ_ASSERT(x, ...)
	#define HZ_CORE_ASSERT(x, ...)
#endif

/* --------------------------------------- 智能指针封装 --------------------------------------- */
namespace Hazel {
	template<typename T>
	using Scope = std::unique_ptr<T>; // 用unique_ptr限制作用域

	template<typename T, typename ...Args>
	constexpr Scope<T> CreateScope(Args&& ... args) {
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	
	template<typename T, typename ...Args>
	constexpr Ref<T> CreateRef(Args&& ...args) {
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}