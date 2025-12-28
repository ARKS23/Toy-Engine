#pragma once
#include "Hazel.h"
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
				-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
				 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
				 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
				 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
				-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
				-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

				-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
				 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
				 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
				 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
				-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
				-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

				-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
				-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
				-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
				-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
				-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
				-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

				 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
				 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
				 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
				 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
				 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
				 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

				-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
				 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
				 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
				 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
				-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
				-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

				-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
				 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
				 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
				 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
				-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
				-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
			};

			float skyboxVertices[] = {
				// positions          
				-1.0f,  1.0f, -1.0f,
				-1.0f, -1.0f, -1.0f,
				 1.0f, -1.0f, -1.0f,
				 1.0f, -1.0f, -1.0f,
				 1.0f,  1.0f, -1.0f,
				-1.0f,  1.0f, -1.0f,

				-1.0f, -1.0f,  1.0f,
				-1.0f, -1.0f, -1.0f,
				-1.0f,  1.0f, -1.0f,
				-1.0f,  1.0f, -1.0f,
				-1.0f,  1.0f,  1.0f,
				-1.0f, -1.0f,  1.0f,

				 1.0f, -1.0f, -1.0f,
				 1.0f, -1.0f,  1.0f,
				 1.0f,  1.0f,  1.0f,
				 1.0f,  1.0f,  1.0f,
				 1.0f,  1.0f, -1.0f,
				 1.0f, -1.0f, -1.0f,

				-1.0f, -1.0f,  1.0f,
				-1.0f,  1.0f,  1.0f,
				 1.0f,  1.0f,  1.0f,
				 1.0f,  1.0f,  1.0f,
				 1.0f, -1.0f,  1.0f,
				-1.0f, -1.0f,  1.0f,

				-1.0f,  1.0f, -1.0f,
				 1.0f,  1.0f, -1.0f,
				 1.0f,  1.0f,  1.0f,
				 1.0f,  1.0f,  1.0f,
				-1.0f,  1.0f,  1.0f,
				-1.0f,  1.0f, -1.0f,

				-1.0f, -1.0f, -1.0f,
				-1.0f, -1.0f,  1.0f,
				 1.0f, -1.0f, -1.0f,
				 1.0f, -1.0f, -1.0f,
				-1.0f, -1.0f,  1.0f,
				 1.0f, -1.0f,  1.0f
			};

			unsigned int indexArray[36];
			for (int i = 0; i < 36; ++i) indexArray[i] = i;

			Hazel::Renderer::Init();

			// 创建Shader
			const std::string srcPath = "assets/shaders/TestShader.glsl";
			m_Shader = Hazel::Shader::Create(srcPath);
			const std::string skyboxSrcPath = "assets/shaders/skybox.glsl";
			m_skyBoxShader = Hazel::Shader::Create(skyboxSrcPath);

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

			m_SkyBoxVAO = Hazel::VertexArray::Create();
			Hazel::Ref<Hazel::VertexBuffer> SkyboxVertexBuffer = Hazel::VertexBuffer::Create(skyboxVertices, sizeof(skyboxVertices));
			Hazel::BufferLayout skyboxLayout = {
				{Hazel::ShaderDataType::Float3, "a_Position"}
			};
			SkyboxVertexBuffer->SetLayout(skyboxLayout);
			m_SkyBoxVAO->AddVertexBuffer(SkyboxVertexBuffer);


			// 创建纹理
			const std::string path = "assets/textures/container2.png";
			m_Texture = Hazel::Texture2D::Create(path);

			// 创建天空盒纹理
			const std::string skyBoxPath = "assets/skybox/Scene_Lake";
			m_SkyboxTexture = Hazel::TextureCubeMap::Create(skyBoxPath);
		}

		virtual void OnUpdate(Hazel::Timestep ts) {
			// 摄像机更新
			m_PerspectiveCameraController.OnUpdate(ts);

			// 渲染更新
			// 清屏在Application中，这里不执行
			Hazel::Renderer::BeginScene(m_PerspectiveCameraController.GetCamera());
			m_Shader->Bind();
			m_Shader->SetInt("u_Texture", 0);
			m_Texture->Bind(0); // 纹理
			glm::mat4 transformMatrix(1.f);
			transformMatrix = glm::translate(transformMatrix, glm::vec3(1.f, 2.f, 3.f));
			transformMatrix = glm::scale(transformMatrix, glm::vec3(1.f));
			Hazel::Renderer::Submit(m_Shader, m_VertexArray, transformMatrix);
			Hazel::Renderer::EndScene();

			// 天空盒
			m_skyBoxShader->Bind();
			m_skyBoxShader->SetMat4("u_Projection", m_PerspectiveCameraController.GetCamera().GetProjectionMatrix());
			m_skyBoxShader->SetMat4("u_View", m_PerspectiveCameraController.GetCamera().GetViewMatrix());
			Hazel::Renderer::DrawSkybox(m_skyBoxShader, m_SkyBoxVAO, m_SkyboxTexture);
		}

		virtual void OnEvent(Hazel::Event& e) {
			m_PerspectiveCameraController.OnEvent(e);
		}

	private:
		Hazel::Ref<Hazel::Shader> m_Shader;
		Hazel::Ref<Hazel::Shader> m_skyBoxShader;
		Hazel::Ref<Hazel::VertexArray> m_VertexArray;
		Hazel::Ref<Hazel::VertexArray> m_SkyBoxVAO;
		Hazel::Ref<Hazel::Texture2D> m_Texture;
		Hazel::Ref<Hazel::TextureCubeMap> m_SkyboxTexture;
		Hazel::PerspectiveCameraController m_PerspectiveCameraController;
	};
}