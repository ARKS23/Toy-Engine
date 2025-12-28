#include "hzpch.h"
#include "OpenGLTexture.h"

#include <stb_image.h>

namespace Hazel {
	namespace Utils {
		/* 工程架构权衡: 类型枚举强依赖OpenGL，不单独写到通用的工具类中(踩坑God class) */
		static GLenum HazelImageFormatToGLDataFormat(ImageFormat format) {
			switch (format)
			{
				case ImageFormat::RGB8: return GL_RGB;
				case ImageFormat::RGBA8: return GL_RGBA;
			}

			HZ_CORE_ASSERT(false, "HazelImageFormatToGLDataFormat Error");
			return 0;
		}

		static GLenum HazelImageFormatToGLInteralFormat(ImageFormat format) {
			switch (format)
			{
				case ImageFormat::RGB8 : return GL_RGB8;
				case ImageFormat::RGBA8: return GL_RGBA8;
			}

			HZ_CORE_ASSERT(false, "HazelImageFormatToGLInteralFormat Error");
			return 0;
		}
	}


	OpenGLTexture2D::OpenGLTexture2D(const TextureSpecification& specification) 
		: m_Specification(specification), m_Width(specification.Width), m_Height(specification.Height)
	{
		m_InternalFormat = Utils::HazelImageFormatToGLInteralFormat(m_Specification.Format);
		m_DataFormat = Utils::HazelImageFormatToGLDataFormat(m_Specification.Format);

		glCreateTextures(GL_TEXTURE_2D, 1, & m_RendererID); // 不再使用glGenTextures + glBindTexture
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height); // 分配显存

		// 设置过滤方式
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// 设置包裹模式
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	OpenGLTexture2D::OpenGLTexture2D(const std::string& path) : m_Path(path){
		int width, height, channels;
		stbi_set_flip_vertically_on_load(true); // OpenGL左下角是(0, 0),需要反向

		stbi_uc* data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		if (data) {
			m_IsLoaded = true;
			m_Width = width;
			m_Height = height;

			GLenum internalFormat = 0, dataFormat = 0;
			if (channels == 4) {
				internalFormat = GL_RGBA8;
				dataFormat = GL_RGBA;
			}
			else if (channels == 3) {
				internalFormat = GL_RGB8;
				dataFormat = GL_RGB;
			}

			m_InternalFormat = internalFormat;
			m_DataFormat = dataFormat;
			HZ_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");

			glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
			glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);


			glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

			stbi_image_free(data); //已经加载到显存，释放内存
		}
		else {
			HZ_CORE_WARN("load texture fail: {0}", path.c_str());
		}

		stbi_set_flip_vertically_on_load(false);
	}

	OpenGLTexture2D::~OpenGLTexture2D() {
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTexture2D::SetData(void* data, uint32_t size) {
		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;
		HZ_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!"); // 检查数据量是否对的上
		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
	}

	void OpenGLTexture2D::Bind(uint32_t slot) const {
		glBindTextureUnit(slot, m_RendererID); // 不用老式的Activate + Bind
	}

	/* ------------------------------------ TextureCubeMap ------------------------------------- */
	OpenGLTextureCubeMap::OpenGLTextureCubeMap(const TextureSpecification& specification)
	: m_Specification(specification), m_Width(specification.Width), m_Height(specification.Height){
		m_InternalFormat = Utils::HazelImageFormatToGLInteralFormat(specification.Format);
		m_DataFormat = Utils::HazelImageFormatToGLDataFormat(specification.Format);

		glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height); // 新版本自动为6面分配内存

		
		// TODO: 后续加入纹理过滤方式和包裹方式调整
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}

	OpenGLTextureCubeMap::OpenGLTextureCubeMap(const std::string& dir) : m_Path(dir){
		std::vector<std::string> paths(CreateFromDirectory(dir));
		HZ_CORE_ASSERT(paths.size() == 6, "CubeMap must have 6 textures!");
		LoadFromPaths(paths);
	}

	OpenGLTextureCubeMap::~OpenGLTextureCubeMap() {
		glDeleteTextures(1, &m_RendererID);
	}

	void OpenGLTextureCubeMap::SetData(void* data, uint32_t size) {
		// TODO
	}

	void OpenGLTextureCubeMap::Bind(uint32_t slot /*= 0*/) const {
		glBindTextureUnit(slot, m_RendererID);
	}

	void OpenGLTextureCubeMap::LoadFromPaths(std::vector<std::string> paths) {
		int width, height, channels;
		stbi_uc* data = stbi_load(paths[0].c_str(), &width, &height, &channels, 0);
		m_Width = width; m_Height = height;

		if (channels == 4) {
			m_InternalFormat = GL_RGBA8;
			m_DataFormat = GL_RGBA;
		}
		else if (channels == 3) {
			m_InternalFormat = GL_RGB8;
			m_DataFormat = GL_RGB;
		}

		stbi_image_free(data); // 拿完元数据释放内存

		glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_RendererID);
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		for (int i = 0; i < 6; ++i) {
			stbi_uc* faceData = stbi_load(paths[i].c_str(), &width, &height, &channels, 0);
			HZ_CORE_ASSERT(faceData, "Failed to load cubemap face!");

			// 上传到指定的面
			glTextureSubImage3D(
				m_RendererID,
				0,					// Mip level
				0, 0, i,			// x,y,z offset, z Offset是层级偏移，就是面数
				m_Width, m_Height,
				1,					// depth (一次传一张)
				m_DataFormat,
				GL_UNSIGNED_BYTE,
				faceData
			);

			stbi_image_free(faceData);
		}
	}

	std::vector<std::string> OpenGLTextureCubeMap::CreateFromDirectory(const std::string& dir)
	{
		std::vector<std::string> paths;
		paths.push_back(dir + "/right.jpg");
		paths.push_back(dir + "/left.jpg");
		paths.push_back(dir + "/top.jpg");
		paths.push_back(dir + "/bottom.jpg");
		paths.push_back(dir + "/front.jpg");
		paths.push_back(dir + "/back.jpg");

		return paths;
	}

}