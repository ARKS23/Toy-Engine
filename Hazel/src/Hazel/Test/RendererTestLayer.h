#pragma once
#include "Hazel/Core/Core.h"
#include "Hazel/Core/Layer.h"
#include "Hazel/Renderer/Camera/PerspectiveCameraController.h"
#include "Hazel/Renderer/Buffer.h"
#include "Hazel/Renderer/VertexArray.h"
#include "Hazel/Renderer/Renderer.h"
#include "Hazel/Renderer/Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Hazel {
	class CameraTestLayer: public Layer{
	public:
		CameraTestLayer() : Layer("Camera Test Layer"), m_PerspectiveCameraController(45, 1920.f / 1080.f, 0.1f, 100.f) {}

	public:
		virtual void OnAttach() override {
			float vertices[] = {
				// back face
				-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
				 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
				 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
				 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
				-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
				-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
				// front face
				-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
				 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
				 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
				 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
				-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
				-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
				// left face
				-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
				-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
				-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
				-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
				-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
				-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
				// right face
				 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
				 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
				 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
				 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
				 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
				 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
				 // bottom face
				 -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
				  1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
				  1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
				  1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
				 -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
				 -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
				 // top face
				 -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
				  1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
				  1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
				  1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
				 -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
				 -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
			};

			unsigned int indexArray[36];
			for (int i = 0; i < 36; ++i) indexArray[i] = i;

			const char* c_vertexSrc = R"(
			#version 460 core
			layout(location = 0) in vec3 aPos;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			void main() {
				gl_Position = u_ViewProjection * u_Transform * vec4(aPos, 1.0);
			} 
		)";
			const std::string vertexSrc(c_vertexSrc);

			const char* c_fargmentSrc = R"(
			#version 460 core
			out vec4 FragColor;

			void main() {
				FragColor = vec4(1.0, 1.0, 1.0, 1.0);
			}
		)";
			const std::string fragmentSrc(c_fargmentSrc);

			Hazel::Renderer::Init();

			// 创建Shader
			m_Shader = Hazel::Shader::Create("CubeShader", vertexSrc, fragmentSrc);

			// 创建VAO,VBO,EBO
			m_VertexArray = Hazel::VertexArray::Create();
			Hazel::Ref<Hazel::VertexBuffer> vertexBuffer = Hazel::VertexBuffer::Create(vertices, sizeof(vertices));
			Hazel::BufferLayout layout = {
				{Hazel::ShaderDataType::Float3, "a_Position"},
				{Hazel::ShaderDataType::Float3, "a_Normal"},
				{Hazel::ShaderDataType::Float2, "a_Texcoords"}
			};
			vertexBuffer->SetLayout(layout);
			Hazel::Ref<Hazel::IndexBuffer> indexBuffer = Hazel::IndexBuffer::Create(indexArray, 36);
			m_VertexArray->AddVertexBuffer(vertexBuffer);
			m_VertexArray->SetIndexBuffer(indexBuffer);
		}

		virtual void OnUpdate(Hazel::Timestep ts) {
			// 摄像机更新
			m_PerspectiveCameraController.OnUpdate(ts);

			// 渲染更新
			// 清屏在Application中，这里不执行
			Hazel::Renderer::BeginScene(m_PerspectiveCameraController.GetCamera());
			m_Shader->Bind();
			glm::mat4 transformMatrix(1.f);
			transformMatrix = glm::translate(transformMatrix, glm::vec3(1.f, 2.f, 3.f));
			transformMatrix = glm::scale(transformMatrix, glm::vec3(1.f));
			Hazel::Renderer::Submit(m_Shader, m_VertexArray, transformMatrix);
			Hazel::Renderer::EndScene();
		}

		virtual void OnEvent(Hazel::Event& e) {
			m_PerspectiveCameraController.OnEvent(e);
		}

	private:
		Hazel::Ref<Hazel::Shader> m_Shader;
		Hazel::Ref<Hazel::VertexArray> m_VertexArray;
		Hazel::PerspectiveCameraController m_PerspectiveCameraController;
	};
}