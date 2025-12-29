#include "hzpch.h"
#include "OpenGLFramebuffer.h"


namespace Hazel {
	static const uint32_t s_MaxFramebufferSize = 8192;
	
	namespace Utils {
		/* 封装一些底层OpenGL调用 */
		static GLenum TextureTarget(bool multisampled) {
			return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
		}

		static void CreateTextures(bool multisampled, uint32_t* outID, uint32_t count) {
			glCreateTextures(TextureTarget(multisampled), count, outID);
		}

		static void BindTexture(bool multisampled, uint32_t id) {
			glBindTexture(TextureTarget(multisampled), id);
		}

		static void AttachColorTexture(uint32_t id, int samples, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, int index) {
			/* 负责创建颜色纹理 */
			bool multisampled = samples > 1;
			if (multisampled) {
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);
			}
			else {
				glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);
				
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0);
		}

		static void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height) {
			/* 负责创建深度纹理 */
			bool multisampled = samples > 1;
			if (multisampled) {
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);
			}
			else {
				glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), id, 0);
		}

		static bool IsDepthFormat(FramebufferTextureFormat format) {
			/* 判断是否为深度图格式 */
			switch (format)
			{
				case FramebufferTextureFormat::DEPTH24STENCIL8:  return true;
			}

			return false;
		}

		static GLenum HazelFBTextureFormatToGL(FramebufferTextureFormat format)
		{
			/* Hazel格式转OpenGL的格式 */
			switch (format)
			{
				case FramebufferTextureFormat::RGBA8:       return GL_RGBA8;
				case FramebufferTextureFormat::RED_INTEGER: return GL_RED_INTEGER;
			}

			HZ_CORE_ASSERT(false, "HazelFBTextureFormatToGL ERROR");
			return 0;
		}
	}

	OpenGLFramebuffer::OpenGLFramebuffer(const FramebufferSpecification& spec) : m_Specification(spec){
		for (auto specification : spec.Attachments.Attachments) {
			if (!Utils::IsDepthFormat(specification.TextureFormat))	// 区分颜色和深度纹理格式
				m_ColorAttachmentSpecifications.emplace_back(specification);
			else
				m_DepthAttachmentSpecification = specification;
		}

		Invalidate();
	}

	OpenGLFramebuffer::~OpenGLFramebuffer() {
		glDeleteFramebuffers(1, &m_RendererID);
		glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
		glDeleteTextures(1, &m_DepthAttachment);
	}

	void OpenGLFramebuffer::Bind() {
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
		glViewport(0, 0, m_Specification.Width, m_Specification.Height); // 记得设置分辨率
	}

	void OpenGLFramebuffer::UnBind() {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Invalidate() {
		// 纹理大小是固定的，一旦窗口大小改变，旧的纹理就需要销毁然后创建新的， 初始化和Resize都会调用
		if (m_RendererID) {
			glDeleteFramebuffers(1, &m_RendererID);
			glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());
			glDeleteTextures(1, &m_DepthAttachment);

			m_ColorAttachments.clear(); // 清空句柄
			m_DepthAttachment = 0;
		}

		// 创建新的FBO
		glCreateFramebuffers(1, &m_RendererID);
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

		bool multisample = m_Specification.samples > 1;

		// 创建和挂载颜色附件
		if (m_ColorAttachmentSpecifications.size()) {
			m_ColorAttachments.resize(m_ColorAttachmentSpecifications.size());
			Utils::CreateTextures(multisample, m_ColorAttachments.data(), m_ColorAttachments.size());

			for (size_t i = 0; i < m_ColorAttachments.size(); ++i) {
				Utils::BindTexture(multisample, m_ColorAttachments[i]);
				switch (m_ColorAttachmentSpecifications[i].TextureFormat)
				{
					case FramebufferTextureFormat::RGBA8:
						Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.samples, GL_RGBA8, GL_RGBA, m_Specification.Width, m_Specification.Height, i);
						break;
					case FramebufferTextureFormat::RED_INTEGER:
						Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.samples, GL_R32I, GL_RED_INTEGER, m_Specification.Width, m_Specification.Height, i);
						break;
				}
			}
		}

		if (m_DepthAttachmentSpecification.TextureFormat != FramebufferTextureFormat::None) {
			Utils::CreateTextures(multisample, &m_DepthAttachment, 1);
			Utils::BindTexture(multisample, m_DepthAttachment);
			switch (m_DepthAttachmentSpecification.TextureFormat) 
			{
				case FramebufferTextureFormat::DEPTH24STENCIL8:
					Utils::AttachDepthTexture(m_DepthAttachment, m_Specification.samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Specification.Width, m_Specification.Height);
					break;
			}
		}

		/* 告诉OpenGL的输出规则 */
		if (m_ColorAttachments.size() > 1) {
			HZ_CORE_ASSERT(m_ColorAttachments.size() <= 4, "framebuffer out of range!");
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(m_ColorAttachments.size(), buffers);
		}
		else if (m_ColorAttachments.empty()) {
			glDrawBuffer(GL_NONE); // 没有颜色附件，不画颜色
		}

		// 完整性检查
		HZ_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete!");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void OpenGLFramebuffer::Resize(uint32_t width, uint32_t height) {
		// 合法性检查
		if (width == 0 || height == 0 || width > s_MaxFramebufferSize || height > s_MaxFramebufferSize) {
			HZ_CORE_WARN("Attempted to rezize framebuffer to {0}, {1}", width, height);
			return;
		}

		m_Specification.Width = width;
		m_Specification.Height = height;
		Invalidate();	// 记得重构一下纹理对象
	}

	int OpenGLFramebuffer::ReadPixel(uint32_t attachmentIndex, int x, int y) {
		HZ_CORE_ASSERT(attachmentIndex < m_ColorAttachments.size(), "ReadPixel out of range!");

		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex); // 告诉OpenGL读哪张图
		int pixelData;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData); // 1,1是代表1 x 1的区域
		return pixelData;
	}

	void OpenGLFramebuffer::ClearAttachment(uint32_t attachmentIndex, int value) {
		/* 每帧开始时清理物品id时要使用 */
		HZ_CORE_ASSERT(attachmentIndex < m_ColorAttachments.size(), "ClearAttachment out of range!");

		FramebufferTextureSpecification& spec = m_ColorAttachmentSpecifications[attachmentIndex];
		glClearTexImage(m_ColorAttachments[attachmentIndex], 0, Utils::HazelFBTextureFormatToGL(spec.TextureFormat), GL_INT, &value);
	}

	uint32_t OpenGLFramebuffer::GetColorAttachmentRendererID(uint32_t index /*= 0*/) const {
		HZ_CORE_ASSERT(index < m_ColorAttachments.size(), "GetColorAttachmentRendererID out of range!");
		return m_ColorAttachments[index];
	}

	const Hazel::FramebufferSpecification& OpenGLFramebuffer::GetSpecification() const {
		return m_Specification;
	}

}