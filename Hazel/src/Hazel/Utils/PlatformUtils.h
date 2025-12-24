#pragma once
#include "Hazel/Core/Core.h"
#include <string>

namespace Hazel {
	class HAZEL_API Time {
	public:
		static float GetTime(); // 目前在WindowsPlatformUtils.cpp中实现
	};
}