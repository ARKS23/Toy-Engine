#include "hzpch.h"
#include "RendererAPI.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Hazel {
	RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;  // hard_code, 之后增加vulkan再进行读文件配置

	Scope<RendererAPI> RendererAPI::Create() {
		/* 静态工厂方法创建对应的API */
		switch (s_API)
		{
			case Hazel::RendererAPI::API::None:
				HZ_CORE_ERROR("Unknow Renderer API!");
				return nullptr;
			case Hazel::RendererAPI::API::OpenGL:
				return CreateScope<OpenGLRendererAPI>();
		}

		HZ_CORE_ERROR("Unknow Renderer API!");
		return nullptr;
	}
}