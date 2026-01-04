#include "hzpch.h"
#include "Renderer.h"

#include "Hazel/Renderer/RenderCommand.h"

namespace Hazel {
	void Renderer::Init() {
		HZ_PROFILE_FUNCTION();

		RenderCommand::Init();
		//Renderer2D::Init();
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

	void Renderer::DrawSkybox(const Ref<Shader>& shader, const Ref<VertexArray>& va, const Ref<TextureCubeMap>& skyboxTexture) {
		RenderCommand::SetDepthFunc(RendererAPI::DepthFunc::Lequal); // 天空盒最大深度需要
		shader->Bind();
		shader->SetInt("u_SkyboxSampler", 0);
		skyboxTexture->Bind(0); // sampler槽位设置
		va->Bind();
		//RenderCommand::DrawIndexed(va);
		RenderCommand::DrawTriangles(va, 36);
		RenderCommand::SetDepthFunc(RendererAPI::DepthFunc::Less);	// 恢复正常比较
	}

	Scope<Renderer::SceneData> Renderer::s_SceneData = CreateScope<Renderer::SceneData>(); // 静态成员初始化
}