#pragma once

#include "Hazel/Core/Core.h"

namespace Hazel {
	enum class ImageFormat {
		None = 0,
		R8,
		RGB8,
		RGBA8,
		RGBA32F
	};

	/* 参数对象化，减少函数的参数量 */
	struct TextureSpecification {
		uint32_t Width = 1;
		uint32_t Height = 1;
		ImageFormat Format = ImageFormat::RGBA8;
		bool GenerateMips = true;	// 是否开启Mipmaps
	};

	class HAZEL_API Texture {
	public:
		virtual ~Texture() = default;

		/* 获取纹理属性的接口 */
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual uint32_t GetRendererID() const = 0;
		virtual const std::string& GetPath() const = 0; // 获取路径，引擎通过路径去重防止重复加载
		virtual const TextureSpecification& GetSpecification() const = 0;

		virtual void SetData(void* data, uint32_t size) = 0; // 程序化生成纹理需要用到的接口
		virtual void Bind(uint32_t slot = 0) const = 0;
		virtual bool IsLoaded() const = 0;

		virtual bool operator==(const Texture& other) const = 0; // 重载相等运算符判断是不是同一个纹理
	};

	class HAZEL_API Texture2D : public Texture {
	public:
		static Ref<Texture2D> Create(const TextureSpecification& specification); // 创建指定参数的纹理，然后通过SetData程序化填充
		static Ref<Texture2D> Create(const std::string& path); // 从系统加载纹理
	};
}

