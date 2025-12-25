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

	};
}

