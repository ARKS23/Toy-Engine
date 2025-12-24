#pragma once

#include "Hazel/Renderer/Buffer.h"

namespace Hazel {
	class HAZEL_API VertexArray {
	public:
		virtual ~VertexArray() = default;

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual void AddVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) = 0; // 添加VBO
		virtual void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) = 0; // 添加EBO

		virtual const std::vector<Ref<VertexBuffer>>& GetVertexBuffers() const = 0; // 获取当前持有的VBO Buffer
		virtual const Ref<IndexBuffer>& GetIndexBuffer() const = 0;  // 获取当前的EBO

	public:
		static Ref<VertexArray> Create();
	};
}

