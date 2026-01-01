#include "hzpch.h"
#include "PerspectiveCameraController.h"
#include "Hazel/Core/KeyCodes.h"
#include "Hazel/Core/MouseCodes.h"
#include "Hazel/Core/Input.h"

namespace Hazel {
	PerspectiveCameraController::PerspectiveCameraController(float fov, float aspectRatio, float nearClip, float farClip)
		: m_Camera(fov, aspectRatio, nearClip, farClip), m_FOV(fov), m_AspectRatio(aspectRatio) {}
	
	void PerspectiveCameraController::OnEvent(Event& e) {
		EventDispatcher dispatcher(e);

		// 绑定事件
		dispatcher.Dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(PerspectiveCameraController::OnWindowResized));
		dispatcher.Dispatch<MouseScrolledEvent>(HZ_BIND_EVENT_FN(PerspectiveCameraController::OnMouseScrolled));
	}
	
	void PerspectiveCameraController::OnUpdate(Timestep ts) {
		// 摄像机移动逻辑
		glm::vec3 position = m_Camera.GetPosition();
		float velocity = m_TranslationSpeed * ts;
		if (Input::IsKeyPressed(Key::W))
			position += m_Camera.GetForward() * velocity;
		if (Input::IsKeyPressed(Key::S))
			position -= m_Camera.GetForward() * velocity;
		if (Input::IsKeyPressed(Key::D))
			position += m_Camera.GetRight() * velocity;
		if (Input::IsKeyPressed(Key::A))
			position -= m_Camera.GetRight() * velocity;
		if (Input::IsKeyPressed(Key::Q))
			position += glm::vec3(0.f, 1.f, 0.f) * velocity;
		if (Input::IsKeyPressed(Key::E))
			position -= glm::vec3(0.f, 1.f, 0.f) * velocity;
		m_Camera.SetPosition(position);

		// 鼠标右键旋转逻辑
		if (Input::IsMouseButtonPressed(Mouse::ButtonRight)) {
			auto [x, y] = Input::GetMousePosition();
			glm::vec2 currentMousePosition = { x, y };

			glm::vec2 delta = (currentMousePosition - m_LastMousePostion) * m_RotateSpeed;
			
			float yaw = m_Camera.GetYaw(), pitch = m_Camera.GetPitch();
			yaw += delta.x;
			pitch -= delta.y;

			// 防万向死锁
			if (pitch > 89.f) pitch = 89.f;
			if (pitch < -89.f) pitch = -89.f;

			m_Camera.SetYaw(yaw);
			m_Camera.SetPitch(pitch);

			m_LastMousePostion = currentMousePosition;
		}
		else {
			// 没有按下也需要同步，防止delta过大视角乱飞
			auto [x, y] = Input::GetMousePosition();
			m_LastMousePostion = { x, y };
		}
	}

	bool PerspectiveCameraController::OnWindowResized(WindowResizeEvent& e) {
		if (e.GetWidth() == 0 || e.GetHeight() == 0) return false;

		/* 窗口大小拉伸，需要重新设置透视投影矩阵 */
		m_AspectRatio = static_cast<float>(e.GetWidth()) / static_cast<float>(e.GetHeight());
		m_Camera.SetProjectionMatrix(m_FOV, m_AspectRatio, 0.1f, 100.f); // 待改进

		return false;
	}

	bool PerspectiveCameraController::OnMouseScrolled(MouseScrolledEvent& e) {
		m_FOV -= e.GetYOffset() * 2.f;
		if (m_FOV < 1.f) m_FOV = 1.f;
		if (m_FOV > 90) m_FOV = 90.f;

		m_Camera.SetProjectionMatrix(m_FOV, m_AspectRatio, 0.1f, 100.f); // 待改进
		return false;
	}
}