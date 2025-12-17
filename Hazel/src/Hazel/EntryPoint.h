#pragma once

#ifdef HZ_PLATFORM_WINDOWS

// export声明从外部链接时找到函数实体，该函数由客户端实现
// 这样做可以实现控制反转，让引擎接管整个流程
extern Hazel::Application* Hazel::CreateApplication();

int main(int argc, char** argv) {
	Hazel::Log::Init();
	HZ_CORE_INFO("INIT CORE LOGGER");
	HZ_WARN("INIT CLIENT LOGGER");

	auto app = Hazel::CreateApplication();
	app->Run();
	delete app;
}

#endif