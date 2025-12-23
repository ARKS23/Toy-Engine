#pragma once

#include "Hazel/Renderer/Camera/Camera.h"

namespace Hazel {
	class PerspectiveCamera : public Camera {
	public:
		PerspectiveCamera(float fov, float aspectRatio, float nearClip, float farClip);

		virtual void SetProjectionMatrix(float fov, float aspectRatio, float nearClip, float farClip) override;
	};
}

