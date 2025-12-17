#pragma once

#include "Event.h"
#include <sstream>

namespace Hazel {
	// 键盘事件基类: 提取公共特征
	class HAZEL_API KeyEvent : public Event {
	public:
		// 获取按键代码
		inline int GetKeyCode() const { return m_KeyCode; }
		// 宏复用：生成获取类别的函数,这里设置两个类别
		EVENT_CLASS_CATEGORY(EventCategoryKeyboard | EventCategoryInput);

	protected:
		// 构造函数设置为protected,不能创建通用的KeyEvent只能创建子类
		KeyEvent(int keycode) : m_KeyCode(keycode) {}

		int m_KeyCode;
	};

	// 按键按下事件
	class HAZEL_API KeyPressedEvent : public KeyEvent {
	public:
		// 按住一个键不放系统会连续发送KeyPressed事件，repeatCount增加, 这里用了委托构造
		KeyPressedEvent(int keycode, int repaetCount) : KeyEvent(keycode), m_RepeatCount(repaetCount) {}

		inline int GetRepeatCount() const { return m_RepeatCount;}

		// 调试用
		std::string ToString() const override {
			std::stringstream ss;
			ss << "KeyPressedEvent:" << m_KeyCode << "(" << m_RepeatCount << " repeats)";
			return ss.str();
		}

		// 宏复用: 生成GetStaticType, GetEventType, GetName
		EVENT_CLASS_TYPE(KeyPressed);

	private:
		int m_RepeatCount;
	};

	// 按键松开事件
	class HAZEL_API KeyReleasedEvent : public KeyEvent {
	public:
		KeyReleasedEvent(int keycode) : KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyReleasedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyReleased);
	};

	// 字符输入事件: 关注生成的字符是什么，一般用于输入框的输入
	class HAZEL_API KeyTypedEvent : public KeyEvent {
	public:
		KeyTypedEvent(int keycode) : KeyEvent(keycode) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "KeyTypedEvent: " << m_KeyCode;
			return ss.str();
		}

		EVENT_CLASS_TYPE(KeyTyped);
	};
};