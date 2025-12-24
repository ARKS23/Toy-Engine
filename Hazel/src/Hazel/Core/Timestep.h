#pragma once
#include "Hazel/Core/Core.h"

/* 时间语义封装工具 */
namespace Hazel {
	class HAZEL_API Timestep {
	public:
		Timestep(float time = 0.f) : m_Time(time){}

		operator float() const { return m_Time; }

		float GetSeconds() const { return m_Time; }
		float GetMilliseconds() const { return m_Time * 1000.f; }

	private:
		float m_Time;
	};
}