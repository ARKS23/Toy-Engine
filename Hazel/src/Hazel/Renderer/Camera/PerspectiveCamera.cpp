#include "hzpch.h"
#include "PerspectiveCamera.h"

namespace Hazel {
	PerspectiveCamera::PerspectiveCamera(float fov, float aspectRatio, float nearClip, float farClip) {
		/* 构造函数中做变换矩阵计算和本地坐标系初始化 */
		SetProjectionMatrix(fov, aspectRatio, nearClip, farClip);
		m_Fov = fov;
		m_AspectRatio = aspectRatio;
		m_NearClip = nearClip;
		m_FarClip = farClip;
		UpdateCameraVectors();
	}

	void PerspectiveCamera::SetProjectionMatrix(float fov, float aspectRatio, float nearClip, float farClip) {
		m_ProjectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, nearClip, farClip);
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
		ReCalculateViewMatrix();
	}
}