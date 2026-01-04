#include "Hazel.h"

#include "EidtorLayer.h"
#include "Hazel/Core/EntryPoint.h"

namespace Hazel {
	class Hazelnut : public Application {
	public:
		Hazelnut(const ApplicationSpecification& specification) : Application(specification) {
			PushLayer(new EidtorLayer());
		}

		~Hazelnut() {

		}
	};

	Application* Hazel::CreateApplication() {
		/* 实现Entrypoint的构建函数 */
		ApplicationSpecification spec;
		spec.Name = "Hazelnut";
		return new Hazelnut(spec);
	}
}