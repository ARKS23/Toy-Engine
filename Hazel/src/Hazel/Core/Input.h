#pragma once

#include "Hazel/Core/Core.h"
#include <unordered_map>
#include "Hazel/Core/KeyCodes.h"
#include "Hazel/Core/MouseCodes.h"

/* 
	Input的设计思路：对外提供静态函数（方便调用），对内调用虚函数（跨平台）
*/

namespace Hazel {
	class HAZEL_API Input {
	public:
		/* 公共API */
		inline static bool IsKeyPressed(KeyCode keycode) { return s_Instance->IsKeyPressedImpl(keycode); }
		inline static bool IsMouseButtonPressed(MouseCode button) { return s_Instance->IsMouseButtonPressedImpl(button); }
		inline static float GetMouseX() { return s_Instance->GetMouseXImpl(); }
		inline static float GetMouseY() { return s_Instance->GetMouseYImpl(); }
		inline static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionImpl(); }

	protected:
		/* 内部实现的纯虚函数，不同平台实现重写自己的虚函数 */
		virtual bool IsKeyPressedImpl(KeyCode keycode) = 0;
		virtual bool IsMouseButtonPressedImpl(MouseCode button) = 0;
		virtual float GetMouseXImpl() = 0;
		virtual float GetMouseYImpl() = 0;
		virtual std::pair<float, float> GetMousePositionImpl() = 0;

	private:
		static Input* s_Instance; // 单例指针，指向当前运行平台的Input实例
	};
}