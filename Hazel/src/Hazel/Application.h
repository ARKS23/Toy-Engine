#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Hazel/Core/Window.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Core/LayerStack.h"

namespace Hazel {

	class HAZEL_API Application {
	public:
		Application();
		virtual ~Application();

	public:
		void Run();
		void OnEvent(Event &e);
		void PushLayer(Layer* layer); // 暴露给客户端使用的接口
		void PushOverlay(Layer* overlay);

	private:
		bool OnWindowClose(WindowCloseEvent& e);

	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
		LayerStack m_LayerStack; // 层栈容器，管理所有层的生命周期
	};

	// 该函数由客户端实现
	Application* CreateApplication();
};

