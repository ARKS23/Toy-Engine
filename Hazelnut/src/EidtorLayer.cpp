#include "EidtorLayer.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

namespace Hazel {

	EidtorLayer::EidtorLayer() : Layer("EditorLayer") {

	}

	void EidtorLayer::OnAttach() {
		HZ_INFO("EditorLayer Attach!");
	}

	void EidtorLayer::OnDetach() {
		HZ_INFO("EditorLayer Deatch!");
	}

	void EidtorLayer::OnImGuiRender() {
		DrawDockSpace();
	}

	void EidtorLayer::OnUpdate(Timestep ts) {
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		RenderCommand::Clear();
	}

	void EidtorLayer::OnEvent(Event& event) {

	}

	void EidtorLayer::DrawDockSpace() {
		/* ------------------------------------------- 下面这堆模板代码实现Dockspace ------------------------------------------- */
		static bool dockspaceOpen = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;

		ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// ===============================
		//  设置窗口 flags
		// ===============================
		ImGuiWindowFlags window_flags =
			ImGuiWindowFlags_MenuBar |
			ImGuiWindowFlags_NoDocking;

		if (opt_fullscreen)
		{
			const ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->WorkPos);
			ImGui::SetNextWindowSize(viewport->WorkSize);
			ImGui::SetNextWindowViewport(viewport->ID);

			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

			window_flags |= ImGuiWindowFlags_NoTitleBar
				| ImGuiWindowFlags_NoCollapse
				| ImGuiWindowFlags_NoResize
				| ImGuiWindowFlags_NoMove
				| ImGuiWindowFlags_NoBringToFrontOnFocus
				| ImGuiWindowFlags_NoNavFocus;
		}

		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		// ===============================
		// 创建承载 DockSpace 的窗口
		// ===============================
		ImGui::Begin("DockSpace", &dockspaceOpen, window_flags);

		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// ===============================
		// DockSpace 核心
		// ===============================
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		// ===============================
		// MenuBar: 这部分考虑封装成单独函数
		// ===============================
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Exit")) {}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ImGui::End();
	}
}