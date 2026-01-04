#pragma once

#include "Hazel.h"

namespace Hazel {
	class EidtorLayer : public Layer{
	public:
		EidtorLayer();
		virtual ~EidtorLayer() = default;

		virtual void OnAttach() override;
		virtual void OnDetach() override;

		/* 设计给ImGui渲染 */
		virtual void OnImGuiRender() override;

		/* 核心循环方法 */
		virtual void OnUpdate(Timestep ts) override;
		virtual void OnEvent(Event& event) override;

	private:

	};
}

