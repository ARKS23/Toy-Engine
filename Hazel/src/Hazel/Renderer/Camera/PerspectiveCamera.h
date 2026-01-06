#pragma once

#include "Hazel/Renderer/Camera/Camera.h"

namespace Hazel {
	class PerspectiveCamera : public Camera {
	public:
		PerspectiveCamera(float fov, float aspectRatio, float nearClip, float farClip);
		PerspectiveCamera(const glm::mat4& projection) : Camera(projection) {/* ”–…Ëº∆»±œ› */ }

		virtual void SetProjectionMatrix(float fov, float aspectRatio, float nearClip, float farClip) override;

	private:
		float m_Fov, m_AspectRatio, m_NearClip, m_FarClip;
	};
}

