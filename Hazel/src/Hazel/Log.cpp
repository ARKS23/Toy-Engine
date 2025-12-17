#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h" // 引入带颜色的控制台输出器

namespace Hazel {
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;

	void Log::Init() {
		//设置日志模式：颜色 | 时间戳 | logger名字 | 具体的日志信息
		spdlog::set_pattern("%^[%T] %n: %v%$");
		
		// 1. 初始化 Core Logger
		s_CoreLogger = spdlog::stdout_color_mt("HAZEL");
		s_CoreLogger->set_level(spdlog::level::trace);

		// 2. 初始化 Client Logger
		s_ClientLogger = spdlog::stdout_color_mt("App");
		s_ClientLogger->set_level(spdlog::level::trace);
	}
};
