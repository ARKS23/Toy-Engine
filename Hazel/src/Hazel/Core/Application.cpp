#include "hzpch.h"
#include "Application.h"

#include "Hazel/Core/Input.h"
#include "Hazel/Events/ApplicationEvent.h"

#include "Hazel/Utils/PlatformUtils.h"

#include <GLFW/glfw3.h>

/* 测试 */
#include "Hazel/Renderer/Renderer.h"
#include "Hazel/Renderer/VertexArray.h"
#include "Hazel/Renderer/Camera/PerspectiveCameraController.h"
#include "Hazel/Renderer/RenderCommand.h"

namespace Hazel {
	//#define HZ_BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)
	/* 
		等价写法: 接收传入参数e
		m_Window->SetEventCallback([this](Event& e) {
			this->OnEvent(e);
		});
	*/

	Application* Application::s_Instance = nullptr;		// 单例指针

	Application::Application(const ApplicationSpecification& specification) : m_Specification(specification) {
		HZ_PROFILE_FUNCTION();

		// 单例模式
		HZ_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create(WindowProps(specification.Name))); // 根据不同平台创建对应平台的窗口
		m_Window->SetEventCallback(HZ_BIND_EVENT_FN(Application::OnEvent)); // 一旦有事件发生，调用OnEvent函数

		Renderer::Init();	// 记得初始化渲染器

		m_ImGuiLayer = new ImGuiLayer();
		PushOverlay(m_ImGuiLayer); // ImGuiLayer是overlay
	}

	Application::~Application(){}

	void Application::Run() {
		while (m_Running) {
			float time = Time::GetTime();
			Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			RenderCommand::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
			RenderCommand::Clear();

			// update顺序:从下往上更新游戏逻辑，与事件的传播顺序是相反的
			for (Layer* layer : m_LayerStack) 
				layer->OnUpdate(timestep);

			/* --------------------------------- ImGui渲染（UI逻辑）--------------------------------- */
			m_ImGuiLayer->Begin(); // 开启UI帧铺画布
			for (Layer* layer : m_LayerStack)
				layer->OnImGuiRender(); // 各层在画布上绘制
			m_ImGuiLayer->End();	// 画好的提交给CPU处理

			m_Window->OnUpdate();
		}
	}

	void Application::Close(){
		m_Running = false;
	}

	void Application::OnEvent(Event& e) {
		// 打印事件
		//HZ_CORE_TRACE("{0}", e.ToString());

		// 事件分发器
		EventDispatcher dispatcher(e);
		// 如果事件类型是 WindowClose，就调用 OnWindowClose 函数 (这里的逻辑是绑定处理函数，绑定的是OnWindowClose)
		dispatcher.Dispatch<WindowCloseEvent>(HZ_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(Application::OnWindowResize));

		// 从上面的Overlay开始查询，一直查询到最底层
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin(); ) {
			(*--it)->OnEvent(e);  // 从end开始的，往前一位再解引用
			if (e.Handled) break; // 处理完成退出循环
		}
	}

	void Application::PushLayer(Layer* layer) {
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay) {
		m_LayerStack.PushOverlay(overlay);
	}

	// 窗口关闭具体逻辑
	bool Application::OnWindowClose(WindowCloseEvent& e) {
		HZ_CORE_INFO("Application Close");

		Close();
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e) {
		HZ_CORE_INFO("Application Resize Window");
		RenderCommand::SetViewport(0, 0, e.GetWidth(), e.GetHeight());
		return true;
	}
};