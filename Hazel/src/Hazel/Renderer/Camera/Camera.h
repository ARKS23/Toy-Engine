#pragma once

#include <glm/glm.hpp>

namespace Hazel {
	class Camera {
	public:
		Camera() = default;
		virtual ~Camera() = default;

	public:
		// 变换矩阵获取函数
		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjetionMatrix() const { return m_ViewProjectionMatrix; }

		// 位置相关
		const glm::vec3& GetPosition() const { return m_Position; }
		void SetPotion(const glm::vec3& position) { m_Position = position; ReCalculateViewMatrix(); }

		// 摄像机朝向
		const glm::vec3& GetForward() const { return m_Forward; }
		const glm::vec3& GetRight() const { return m_Right; }
		const glm::vec3& GetUp() const { return m_Up; }

		// 欧拉角
		float GetYaw() const { return m_Yaw; }
		float GetPitch() const { return m_Pitch; }
		void SetYaw(float yaw) { m_Yaw = yaw; UpdateCameraVectors(); }
		void SetPitch(float pitch) { m_Pitch = pitch; UpdateCameraVectors(); }

	public:
		virtual void SetProjectionMatrix(float a, float b, float c, float d) = 0; // 正交相机和透视相机参数含义不一样

	protected:
		void UpdateCameraVectors();
		virtual void ReCalculateViewMatrix();

	protected:
		glm::mat4 m_ProjectionMatrix = glm::mat4(1.f);
		glm::mat4 m_ViewMatrix = glm::mat4(1.f);
		glm::mat4 m_ViewProjectionMatrix = glm::mat4(1.f);

		glm::vec3 m_Position = { 0.f, 0.f, 0.f };
		
		glm::vec3 m_Forward;
		glm::vec3 m_Right;
		glm::vec3 m_Up;
		glm::vec3 m_WorldUp = {0.f, 1.f, 0.f};

		// 欧拉角，单位是度
		float m_Yaw = -90.f;
		float m_Pitch = 0.f;
	};
}

