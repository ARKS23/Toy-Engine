#include "hzpch.h"
#include "WindowsWindow.h"

#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/MouseEvent.h"
#include "Hazel/Events/KeyEvent.h"

//#include <glad/glad.h>

namespace Hazel {
	// 单例模式: glfwInit只调用一次
	static bool s_GLFWInitialized = false;

	// GLFW出现错误时打印错误信息的回调
	static void GLFWErrorCallback(int error, const char* description) {
		HZ_CORE_ERROR("GLFW Error ({0}: {1})", error, description);
	}

	// 工厂方法实现: Application调用的构建函数
	Window* Window::Create(const WindowProps& props) {
		return new WindowsWindow(props);
	}

	// 构造
	WindowsWindow::WindowsWindow(const WindowProps& props) {
		Init(props);
	}

	// 析构
	WindowsWindow::~WindowsWindow() {
		Shutdown();
	}
	void WindowsWindow::OnUpdate() {
		/* 这两个函数在渲染循环中最后都会调用，封装到update中 */

		// 轮询事件
		glfwPollEvents();
		// 交换缓冲区
		glfwSwapBuffers(m_Window);
	}

	void WindowsWindow::SetVSync(bool enabled) {
		/* 垂直同步设置 */
		if (enabled)
			glfwSwapInterval(1);
		else
			glfwSwapInterval(0);

		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVSync() const
	{
		return m_Data.VSync;
	}

	void WindowsWindow::Init(const WindowProps& props) {
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;
		HZ_CORE_INFO("Creating window {0} {1}, {2}", m_Data.Title, m_Data.Width, m_Data.Height);

		// 单例初始化GLFW
		if (!s_GLFWInitialized) {
			// TODO: 关闭时调用glfwTerminate
			int success = glfwInit();
			// 断言检查
			HZ_CORE_ASSERT(success, "Could not intialize GLFW!");

			glfwSetErrorCallback(GLFWErrorCallback); // 绑定错误信息回调函数
			s_GLFWInitialized = true;
		}

		// 创建窗口
		m_Window = glfwCreateWindow(static_cast<int>(m_Data.Width), static_cast<int>(m_Data.Height), m_Data.Title.c_str(), nullptr, nullptr);

		// 设置OpenGL上下文
		glfwMakeContextCurrent(m_Window);

		// 初始化GLAD
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		HZ_CORE_ASSERT(status, "Failed to initialize Glad!");

		// 打印OpenGL的Log
		HZ_CORE_INFO("OpenGL Info:");
		HZ_CORE_INFO("  Vendor: {0}", (const char*)glGetString(GL_VENDOR));
		HZ_CORE_INFO("  Renderer: {0}", (const char*)glGetString(GL_RENDERER));
		HZ_CORE_INFO("  Version: {0}", (const char*)glGetString(GL_VERSION));

		// 设置自定义指针槽，存储自定义数据,后续设置回调需要使用
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);


		/* ========================================================= 设置回调函数 ============================================================ */
		// 窗口大小改变
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
			// 取回数据
			//WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			data.Width = width;
			data.Height = height;

			// 创建事件，发回Application
			WindowResizeEvent event(width, height);
			data.EventCallback(event);
		});

		// 关闭窗口
		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
			//WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			WindowCloseEvent event;
			data.EventCallback(event);
		});

		// 键盘按键
		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
			//WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			switch (action)
			{
				case GLFW_PRESS:{
					KeyPressedEvent event(key, 0); // 0是repeatCount
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE: {
					KeyReleasedEvent event(key);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT: {
					KeyPressedEvent event(key, 1); // 0是repeatCount
					data.EventCallback(event);
					break;
				}
			}
		});

		// 字符输入
		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode) {
			//WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));
			KeyTypedEvent event(keycode);
			data.EventCallback(event);
		});

		// 鼠标点击
		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
			//WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			switch (action)
			{
				case GLFW_PRESS: {
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE: {
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
					break;
				}
			}
		});

		// 鼠标滚动
		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
			//WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			// 进行类型转换，事件系统设计的是float输入参数
			MouseScrolledEvent event(static_cast<float>(xOffset), static_cast<float>(yOffset));
			data.EventCallback(event);
		});

		// 鼠标移动
		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos) {
			//WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowData& data = *static_cast<WindowData*>(glfwGetWindowUserPointer(window));

			MouseMovedEvent event(static_cast<float>(xPos), static_cast<float>(yPos));
			data.EventCallback(event);
		});
	}
	void WindowsWindow::Shutdown() {
		glfwDestroyWindow(m_Window);
	}
}