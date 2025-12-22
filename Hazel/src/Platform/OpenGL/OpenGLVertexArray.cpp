#include "hzpch.h"
#include "OpenGLVertexArray.h"

namespace Hazel {
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
		/* 将Buffer.h中引擎定义的数据类型转换到OpenGL的类型 */
		switch (type)
		{
			case Hazel::ShaderDataType::Float:
				return GL_FLOAT;
			case Hazel::ShaderDataType::Float2:
				return GL_FLOAT;
			case Hazel::ShaderDataType::Float3:
				return GL_FLOAT;
			case Hazel::ShaderDataType::Float4:
				return GL_FLOAT;
			case Hazel::ShaderDataType::Mat3:
				return GL_FLOAT;
			case Hazel::ShaderDataType::Mat4:
				return GL_FLOAT;
			case Hazel::ShaderDataType::Int:
				return GL_INT;
			case Hazel::ShaderDataType::Int2:
				return GL_INT;
			case Hazel::ShaderDataType::Int3:
				return GL_INT;
			case Hazel::ShaderDataType::Int4:
				return GL_INT;
			case Hazel::ShaderDataType::Bool:
				return GL_BOOL;
		}

		HZ_CORE_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

	OpenGLVertexArray::OpenGLVertexArray() {
		glCreateVertexArrays(1, &m_RenderID); // 4.5+版本DSA风格
	}

	OpenGLVertexArray::~OpenGLVertexArray() {
		glDeleteVertexArrays(1, &m_RenderID);
	}

	void OpenGLVertexArray::Bind() const {
		glBindVertexArray(m_RenderID);
	}

	void OpenGLVertexArray::UnBind() const {
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) {
		/* 核心函数: 解析Layout */
		HZ_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no fucking layout!");

		glBindVertexArray(m_RenderID);
		vertexBuffer->Bind();

		// 解析layout: 逐个element设置vertexArrayPointer，内部的element的偏移量已经在layout中构造时预处理过
		const BufferLayout& layout = vertexBuffer->GetLayout();
		for (const BufferElement& element : layout) {
			// TODO优化: 设置成静态预分配
			std::unordered_set<ShaderDataType> floatSet({ ShaderDataType::Float, ShaderDataType::Float2, ShaderDataType::Float3, ShaderDataType::Float4 });
			std::unordered_set<ShaderDataType> intBoolSet({ ShaderDataType::Int, ShaderDataType::Int2, ShaderDataType::Int3, ShaderDataType::Int4, ShaderDataType::Bool});
			std::unordered_set<ShaderDataType> matSet({ ShaderDataType::Mat3, ShaderDataType::Mat4});

			if (floatSet.count(element.Type)) {
				glEnableVertexAttribArray(m_VertexBufferIndex); // 设置对应的槽，GLSL: layout (location = m_VertexBufferIndex)
				glVertexAttribPointer(
					m_VertexBufferIndex,	// 槽位
					element.GetComponentCount(), // 分量数，float3 -> 3
					ShaderDataTypeToOpenGLBaseType(element.Type), // 转成OpenGL的类型
					element.Normalized ? GL_TRUE : GL_FALSE, // 是否标准化
					layout.GetStride(), // 整体步长
					(const void*)element.Offset // 该element在内部的偏移量
				);
				m_VertexBufferIndex++; // 更新槽位给下个element使用
			}
			else if (intBoolSet.count(element.Type)) {
				glEnableVertexAttribArray(m_VertexBufferIndex);
				glVertexAttribIPointer( // 踩坑: 这里Int类型要加I,否则GLSL编译后转成float
					m_VertexBufferIndex,
					element.GetComponentCount(),
					ShaderDataTypeToOpenGLBaseType(element.Type),
					layout.GetStride(),
					(const void*)element.Offset
				);
				m_VertexBufferIndex++;
			}
			else if (matSet.count(element.Type)) {
				uint8_t count = element.GetComponentCount();
				for (uint8_t i = 0; i < count; ++i) { // 矩阵类型要占用多个槽，遍历处理。比如mat3等于3个float3
					glEnableVertexAttribArray(m_VertexBufferIndex);
					glVertexAttribPointer(
						m_VertexBufferIndex,
						count,
						ShaderDataTypeToOpenGLBaseType(element.Type),
						element.Normalized ? GL_TRUE : GL_FALSE,
						layout.GetStride(),
						(const void*)(element.Offset + sizeof(float) * count * i) // element内部的内部的layout布局
					);
					glVertexAttribDivisor(m_VertexBufferIndex, 1); // 实例化渲染
					m_VertexBufferIndex++;
				}
			}
			else {
				HZ_CORE_ASSERT(false, "Unknown ShaderDataType!");
			}
		}

		m_VertexBuffers.push_back(vertexBuffer); // 纳入buffers
	}

	void OpenGLVertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) {
		glBindVertexArray(m_RenderID);
		indexBuffer->Bind(); // 内部调用索引绑定
		m_IndexBuffer = indexBuffer;
	}
}