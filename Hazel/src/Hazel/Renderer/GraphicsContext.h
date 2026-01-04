#pragma once

/* 图形上下文也设计成顶层接口，各平台再对接口进行具体实现 */
namespace Hazel {
	class  HAZEL_API GraphicsContext {
	public:
		virtual void Init() = 0;
		virtual void SwapBuffers() = 0;

		virtual ~GraphicsContext() = default;
	};
}

