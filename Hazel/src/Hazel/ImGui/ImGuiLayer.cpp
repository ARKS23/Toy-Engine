#include "hzpch.h"
#include "ImGuiLayer.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include "Hazel/Core/Application.h"

#include <GLFW/glfw3.h>

namespace Hazel {
	ImGuiLayer::ImGuiLayer() : Layer("ImGui Layer") {}

	ImGuiLayer::~ImGuiLayer() {}

	void ImGuiLayer::OnAttach() {
		IMGUI_CHECKVERSION(); // 版本检查
		ImGui::CreateContext(); // 创建上下文

		/* 配置IO */
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // 开启键盘控制
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // 开启手柄控制
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;	// 开启窗口停靠
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // 开启多视口

		// 设置风格
		ImGui::StyleColorsDark();

		// 多视口开启时样式调整
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			style.WindowRounding = 0.0f; // 去掉圆角
			style.Colors[ImGuiCol_WindowBg].w = 1.0f; // 背景非透明
		}

		// 获取Application单例，然后从中获取Window类再从具体Window类中获取原生指针
		Application& app = Application::Get();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		// 初始化后端
		ImGui_ImplGlfw_InitForOpenGL(window, true); // 新版ImGui自动接管了输入，不用进行手动处理
		ImGui_ImplOpenGL3_Init("#version 460");
	}

	void ImGuiLayer::OnDetach() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	

	void ImGuiLayer::OnEvent(Event& e) {
		if (m_BlockEvents) {
			ImGuiIO& io = ImGui::GetIO();
			e.Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;
			e.Handled |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
		}
	}

	void ImGuiLayer::OnImGuiRender() {
		/* 测试使用 */
		static bool show = true;
		ImGui::ShowDemoWindow(&show);
	}

	void ImGuiLayer::Begin() {
		// 开始新的一帧
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
	}

	void ImGuiLayer::End() {
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::Get();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		// 渲染主窗口
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// 处理多视口情况
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
			GLFWwindow* backup_current_context = glfwGetCurrentContext(); // 备份主窗口的上下文
			ImGui::UpdatePlatformWindows(); // 更新所有的独立窗口
			ImGui::RenderPlatformWindowsDefault(); // 独立窗口中执行渲染
			glfwMakeContextCurrent(backup_current_context); // 切回主窗口上下文
		}
	}
}
