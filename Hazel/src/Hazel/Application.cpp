#include "hzpch.h"
#include "Application.h"

#include "Hazel/Events/ApplicationEvent.h"

#include <GLFW/glfw3.h>

namespace Hazel {
#define HZ_BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)
	/* 等价写法: 接收传入参数e
	m_Window->SetEventCallback([this](Event& e) {
		this->OnEvent(e);
	});
	*/

	Application::Application() {
		m_Window = std::unique_ptr<Window>(Window::Create());
		// 一旦有事件发生，调用OnEvent函数
		m_Window->SetEventCallback(HZ_BIND_EVENT_FN(Application::OnEvent));
	}

	Application::~Application()
	{
	}

	void Application::Run() {
		while (m_Running) {
			glClearColor(0.1, 0, 0.1, 1);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->OnUpdate();
		}
	}

	void Application::OnEvent(Event& e) {
		// 打印事件
		HZ_CORE_TRACE("{0}", e.ToString());

		// 事件分发器
		EventDispatcher dispatcher(e);

		// 如果事件类型是 WindowClose，就调用 OnWindowClose 函数 (这里的逻辑是绑定处理函数，绑定的是OnWindowClose)
		dispatcher.Dispatch<WindowCloseEvent>(HZ_BIND_EVENT_FN(Application::OnWindowClose));
	}

	// 窗口关闭具体逻辑
	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}

};