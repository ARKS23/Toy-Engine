#include "hzpch.h"
#include "OpenGLRendererAPI.h"

namespace Hazel {

	void OpenGLRendererAPI::Init() {
		// TODO: ´ý²¹³ä
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // ÉèÖÃ»ìºÏ·½Ê½

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_LINE_SMOOTH); // Ïß¶Î¿¹¾â³Ý
	}

	void OpenGLRendererAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
		glViewport(x, y, width, height);
	}

	void OpenGLRendererAPI::SetClearColor(const glm::vec4& color) {
		glClearColor(color.r, color.g, color.b, color.a);
	}

	void OpenGLRendererAPI::Clear() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void OpenGLRendererAPI::DrawIndex(const Ref<VertexArray>& VertexArray, uint32_t indexCount /*= 0*/) {
		VertexArray->Bind();
		uint32_t count = indexCount ? indexCount : VertexArray->GetIndexBuffer()->GetCount();
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
	}

	void OpenGLRendererAPI::DrawLines(const Ref<VertexArray>& VertexArray, uint32_t vertexCount) {
		VertexArray->Bind();
		glDrawArrays(GL_LINES, 0, vertexCount);
	}

	void OpenGLRendererAPI::DrawTriangles(const Ref<VertexArray>& VertexArray, uint32_t vertexCount) {
		VertexArray->Bind();
		glDrawArrays(GL_TRIANGLES, 0, vertexCount);
	}

	void OpenGLRendererAPI::SetLineWidth(float width) {
		glLineWidth(width);
	}

	void OpenGLRendererAPI::SetDepthFunc(DepthFunc func) {
		GLenum glFunc = GL_LESS;
		switch (func)
		{
			case Hazel::RendererAPI::DepthFunc::Never:
				glFunc = GL_NEVER;
				break;
			case Hazel::RendererAPI::DepthFunc::Less:
				glFunc = GL_LESS;
				break;
			case Hazel::RendererAPI::DepthFunc::Equal:
				glFunc = GL_EQUAL;
				break;
			case Hazel::RendererAPI::DepthFunc::Lequal:
				glFunc = GL_LEQUAL;
				break;
			case Hazel::RendererAPI::DepthFunc::Greater:
				glFunc = GL_GREATER;
				break;
			case Hazel::RendererAPI::DepthFunc::Notequal:
				glFunc = GL_NOTEQUAL;
				break;
			case Hazel::RendererAPI::DepthFunc::Gequal:
				glFunc = GL_GEQUAL;
				break;
			case Hazel::RendererAPI::DepthFunc::Always:
				glFunc = GL_ALWAYS;
				break;
			default:
				HZ_CORE_ASSERT(false, "Unknown Depth Func");
		}
		glDepthFunc(glFunc);
	}

	void OpenGLRendererAPI::SetDepthMask(bool writable) {
		glDepthMask(writable ? GL_TRUE : GL_FALSE);
	}

}
