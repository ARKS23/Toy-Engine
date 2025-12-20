#pragma once

#include "Hazel/Core/Input.h"

namespace Hazel {
	class WindowsInput : public Input {
	protected:
		/* 实现Windows平台的接口 */
		virtual bool IsKeyPressedImpl(int keycode) override;
		virtual bool IsMouseButtonPressedImpl(int button) override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
		virtual std::pair<float, float> GetMousePositionImpl() override;
	};
}

