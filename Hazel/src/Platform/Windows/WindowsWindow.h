#pragma once

#include "Hazel/Core/Window.h"

#include <GLFW/glfw3.h>

namespace Hazel {
	class GraphicsContext;

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		// 更新函数处理事件轮询和交换缓冲区
		void OnUpdate() override;

		// 获取窗口宽高
		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }

		// 窗口属性设置
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		virtual void SetVSync(bool enabled) override;
		virtual bool IsVSync() const override;

		// 获取原生窗口指针
		virtual void* GetNativeWindow() const override { return m_Window; }

	private:
		// 内部初始化和关闭函数
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

	private:
		GLFWwindow* m_Window; // 真正工作的GLFW窗口指针
		GraphicsContext* m_Context; // 上下文管理对象

		// 设计这个结构体是为了兼容GLFW的C风格代码
		struct WindowData {
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};
		WindowData m_Data;
	};
}

