#pragma once

#include "Hazel/Core/Core.h"

/* 与Learn OpenGL中的每次bind不同，这里采用UBO方案减少CPU与GPU的交互 */
namespace Hazel {
	class HAZEL_API UniformBuffer {
	public:
		virtual ~UniformBuffer() {}
		virtual void SetData(const void* data, uint32_t size, uint32_t offset = 0) = 0; //上传数据的接口

		static Ref<UniformBuffer> Create(uint32_t size, uint32_t binding); // 第二个参数是绑定点
	};
}
