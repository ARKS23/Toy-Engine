#include "hzpch.h"
#include "OpenGLContext.h"

#include "GLFW/glfw3.h"

namespace Hazel {
	OpenGLContext::OpenGLContext(GLFWwindow* window) : m_WindowHandle(window){
		HZ_CORE_ASSERT(m_WindowHandle, "OpenGLContext: WindowHandle is null!");
	}

	void OpenGLContext::Init() {
		glfwMakeContextCurrent(m_WindowHandle); // 设置为当前上下文

		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress); // 初始化GLAD
		HZ_CORE_ASSERT(status, "Failed to initialize Glad!");

		HZ_CORE_INFO("OpenGL Info:"); // 打印软硬件信息
		HZ_CORE_INFO("Vendor:{0}", (const char*)glGetString(GL_VENDOR));
		HZ_CORE_INFO("Renderer:{0}", (const char*)glGetString(GL_RENDERER));
		HZ_CORE_INFO("Version:{0}", (const char*)glGetString(GL_VERSION));
	}

	void OpenGLContext::SwapBuffers() {
		glfwSwapBuffers(m_WindowHandle);
	}

}
