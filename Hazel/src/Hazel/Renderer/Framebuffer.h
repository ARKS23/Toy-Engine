#pragma once

#include "Hazel/Core/Core.h"

namespace Hazel {
	enum class FramebufferTextureFormat {
		None = 0,

		// Color Format
		RGBA8,
		RED_INTEGER,	// 鼠标拾取时存物品ID用这个通道

		// Depth / stencil
		DEPTH24STENCIL8,

		// Defaults
		Depth = DEPTH24STENCIL8
	};

	/* 纹理枚举格式的简单封装，待拓展 */
	struct FramebufferTextureSpecification {
		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat format) : TextureFormat(format) {}

		FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;
		
		// TODO: 待补充 filtering/warp
	};

	/* 对TextureSpecification的列表封装，支持MRT多目标渲染 */
	struct FramebufferAttachmentSpecification {
		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments) : Attachments(attachments) {}

		std::vector<FramebufferTextureSpecification> Attachments;
	};

	/* 配置结构体 */
	struct FramebufferSpecification {
		uint32_t Width = 0, Height = 0;					// 帧缓冲的分辨率
		FramebufferAttachmentSpecification Attachments;	// 附件列表
		uint32_t samples = 1;							// MSAA

		bool SwapChainTarget = false;					// 是否直接画到屏幕上，用来控制离屏渲染
	};

	class HAZEL_API Framebuffer {
	public:
		virtual ~Framebuffer() = default;

		virtual void Bind() = 0;
		virtual void UnBind() = 0;

		virtual void Resize(uint32_t width, uint32_t height) = 0; // 编辑器中拖动视口改变分辨率时调用,让纹理跟随分辨率
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) = 0; // 参数：要读取的纹理，x、y鼠标坐标，返回物体ID

		virtual void ClearAttachment(uint32_t attachmentIndex, int value) = 0; // 清空某个特定的附件

		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const = 0; // 返回某个纹理ID

		virtual const FramebufferSpecification& GetSpecification() const = 0;	// 返回配置结构体

		static Ref<Framebuffer> Create(const FramebufferSpecification& spec);

	};
}

