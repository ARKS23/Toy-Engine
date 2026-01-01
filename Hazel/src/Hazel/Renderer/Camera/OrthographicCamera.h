#pragma once

#include "Hazel/Core/Core.h"
#include "Hazel/Renderer/Camera/Camera.h"

namespace Hazel {
	class HAZEL_API OrthographicCamera : public Camera{
	public:
		OrthographicCamera(float left, float right, float bottom, float top);

	public:
		float GetRotation() const { return m_Rotation; }
		void SetRotation(float rotation) { m_Rotation = rotation; ReCalculateViewMatrix(); }

	public:
		virtual void SetProjectionMatrix(float left, float right, float bottom, float top) override;
		virtual void ReCalculateViewMatrix() override;

	private:
		float m_Rotation = 0.0f;
	};
}

