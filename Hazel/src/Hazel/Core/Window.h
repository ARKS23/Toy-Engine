#pragma once

#include "Hazel/Core.h"
#include "Hazel/Events/Event.h"

#include <sstream>

namespace Hazel {
	// 窗口属性结构体，避免传入一堆参数给Window
	struct WindowProps{
	public:
		WindowProps(const std::string &title = "Engine", 
			unsigned int width = 1920, unsigned int height = 1080) : Title(title), Width(width), Height(height) {}
	
	public:
		std::string Title;
		unsigned int Width;
		unsigned int Height;
	};

	// 窗口抽象纯虚基类
	class HAZEL_API Window {
	public:
		using EventCallbackFn = std::function<void(Event&)>; // 回调函数: 设置一个函数指针别名，使用它把事件从窗口传回给Application

		virtual ~Window() {}

		// 纯虚函数: 后续不同平台子类实现
		virtual void OnUpdate() = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		// 窗口配置相关的的接口
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		//virtual void* GetNativeWindow() const = 0;

		// 静态工厂方法：在编译期确定使用哪个平台的实现, Application调用该函数创建窗口
		static Window* Create(const WindowProps& props = WindowProps());
	};
}