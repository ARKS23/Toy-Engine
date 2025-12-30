#pragma once

#include "Hazel.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class FramebufferTestLayer : public Hazel::Layer {
public:
	FramebufferTestLayer() : Layer("FramebufferTestLayer"), m_PerspectiveCameraController(45, 1920.f / 1080.f, 0.1f, 100.f) {}

	virtual void OnAttach() override {
		Hazel::FramebufferSpecification fboSpec;
		fboSpec.Width = 1920;
		fboSpec.Height = 1080;
		fboSpec.Attachments = {
			Hazel::FramebufferTextureFormat::RGBA8,
			Hazel::FramebufferTextureFormat::RED_INTEGER,
			Hazel::FramebufferTextureFormat::Depth
		};
		m_Framebuffer = Hazel::Framebuffer::Create(fboSpec);

		this->Init();
	}

	virtual void OnImGuiRender() override {
		ImGui::Begin("Viewport Testing");
		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID(0);
		// 在 ImGui 窗口里画出这张图
		ImGui::Image((void*)(uintptr_t)textureID, ImVec2{ 1920, 1080 }, ImVec2{ 0, 1 }, ImVec2{ 1, 0 });
		ImGui::End();
	}

	virtual void OnUpdate(Hazel::Timestep ts) override{
		// 这里简化测试，实际应该配合 OnWindowResize 事件
		if (Hazel::FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			spec.Width != 1920 || spec.Height != 1080)
		{
			m_Framebuffer->Resize(1920, 1080);
		}

		m_Framebuffer->Bind();

		Hazel::RenderCommand::SetClearColor({ 0.33f, 0.1f, 0.1f, 1 });
		Hazel::RenderCommand::Clear();
		m_Framebuffer->ClearAttachment(1, -1);

		this->RenderScene(ts);
		m_Framebuffer->UnBind();
	}

private:
	void Init() {
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

	virtual void OnEvent(Hazel::Event& e) override{
		m_PerspectiveCameraController.OnEvent(e);
	}

private:
	void RenderScene(Hazel::Timestep ts) {
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

private:
	Hazel::Ref<Hazel::Framebuffer> m_Framebuffer;

	Hazel::Ref<Hazel::Shader> m_Shader;
	Hazel::Ref<Hazel::Shader> m_skyBoxShader;
	Hazel::Ref<Hazel::VertexArray> m_VertexArray;
	Hazel::Ref<Hazel::VertexArray> m_SkyBoxVAO;
	Hazel::Ref<Hazel::Texture2D> m_Texture;
	Hazel::Ref<Hazel::TextureCubeMap> m_SkyboxTexture;
	Hazel::PerspectiveCameraController m_PerspectiveCameraController;
};