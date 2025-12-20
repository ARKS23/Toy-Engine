#include "Hazel.h" 

// 客户端自定义层
class ExampleLayer : public Hazel::Layer {
public:
	ExampleLayer() : Layer("Example") {}

public:
	void OnUpdate() override {

	}

	void OnEvent(Hazel::Event& event) override {
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
};

class Sandbox : public Hazel::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
		PushOverlay(new Hazel::ImGuiLayer());
	}

	~Sandbox() {

	}
};

Hazel::Application* Hazel::CreateApplication() {
	return new Sandbox();
}