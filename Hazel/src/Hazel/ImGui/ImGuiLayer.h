#pragma once

#include "Hazel/Core/Layer.h"

#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/KeyEvent.h"
#include "Hazel/Events/MouseEvent.h"

namespace Hazel {
	class HAZEL_API ImGuiLayer : public Layer{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event &e) override;

		virtual void OnImGuiRender() override; // 测试用

	public:
		void Begin(); // 开启画布
		void End();   // 绘制画布上的内容

	private:
		float m_Time = 0.0f;
	};
}

