#include "hzpch.h"
#include "OpenGLUniformBuffer.h"

namespace Hazel {

	OpenGLUniformBuffer::OpenGLUniformBuffer(uint32_t size, uint32_t binding) {
		glCreateBuffers(1, &m_RendererID); // 不同于旧版本的glgen + bind
		glNamedBufferData(m_RendererID, size, nullptr, GL_DYNAMIC_DRAW); // 不同于旧版本的glBindBuffer + hlBufferData + glBindBuffer
		glBindBufferBase(GL_UNIFORM_BUFFER, binding, m_RendererID); // UBO设置binding slot
	}

	OpenGLUniformBuffer::~OpenGLUniformBuffer() {
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLUniformBuffer::SetData(const void* data, uint32_t size, uint32_t offset /* = 0 */) {
		glNamedBufferSubData(m_RendererID, offset, size, data); // 不同于旧版本 bind + glBufferSubData + bind
	}
}