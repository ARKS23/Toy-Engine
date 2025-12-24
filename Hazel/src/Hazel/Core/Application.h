#pragma once

#include "Core.h"
#include "Hazel/Events/Event.h"
#include "Hazel/Core/Window.h"
#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Core/LayerStack.h"
#include "Hazel/ImGui/ImGuiLayer.h"

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

		inline Window& GetWindow() { return *m_Window; } // 获取当前使用的Window类
		inline ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

	public:
		static Application& Get() { return *s_Instance; } // 静态函数: 返回Application单例引用

	private:
		/* 测试函数 */
		void TestRenderer();

	private:
		bool OnWindowClose(WindowCloseEvent& e);

	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack; // 层栈容器，管理所有层的生命周期
		float m_LastFrameTime = 0.f; // 用于处理帧间时长

	private:
		static Application* s_Instance; // 单例Application
	};

	// 该函数由客户端实现
	Application* CreateApplication();
};

