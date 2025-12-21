#pragma once

#include "Hazel/Renderer/RendererAPI.h"

namespace Hazel {
	class Renderer {
	public:
		static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
	};
}

