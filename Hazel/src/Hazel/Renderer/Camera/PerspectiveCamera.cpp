#include "hzpch.h"
#include "PerspectiveCamera.h"

namespace Hazel {
	PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio, float nearClip, float farClip) {
		SetProjectionMatrix(fov, aspectRatio, nearClip, farClip);
		UpdateCameraVectors();
	}

	void PerspectiveCamera::SetProjectionMatrix(float fov, float aspectRatio, float nearClip, float farClip) {
		m_ProjectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
		ReCalculateViewMatrix();
	}
}