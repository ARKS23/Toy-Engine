#pragma once

#include "Hazel.h"
#include "Hazel/Scene/Scene.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Hazel/Renderer/Framebuffer.h"

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
		void DrawDockSpace();

	private:
		Ref<Scene> m_ActiveScene;
		SceneHierarchyPanel m_SceneHierarchyPanel;
		Ref<Framebuffer> m_Framebuffer;
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };
	};
}

