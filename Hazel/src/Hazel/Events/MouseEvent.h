#pragma once

#include "Event.h"
#include <sstream>

namespace Hazel {
	/* 鼠标移动事件 */
	class HAZEL_API MouseMovedEvent : public Event {
	public:
		MouseMovedEvent(float x, float y) : m_MouseX(x), m_MouseY(y) {}

		inline float GetX() const { return m_MouseX; }
		inline float GetY() const { return m_MouseY; }

		std::string ToString() const override {
			std::stringstream ss;
			ss << "MouseMovedEvent: " << m_MouseX << ", " << m_MouseY;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseMoved); // 小类
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput); // 大类

	private:
		float m_MouseX, m_MouseY;
	};

	/* 鼠标滚轮滚动事件 */
	class HAZEL_API MouseScrolledEvent : public Event {
	public:
		MouseScrolledEvent(float xOffset, float yOffset) : m_XOffset(xOffset), m_YOffset(yOffset) {}
		inline float GetXOffset() const { return m_XOffset; }
		inline float GetYOffset() const { return m_YOffset; }

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseScrolledEvent: " << GetXOffset() << ", " << GetYOffset();
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseScrolled); // 小类
		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryInput); //大类

	private:
		float m_XOffset, m_YOffset;
	};

	/* 鼠标按键事件基类(类似键盘按键事件基类) */
	class HAZEL_API MouseButtonEvent : public Event {
	public:
		inline int GetMouseButton() const { return m_Button; }

		EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryMouseButton | EventCategoryInput); // 这里设置公共的大类，小类可能不同，在子类中设置

	protected:
		MouseButtonEvent(int button) : m_Button(button) {} // 后续子类委托构造

		int m_Button;
	};

	// 4. 鼠标按下事件
	class HAZEL_API MouseButtonPressedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonPressedEvent(int button)
			: MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonPressedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonPressed);
	};

	// 5. 鼠标松开事件
	class HAZEL_API MouseButtonReleasedEvent : public MouseButtonEvent
	{
	public:
		MouseButtonReleasedEvent(int button)
			: MouseButtonEvent(button) {}

		std::string ToString() const override
		{
			std::stringstream ss;
			ss << "MouseButtonReleasedEvent: " << m_Button;
			return ss.str();
		}

		EVENT_CLASS_TYPE(MouseButtonReleased);
	};
};