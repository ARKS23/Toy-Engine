#include "Hazel.h" 
#include "Test/CameraTestLayer.h"
#include "Test/FramebufferTestLayer.h"

// 客户端自定义层
class ExampleLayer : public Hazel::Layer {
public:
	ExampleLayer() : Layer("Example") {}

public:
	virtual void OnUpdate(Hazel::Timestep ts) override {

	}

	virtual void OnEvent(Hazel::Event& event) override {
		// 测试事件
		if (event.GetEventType() == Hazel::EventType::KeyPressed) {
			Hazel::KeyPressedEvent& e = (Hazel::KeyPressedEvent&)event;
			HZ_TRACE("ExampleLayer 捕获到了按键: {0}", (char)e.GetKeyCode());
		}

		// 测试引擎封装的输入按键映射
		if (Hazel::Input::IsKeyPressed(Hazel::Key::Tab)) {
			HZ_TRACE("引擎输入轮询测试，按下TAB按键");
		}
	}

	virtual void OnImGuiRender() override {
		// 自定义一个窗口
	}
};


/* 客户端自定义App */
class Sandbox : public Hazel::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
		PushLayer(new Hazel::CameraTestLayer()); // 测试摄像机
		PushLayer(new FramebufferTestLayer()); // 测试帧缓冲
	}

	~Sandbox() {

	}
};

Hazel::Application* Hazel::CreateApplication() {
	return new Sandbox();
}