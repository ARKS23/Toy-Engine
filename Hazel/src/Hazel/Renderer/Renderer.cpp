#include "hzpch.h"
#include "Renderer.h"

#include "Hazel/Renderer/RenderCommand.h"

namespace Hazel {
	void Renderer::Init() {
		RenderCommand::Init();
	}

	void Renderer::Shutdown() {
	
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height) {
		RenderCommand::SetViewport(0, 0, width, height);
	}

	void Renderer::BeginScene(Camera& camera) {
		s_SceneData->ViewProjectMatrix = camera.GetViewProjetionMatrix();
	}

	void Renderer::EndScene() {

	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform /*= glm::mat4(1.f)*/) {
		shader->Bind();
		shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectMatrix);
		shader->SetMat4("u_Transform", transform);

		vertexArray->Bind();
		RenderCommand::DrawIndexed(vertexArray);
	}

	Scope<Renderer::SceneData> Renderer::s_SceneData = CreateScope<Renderer::SceneData>(); // 静态成员初始化
}