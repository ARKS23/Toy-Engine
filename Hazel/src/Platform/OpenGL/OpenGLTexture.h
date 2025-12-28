#pragma once

#include "Hazel/Renderer/Texture.h"

namespace Hazel {
	class OpenGLTexture2D : public Texture2D {
	public:
		OpenGLTexture2D(const TextureSpecification& specification);
		OpenGLTexture2D(const std::string& path);
		virtual ~OpenGLTexture2D();

	public:
		virtual uint32_t GetWidth() const override{ return m_Width; }
		virtual uint32_t GetHeight() const override{ return m_Height; }
		virtual uint32_t GetRendererID() const override{ return m_RendererID; }
		virtual const std::string& GetPath() const override{ return m_Path; }
		virtual const TextureSpecification& GetSpecification() const override{ return m_Specification; }

		virtual void SetData(void* data, uint32_t size) override;
		virtual void Bind(uint32_t slot = 0) const override;
		virtual bool IsLoaded() const override { return m_IsLoaded; }

		virtual bool operator==(const Texture& other) const override { return m_RendererID == other.GetRendererID(); }


	private:
		TextureSpecification m_Specification;

		std::string m_Path;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
		bool m_IsLoaded = false;
		GLenum m_InternalFormat, m_DataFormat; // GPU显存中的数据格式和CPU内存中的数据格式
	};


	class OpenGLTextureCubeMap : public TextureCubeMap {
	public:
		OpenGLTextureCubeMap(const TextureSpecification& specification);
		OpenGLTextureCubeMap(const std::string& dir);
		virtual ~OpenGLTextureCubeMap();

	public:
		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const override { return m_Height; }
		virtual uint32_t GetRendererID() const override { return m_RendererID; }
		virtual const std::string& GetPath() const override { return m_Path; }
		virtual const TextureSpecification& GetSpecification() const override { return m_Specification; }

		virtual void SetData(void* data, uint32_t size) override;
		virtual void Bind(uint32_t slot = 0) const override;
		virtual bool IsLoaded() const override { return m_IsLoaded; }

		virtual bool operator==(const Texture& other) const override { return m_RendererID == other.GetRendererID(); }

	private:
		void LoadFromPaths(std::vector<std::string> paths);
		std::vector<std::string> CreateFromDirectory(const std::string& dir);

	private:
		TextureSpecification m_Specification;

		std::string m_Path;
		uint32_t m_Width, m_Height;
		uint32_t m_RendererID;
		bool m_IsLoaded = false;
		GLenum m_InternalFormat, m_DataFormat;
	};
}

