#pragma once

#include "Hazel/Renderer/RendererAPI.h"
#include "Hazel/Renderer/Shader.h"

namespace Hazel {
	class Renderer {
	public:
		static void Init();
		static void Shutdown();

		static void OnWindowResize(uint32_t width, uint32_t height); // 窗口大小改变时调用的函数

		/* BeginScene和EndScene控制渲染每一帧 */
		static void BeginScene(); // 待补充
		static void EndScene();

		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.f)); // 提交数据进行渲染

		static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }

	private:
		struct SceneData
		{
			glm::mat4 ViewProjectMatrix;
		};
		
		static Scope<SceneData> s_SceneData;  // 目前设计:场景数据管理，全局通用
	};
}

