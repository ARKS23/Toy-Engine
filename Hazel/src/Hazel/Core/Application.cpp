#include "hzpch.h"
#include "Application.h"

#include "Hazel/Core/Input.h"
#include "Hazel/Events/ApplicationEvent.h"

#include <GLFW/glfw3.h>

namespace Hazel {
	//#define HZ_BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)
	/* 
		等价写法: 接收传入参数e
		m_Window->SetEventCallback([this](Event& e) {
			this->OnEvent(e);
		});
	*/

	Application* Application::s_Instance = nullptr;

	Application::Application() {
		// 单例模式
		HZ_CORE_ASSERT(!s_Instance, "Application already exists!");
		s_Instance = this;

		// 根据不同平台创建对应平台的窗口
		m_Window = std::unique_ptr<Window>(Window::Create());
		// 一旦有事件发生，调用OnEvent函数
		m_Window->SetEventCallback(HZ_BIND_EVENT_FN(Application::OnEvent));
	}

	Application::~Application()
	{
	}

	void Application::Run() {
		while (m_Running) {
			glClearColor(0.3, 0.2, 0.5, 1);
			glClear(GL_COLOR_BUFFER_BIT);

			// update顺序:从下往上遍历，与事件的传播顺序是相反的
			for (Layer* layer : m_LayerStack) {
				layer->OnUpdate();
			}

			//TEST
			std::pair<float, float> pos = Input::GetMousePosition();
			HZ_CORE_TRACE("{0}, {1}", pos.first, pos.second);

			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e) {
		// 打印事件
		//HZ_CORE_TRACE("{0}", e.ToString());

		// 事件分发器
		EventDispatcher dispatcher(e);
		// 如果事件类型是 WindowClose，就调用 OnWindowClose 函数 (这里的逻辑是绑定处理函数，绑定的是OnWindowClose)
		dispatcher.Dispatch<WindowCloseEvent>(HZ_BIND_EVENT_FN(Application::OnWindowClose));

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
	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		HZ_CORE_INFO("Application Close");
		m_Running = false;
		return true;
	}

};