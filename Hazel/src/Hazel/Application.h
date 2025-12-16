#pragma once

#include "Core.h"

namespace Hazel {

	class HAZEL_API Application {
	public:
		Application();
		virtual ~Application();

		void Run();
	};

	// 该函数由客户端实现
	Application* CreateApplication();
};

