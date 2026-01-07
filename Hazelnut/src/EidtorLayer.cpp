#include "EidtorLayer.h"

#include "Panels/SceneHierarchyPanel.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

namespace Hazel {

	EidtorLayer::EidtorLayer() : Layer("EditorLayer") {

	}

	void EidtorLayer::OnAttach() {
		HZ_INFO("EditorLayer Attach!");

		m_ActiveScene = Hazel::CreateRef<Hazel::Scene>();
		m_SceneHierarchyPanel.SetContext(m_ActiveScene);

		FramebufferSpecification fbSpec;
		fbSpec.Width = 1920;
		fbSpec.Height = 1080;
		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::Depth };

		m_Framebuffer = Framebuffer::Create(fbSpec);

		// 手动创建一个绿色圆
		auto circle = m_ActiveScene->CreateEntity("Green Circle"); // 创建实体

		// 添加 CircleRenderer 组件
		auto& circleRenderer = circle.AddComponent<CircleRendererComponent>();
		circleRenderer.Color = { 0.0f, 1.0f, 0.0f, 1.0f }; // 设为绿色
		circleRenderer.Thickness = 1.0f; // 实心
		circleRenderer.Fade = 0.005f;    // 边缘稍微柔化

		/* 测试图形 */
		auto circle2 = m_ActiveScene->CreateEntity("Blue Circle"); 
		auto& circleRenderer2 = circle2.AddComponent<CircleRendererComponent>();
		circleRenderer2.Color = { 0.0f, 0.32f, 1.0f, 1.0f }; 
		circleRenderer2.Thickness = 0.81f; 
		circleRenderer2.Fade = 0.005f;  

		// 相机
		Entity camera = m_ActiveScene->CreateEntity("Main Camera");
		CameraComponent& cc = camera.AddComponent<CameraComponent>();

		cc.Primary = true; // 设为主摄像机
		cc.camera.SetOrthographic(10.0f, -1.0f, 1.0f); // 设置为正交投影，Size=10

		m_SceneHierarchyPanel.SetContext(m_ActiveScene);
	}

	void EidtorLayer::OnDetach() {
		HZ_INFO("EditorLayer Deatch!");
	}

	void EidtorLayer::OnImGuiRender() {
		DrawDockSpace();

		m_SceneHierarchyPanel.OnImGuiRender();

		// 绘制Viewport窗口
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0, 0 }); // 去掉窗口边距，让画面充满
		ImGui::Begin("Viewport"); // 创建一个叫 Viewport 的窗口

		// 读取当前 ImGui 窗口的大小，存起来给 OnUpdate 用来 resize
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		// 把 Framebuffer 的纹理 ID 拿出来，当成图片画在 ImGui 里
		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();

		// 注意：ImGui 的坐标系和 OpenGL 可能是反的 (UV 坐标 0,1 vs 1,0)
		// 如果画面倒了，修改后面的 UV 参数： ImVec2{ 0, 1 }, ImVec2{ 1, 0 }
		ImGui::Image((void*)(uintptr_t)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

		ImGui::End();
		ImGui::PopStyleVar();
	}

	void EidtorLayer::OnUpdate(Timestep ts) {
		if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f && // 确保尺寸有效
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);
		}

		m_Framebuffer->Bind();

		RenderCommand::SetClearColor({ 0.12f, 0.1f, 0.15f, 1.0f });
		RenderCommand::Clear();

		m_ActiveScene->OnUpdateRuntime(ts);

		m_Framebuffer->UnBind();
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