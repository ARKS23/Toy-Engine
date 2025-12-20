#include "hzpch.h"
#include "WindowsInput.h"

#include "Hazel/Core/Application.h"
#include "GLFW/glfw3.h"

namespace Hazel {
	// 初始化单例，在Windows平台下s_Instance是WindowsInput的实例
	Input* Input::s_Instance = new WindowsInput();

	bool WindowsInput::IsKeyPressedImpl(KeyCode keycode) {
		// 得到原生指针的函数调用链: 获取 Application 单例 -> 获取 Window 对象 -> 获取原生 GLFWwindow 指针
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());

		// 查询按键状态
		int state = glfwGetKey(window, static_cast<int32_t>(keycode));

		// 按下或按住不放都是按下，返回true
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}

	bool WindowsInput::IsMouseButtonPressedImpl(MouseCode button) {
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		int state = glfwGetMouseButton(window, static_cast<int32_t>(button));
		return state == GLFW_PRESS;
	}

	float WindowsInput::GetMouseXImpl() {
		std::pair<float, float> pos = GetMousePositionImpl();
		return pos.first;
	}

	float WindowsInput::GetMouseYImpl() {
		std::pair<float, float> pos = GetMousePositionImpl();
		return pos.second;
	}

	std::pair<float, float> WindowsInput::GetMousePositionImpl() {
		GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);
		return { static_cast<float>(xPos), static_cast<float>(yPos) };
	}
}
