#include "hzpch.h"
#include "Camera.h"

namespace Hazel {
	void Camera::ReCalculateViewMatrix() {
		m_ViewMatrix = glm::lookAt(m_Position, m_Position + m_Forward, m_Up);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
	}

	void Camera::UpdateCameraVectors() {
		// 计算新的front，推导过程在Learn OpenGL笔记Camera章节
		glm::vec3 front;
		front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		front.y = sin(glm::radians(m_Pitch));
		front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		m_Forward = front;

		// 右手坐标系，叉乘顺序用右手螺旋定则确定方向
		m_Right = glm::normalize(glm::cross(m_Forward, m_WorldUp));
		m_Up = glm::normalize(glm::cross(m_Right, m_Forward));

		// 更新View矩阵
		ReCalculateViewMatrix();
	}
}
