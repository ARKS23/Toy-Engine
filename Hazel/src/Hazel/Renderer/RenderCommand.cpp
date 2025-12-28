#include "hzpch.h"
#include "RenderCommand.h"

namespace Hazel {

	void RenderCommand::Init() {
		s_RendererAPI->Init();
	}

	void RenderCommand::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
		s_RendererAPI->SetViewport(x, y, width, height);
	}

	void RenderCommand::SetClearColor(const glm::vec4& color) {
		s_RendererAPI->SetClearColor(color);
	}

	void RenderCommand::Clear() {
		s_RendererAPI->Clear();
	}

	void RenderCommand::DrawIndexed(const Ref<VertexArray>& vertexArray, uint32_t indexCount /*= 0*/) {
		s_RendererAPI->DrawIndex(vertexArray, indexCount);
	}

	void RenderCommand::DrawLines(const Ref<VertexArray>& VertexArray, uint32_t vertexCount) {
		s_RendererAPI->DrawLines(VertexArray, vertexCount);
	}

	void RenderCommand::DrawTriangles(const Ref<VertexArray>& VertexArray, uint32_t vertexCount)
	{
		s_RendererAPI->DrawTriangles(VertexArray, vertexCount);
	}

	void RenderCommand::SetLineWidth(float width) {
		s_RendererAPI->SetLineWidth(width);
	}

	void RenderCommand::SetDepthFunc(RendererAPI::DepthFunc func) {
		s_RendererAPI->SetDepthFunc(func);
	}

	Hazel::Scope<Hazel::RendererAPI> RenderCommand::s_RendererAPI = RendererAPI::Create();
}
