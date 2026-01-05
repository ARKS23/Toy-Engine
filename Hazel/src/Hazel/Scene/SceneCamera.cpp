#include "hzpch.h"
#include "SceneCamera.h"

namespace Hazel {

	SceneCamera::SceneCamera() {
		RecalculateProjection();
	}

	void SceneCamera::SetPerspective(float verticalFov, float nearClip, float farClip) {
		m_ProjectionType = ProjectionType::Perspeective;
		m_PerspectiveFOV = verticalFov;
		m_PerspectiveNear = nearClip;
		m_PerspectiveFar = farClip;
		RecalculateProjection();
	}

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip) {
		m_ProjectionType = ProjectionType::Orthographic;
		m_OrthographicSize = size;
		m_OrthographicNear = nearClip;
		m_OrthographicFar = farClip;
		RecalculateProjection();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height) {
		HZ_CORE_ASSERT(width > 0 && height > 0, "SetViewportSize Wrong para");
		m_AspectRatio = static_cast<float>(width) / static_cast<float>(height);
		RecalculateProjection();
	}

	void SceneCamera::SetProjectionMatrix(float a, float b, float c, float d) {
		if (m_ProjectionType == ProjectionType::Perspeective) {
			m_ProjectionMatrix = glm::perspective(m_PerspectiveFOV, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar);
		}
		else {
			float orthoLeft = -m_OrthographicSize * m_AspectRatio * 0.5f;
			float orthoRight = m_OrthographicSize * m_AspectRatio * 0.5f;
			float orthoBottom = -m_OrthographicSize * 0.5f;
			float orthoTop = m_OrthographicSize * 0.5f;

			m_ProjectionMatrix = glm::ortho(orthoLeft, orthoRight,
				orthoBottom, orthoTop, m_OrthographicNear, m_OrthographicFar);
		}
	}

	void SceneCamera::RecalculateProjection() {
		SetProjectionMatrix(m_PerspectiveFOV, m_AspectRatio, m_PerspectiveNear, m_PerspectiveFar); // 内部判断投影方式
	}

}