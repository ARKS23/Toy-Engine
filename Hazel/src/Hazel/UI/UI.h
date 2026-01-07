#pragma once

#include "imgui.h"

/* 颜色设置封装类: RAII的方式使用ImGui更加安全 */
namespace Hazel::UI {	// C++17标准namespace 语法糖
	struct ScopedStyleColor {
	public:
		ScopedStyleColor() = default;

		ScopedStyleColor(ImGuiCol idx, ImVec4 color, bool predicate = true) : m_Set(predicate){
			if (predicate) {
				ImGui::PushStyleColor(idx, color);
			}
		}

		ScopedStyleColor(ImGuiCol idx, ImU32 color, bool predicate = true) : m_Set(predicate){
			if (predicate)
				ImGui::PushStyleColor(idx, color);
		}

		~ScopedStyleColor() {
			if (m_Set) {
				ImGui::PopStyleColor();
			}
		}

	private:
		bool m_Set = false;
	};
}