#pragma once

#include "Hazel/Renderer/Framebuffer.h"

namespace Hazel {
	class OpenGLFramebuffer : public Framebuffer{
	public:
		OpenGLFramebuffer(const FramebufferSpecification& spec);
		virtual ~OpenGLFramebuffer();

	public:
		virtual void Bind() override;
		virtual void UnBind() override;

		void Invalidate();

		virtual void Resize(uint32_t width, uint32_t height) override; // 编辑器中拖动视口改变分辨率时调用,让纹理跟随分辨率
		virtual int ReadPixel(uint32_t attachmentIndex, int x, int y) override; // 参数：要读取的纹理，x、y鼠标坐标，返回物体ID

		virtual void ClearAttachment(uint32_t attachmentIndex, int value) override; // 清空某个特定的附件

		virtual uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const override; // 返回某个纹理ID

		virtual const FramebufferSpecification& GetSpecification() const override;	// 返回配置结构体

	private:
		uint32_t m_RendererID = 0;		// FBO
		FramebufferSpecification m_Specification;	// 配置结构

		std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecifications; // 颜色附件规格列表,这个列表不变
		std::vector<uint32_t> m_ColorAttachments;	// OpenGL的纹理ID列表,这个列表可能会重新创建和加载

		/* 深度附件 */
		FramebufferTextureSpecification m_DepthAttachmentSpecification = FramebufferTextureFormat::None;
		uint32_t m_DepthAttachment = 0;
	};
}

