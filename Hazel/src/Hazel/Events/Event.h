#pragma once

#include "Hazel/Core.h"

#include <string>
#include <functional>
#include <ostream>

namespace Hazel {
	// 事件系统设计的是阻塞事件，立即回应
	// 后续可能改进存储至buffer中

	// 设计各类事件类型
	enum class EventType
	{
		None = 0,
		WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
		AppTick, AppUpdate, AppRender,
		KeyPressed, KeyReleased, KeyTyped,
		MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled, MouseScrolling
	};

	// 给事件类型分大类,这里用了BIT宏：设置整数的二进制第几位为1
	enum EvnetCategory
	{
		None = 0,
		EventCategoryApplication = BIT(0),
		EventCategoryInput = BIT(1),
		EventCategoryKeyboard = BIT(2),
		EventCategoryMouse = BIT(3),
		EventCategoryMouseButton = BIT(4)
	};

	// 每一个具体的 Event 子类都需要实现这三个函数，定义宏快速编写
	#define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::##type; }\
									virtual EventType GetEventType() const override { return GetStaticType(); }\
									virtual const char* GetName() const override { return #type; }

	// 用于生成 GetCategoryFlags 函数
	#define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }


	/* 事件基类 */
	class HAZEL_API Event {
		friend class EventDispatcher; // 友元类，方便让分发器快速访问私有成员
	public:
		virtual EventType GetEventType() const = 0; // 获取具体的类型
		virtual const char* GetName() const = 0;	// 获取事件名字，用于调试
		virtual int GetCategoryFlags() const = 0;   // 获取分类标志位
		virtual std::string ToString() const { return GetName(); } // 打印名字

		inline bool IsInCategory(EvnetCategory category) { return GetCategoryFlags() & category; } // 判断当前事件是否属于某个分类

	public:
		bool Handled = false; // 拦截标志位：如果为true则说明事件已经处理，不继续传给下层
	};

	/* 事件分发器 */
	class EventDispatcher {
		// 定义一个函数指针类型: 返回值bool， 参数T&
		template<typename T>
		using EventFn = std::function<bool(T&)>;

	public:
		EventDispatcher(Event &event): m_Event(event) {}

		// 核心分发逻辑
		template<typename T>
		bool Dispatch(EventFn<T> func) {
			// 运行时检查事件类型是否为想要处理的类型
			if (m_Event.GetEventType() == T::GetStaticType()) {
				m_Event.Handled = func(*(T*)&m_Event); // 进行处理
				return true;
			}
			return false; // 匹配失败，不进行处理
		}

	private:
		Event& m_Event;
	};

	// 重载输出操作符，方便spdlog直接打印Event对象
	inline std::ostream& operator<<(std::ostream& os, const Event& e) {
		return os << e.ToString();
	}
};