#pragma once
#include "Hazel/Core/Core.h"
#include "Hazel/Events/Event.h"
#include "Hazel/Core/Timestep.h"

namespace Hazel {
	/* 设计Layer作为一套标准接口，想成为layer的东西需要继承layer */
	class HAZEL_API Layer {
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

	public:
		/* 生命周期方法 */
		virtual void OnAttach() {} // 被推入LayerStack时调用
		virtual void OnDetach() {} // 从LayerStack移除时调用

		/* 设计给ImGui渲染 */
		virtual void OnImGuiRender() {}

		/* 核心循环方法 */
		virtual void OnUpdate(Timestep ts) {} // 每帧更新:用于处理游戏逻辑、位置移动、渲染物体
		virtual void OnEvent(Event &event) {} // 事件处理: 响应鼠标点击、按键键盘等事件

		inline const std::string& GetName() const { return m_DebugName; } // 调试用

	protected:
		std::string m_DebugName;
	};
}

