#pragma once
#include "Hazel/Core/Core.h"
#include "Hazel/Renderer/RendererAPI.h"

namespace Hazel {
	/* 这个类调用图形API: 不关心底层使用什么API */
	class RenderCommand {
	public:
		static void Init();
		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
		static void SetClearColor(const glm::vec4& color);
		static void Clear();
		static void DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount = 0);
		static void DrawLines(const Ref<VertexArray>& VertexArray, uint32_t vertexCount);
		static void SetLineWidth(float width);

	private:
		static Scope<RendererAPI> s_RendererAPI; // 编译文件中初始化
	};
}

