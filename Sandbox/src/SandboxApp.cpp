#include "Hazel.h" 

// 客户端自定义层
class ExampleLayer : public Hazel::Layer {
public:
	ExampleLayer() : Layer("Example") {}

public:
	void OnUpdate() override {

	}

	void OnEvent(Hazel::Event& event) override {
		if (event.GetEventType() == Hazel::EventType::KeyPressed) {
			Hazel::KeyPressedEvent& e = (Hazel::KeyPressedEvent&)event;
			HZ_TRACE("ExampleLayer 捕获到了按键: {0}", (char)e.GetKeyCode());
		}
	}
};

class Sandbox : public Hazel::Application {
public:
	Sandbox() {
		PushLayer(new ExampleLayer());
	}

	~Sandbox() {

	}
};

Hazel::Application* Hazel::CreateApplication() {
	return new Sandbox();
}