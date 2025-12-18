#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Hazel/Core/Window.h"
#include "Hazel/Events/ApplicationEvent.h"

namespace Hazel {

	class HAZEL_API Application {
	public:
		Application();
		virtual ~Application();

	public:
		void Run();
		void OnEvent(Event &e);

	private:
		bool OnWindowClose(WindowCloseEvent& e);

	private:
		std::unique_ptr<Window> m_Window;
		bool m_Running = true;
	};

	// 该函数由客户端实现
	Application* CreateApplication();
};

