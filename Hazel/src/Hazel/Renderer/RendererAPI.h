#pragma once

#include "Hazel/Renderer/VertexArray.h"

#include <glm/glm.hpp>

namespace Hazel {
	class RendererAPI {
	public:
		enum class API {
			// 定义支持的API
			None = 0,
			OpenGL = 1
		};

	public:
		virtual ~RendererAPI() = default;

		virtual void Init() = 0; // 初始化:开启混合，深度测试等
		virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0; // 视口设置
		virtual void SetClearColor(const glm::vec4& color) = 0; // 设置清屏颜色
		virtual void Clear() = 0; // 清屏操作

		/* 核心绘制指令 */
		virtual void DrawIndex(const Ref<VertexArray>& VertexArray, uint32_t indexCount = 0) = 0;	// 用索引来绘制，默认传入0表示绘制整个EBO
		virtual void DrawLines(const Ref<VertexArray>& VertexArray, uint32_t vertexCount) = 0;		// 调试渲染，绘制线框

		virtual void SetLineWidth(float width) = 0; // 设置线框大小

		static API GetAPI() { return s_API; } // 静态方法获取当前API
		static Scope<RendererAPI> Create();	  // 静态工厂方法

	private:
		static API s_API;
	};
}

