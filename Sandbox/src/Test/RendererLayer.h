#pragma once

#include "Hazel.h"

#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class RendererLayer : public Hazel::Layer
{
public:
	RendererLayer() : Layer("RendererLayer"), m_CameraController(1920.0f / 1080.0f)
	{
	}

	virtual void OnAttach() override
	{
		// 1. 加载一张测试纹理
		m_CheckerboardTexture = Hazel::Texture2D::Create("assets/textures/container2.png");
		Hazel::Renderer2D::Init(); // 引擎真正运行时应该初始化渲染器
	}

	virtual void OnDetach() override {}

	virtual void OnUpdate(Hazel::Timestep ts) override
	{
		// 1. 更新摄像机控制 (处理缩放、移动)
		m_CameraController.OnUpdate(ts);

		// 2. 重置渲染统计数据 (每帧清零)
		Hazel::Renderer2D::ResetStats();

		// 3. 清屏
		Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Hazel::RenderCommand::Clear();

		// 4. 开始场景 (这是必须的，用来上传相机矩阵)
		Hazel::Renderer2D::BeginScene(m_CameraController.GetCamera());

		// --- 测试 1: 画一个红色的纯色方块 ---
		// 参数: 位置(x,y), 大小(x,y), 颜色(r,g,b,a)
		Hazel::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, m_SquareColor);

		// --- 测试 2: 画一个带纹理的方块 ---
		// 参数: 位置, 大小, 纹理指针
		Hazel::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 1.5f, 1.5f }, m_CheckerboardTexture);

		// --- 测试 3: 画一个旋转的方块 ---
		// 参数: 位置, 大小, 旋转角度(度数), 颜色
		Hazel::Renderer2D::DrawRotatedQuad({ 1.0f, 1.0f }, { 0.5f, 0.5f }, 45.0f, { 0.2f, 0.8f, 0.2f, 1.0f });

		// --- 压力测试 (可选): 画一堆背景方块来测试 Batching ---
		// 这里的 Z轴设为 -0.1f，保证它在其他物体后面
		for (float y = -5.0f; y < 5.0f; y += 0.5f)
		{
			for (float x = -5.0f; x < 5.0f; x += 0.5f)
			{
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.7f };
				//Hazel::Renderer2D::DrawQuad({ x, y, -0.1f }, { 0.45f, 0.45f }, color);
				Hazel::Renderer2D::DrawQuad({ x, y, -0.1f }, { 0.45f, 0.45f }, m_SquareColor + color);
			}
		}

		// 5. 结束场景 (这里会真正调用 Flush 把东西画出来)
		Hazel::Renderer2D::EndScene();
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Settings");

		// 显示这一帧的渲染数据
		auto stats = Hazel::Renderer2D::GetStats();
		ImGui::Text("Renderer2D Stats:");
		ImGui::Text("Draw Calls: %d", stats.DrawCalls); // 关键指标：越少越好
		ImGui::Text("Quads: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

		// 也可以在这里控制方块颜色
		ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));

		ImGui::End();
	}

	virtual void OnEvent(Hazel::Event& e) override
	{
		m_CameraController.OnEvent(e);
	}

private:
	Hazel::OrthographicCameraController m_CameraController;

	// 资源
	Hazel::Ref<Hazel::Texture2D> m_CheckerboardTexture;

	// 状态数据
	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};