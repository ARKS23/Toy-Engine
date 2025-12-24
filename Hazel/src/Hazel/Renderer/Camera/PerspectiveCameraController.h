#pragma once

#include "Hazel/Renderer/Camera/PerspectiveCamera.h"
#include "Hazel/Core/Timestep.h"
#include "Hazel/Events/MouseEvent.h"
#include "Hazel/Events/ApplicationEvent.h"

namespace Hazel {
	class HAZEL_API PerspectiveCameraController {
	public:
		PerspectiveCameraController(float fov, float aspectRatio, float nearClip, float farClip);

	public:
		void OnEvent(Event& e);
		void OnUpdate(Timestep ts);

		PerspectiveCamera& GetCamera() { return m_Camera; }
		const PerspectiveCamera& GetCamera() const { return m_Camera; }

	private:
		bool OnWindowResized(WindowResizeEvent& e);
		bool OnMouseScrolled(MouseScrolledEvent& e);

	private:
		PerspectiveCamera m_Camera;

		float m_TranslationSpeed = 5.f; // 移动速度
		float m_RotateSpeed = 0.1f;		// 鼠标灵敏度

		glm::vec2 m_LastMousePostion = { 0.f, 0.f }; // 上一帧鼠标位置缓存

		float m_AspectRatio;
		float m_FOV = 45.f;
	};
}

